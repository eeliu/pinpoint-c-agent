//
// Created by Bluse on 1/3/2020.
//

#include <stdarg.h>
#include <cstdio>
#include <functional>
#include <iostream>

#include "common.h"
#include "TransLayer.h"
#include "SharedObj.h"

static log_error_cb _error_cb;
extern PPAgentT global_agent_info;
static uint64_t get_current_msec_stamp();
const static char* CLUSE="clues";

enum E_SHM_OFFSET{
    UNIQUE_ID_OFFSET = 0,
    TRACE_LIMIT_OFFSET= 8,
    TRIGER_OFFSET=16,
};

enum E_ANGET_STATUS{
    E_OFFLINE= 0,
    E_TRACE_PASS,
    E_TRACE_BLOCK
};

class TraceNode
{
public:
    TraceNode(Json::Value& node):
        node(node)
    {
        ancestor_start_time = start_time = 0;
    }
    Json::Value &node;
    uint64_t ancestor_start_time;
    uint64_t start_time;
};

typedef std::stack<TraceNode> Stack;
class PerThreadAgent{
public:
    PerThreadAgent(PPAgentT agent):
    trace_limit(agent.trace_limit),
    timeout_ms( agent.timeout_ms),
    co_host(agent.co_host),
    start_time(0),
    translayer(TransLayer(agent.co_host,agent.timeout_ms))
    {
        this->fetal_error_time = 0;

        /**
         * Max [ 0 ~ pagesize ]
         * |  uid   | timestamp | triger  |
         *   [0~7]      [8~15]    [16~23]
         */
        this->uid       = (int64_t*)((char*)fetch_shared_obj_addr() + UNIQUE_ID_OFFSET);
        this->triger_timestamp = (int64_t*)((char*)fetch_shared_obj_addr() + TRACE_LIMIT_OFFSET);
        this->triger    =  (int64_t*)((char*)fetch_shared_obj_addr() + TRIGER_OFFSET );
        this->app_name = "";
        this->app_id = "";
        this->limit = E_OFFLINE;
        using namespace std::placeholders;
        this->translayer.registerPeerMsgCallback(std::bind(&PerThreadAgent::_handleMsgFromCollector,this,_1,_2,_3));

    }

    ~PerThreadAgent()
    {

    }

    int32_t endTrace(PerThreadAgent* agent)
    {
        if( this->stack.size() == 1 ) // ancestor node
        {
            TraceNode& ancestor =  this->stack.top();
            if(this->limit== E_TRACE_PASS)
            {
                uint64_t timestamp =  get_current_msec_stamp();
                ancestor.node["E"] = this->fetal_error_time != 0?( this->fetal_error_time - ancestor.start_time) : timestamp - ancestor.start_time;
                std::string trace = this->writer.write(ancestor.node);
                Header header;
                header.length  = htonl(trace.size());
                header.type    = htonl(REQ_UPDATE_SPAN);

                std::string buf((char*)&header,sizeof(header));
                buf += trace;

                this->translayer.sendMsgToAgent(buf);
                pp_trace("this span:(%s)",trace.c_str());
            }else if(this->limit== E_TRACE_BLOCK){
                // do nothing
            }
            else { // E_OFF
            }
            ancestor.node.clear();
            this->stack.pop();
            this->translayer.trans_layer_pool();
        }
        else if(this->stack.size() > 1) // descendants
        {
            TraceNode& descendant =this->stack.top();
            uint64_t timestamp =  get_current_msec_stamp();
            descendant.node["E"] =  this->fetal_error_time != 0? ( this->fetal_error_time - descendant.start_time ): timestamp - descendant.start_time;
            this->stack.pop();
        }

        pp_trace("pinpoint_end_trace Done!");
        return this->stack.size();
    }
    
    int32_t startTrace(PerThreadAgent* agent)
    {
        pp_trace("pinpoint_start start");
        uint64_t timestamp =  get_current_msec_stamp();
        if(!this->stack.empty()) // child
        {
            TraceNode& parent = this->stack.top();
            uint node_length = parent.node["calls"].size();
            TraceNode child(parent.node["calls"][node_length]);
            child.node["S"] = timestamp - parent.ancestor_start_time;
            child.ancestor_start_time = parent.ancestor_start_time;
            child.start_time = timestamp;
            this->stack.push(child);
            agent->fetal_error_time = 0; // reset fetal_error_time
        }else{ // ancestor
            this->translayer.trans_layer_pool();
            TraceNode ancestor(this->root);
            ancestor.node["S"] = timestamp;
            ancestor.node["FT"]= PHP;
            ancestor.ancestor_start_time = timestamp;
            ancestor.start_time = timestamp;
            this->stack.push(ancestor);
        }
        return this->stack.size();
    }

    inline uint getStackSize()
    {
        return this->stack.size();
    }

    void AddClue(const char* key,const char* value)
    {
        if(this->limit != E_TRACE_PASS)
        {
            return ;
        }

        if(this->stack.size()> 0)
        {
            Json::Value& curNode = this->stack.top().node;
            curNode[key] = value;
            pp_trace("key:%s value:%s",key,value);
        }else{
            pp_trace("%d: stack is empty,error found.",__LINE__);
        }
    }

    void AddClues(const char* key,const char* value)
    {
        if(this->limit != E_TRACE_PASS)
        {
            return ;
        }

        if(this->stack.size()> 0)
        {
            Json::Value& curNode = this->stack.top().node;
            std::string cvalue ="";
            cvalue+=key;
            cvalue+=':';
            cvalue+=value;
            curNode[CLUSE].append(cvalue);
            pp_trace("cluse:%s",curNode[CLUSE].asCString());
        }else{
           pp_trace("%d: stack is empty,error found.",__LINE__);
        }
    }

    bool checkTraceLimit(int64_t timestamp)
    {
        time_t ts = (timestamp == -1) ?(timestamp) :(time(NULL));

        if(this->trace_limit < 0)
        {
        }else if(this->trace_limit == 0)
        {
            return true;
        }
        else if(*this->triger_timestamp != ts )
        {
            __sync_synchronize();
            *this->triger_timestamp = ts;
            *triger = 0 ;
        }
        else if(*triger >= this->trace_limit)
        {
            return true;
        }else
        {
            __sync_add_and_fetch(this->triger,1);
            pp_trace("triger:%ld",*triger);
        }
        return false;
    }

    inline void setLimit(E_ANGET_STATUS status )
    {
        this->limit = status;
    }

    uint64_t generateUniqueId()
    {
        return __sync_fetch_and_add(this->uid,1);
    }

    const char* formatLogging(const char *format,va_list args);

private:

    void _handleMsgFromCollector(int type,const char* buf,size_t len)
    {
        switch (type)
        {

        case RESPONSE_AGENT_INFO:
            this->_handle_agent_info(type,buf,len);
            break;
        
        default:
            break;
        }
    
    }

    void _handle_agent_info(int type,const char* buf,size_t len)
    {
        Json::Value  root;
        Json::Reader reader;
        int ret = reader.parse(buf,buf+len,root);
        if(!ret)
        {
            return ;
        }

        if(root.isMember("time")){
            this->start_time = atoll(root["time"].asCString());
        }

        if(root.isMember("id")){
            this->app_id      =  strdup(root["id"].asCString());
        }

        if(root.isMember("name")){
            this->app_name    =  strdup(root["name"].asCString());
        }

        this->limit= E_TRACE_PASS;

        pp_trace("starttime:%ld appid:%s appname:%s",this->start_time,this->app_id.c_str(),this->app_name.c_str());
    }


private:
    const char* co_host; // tcp:ip:port should support dns
    uint  timeout_ms;
    E_ANGET_STATUS  limit;
    const int   trace_limit;
    int   fetal_error_time;
    int64_t * triger_timestamp;
    int64_t*  triger;
    int64_t*  uid;
    char log_buf[LOG_SIZE]={0};
    std::string app_name;
    std::string app_id;
    uint64_t start_time;
    Json::Value root;
    TransLayer translayer;
    Stack stack;
    Json::FastWriter writer;
};


#ifdef __linux__
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>


#define getOSPid getpid
pthread_key_t key;
pthread_once_t init_done = PTHREAD_ONCE_INIT;
static void _init_common_shared() __attribute__((constructor));
static void _free_common_shared() __attribute__((destructor));
static void thread_init(void);
static void free_agent(void *agent);

void _init_common_shared()
{
    // initialize thread key
    pthread_once(&init_done,thread_init);
    // initialize shm object
    if(!checking_and_init())
    {
        fprintf(stderr,"[pinpoint] initialize pinpoint module failed!");
    }
}

void _free_common_shared()
{
    detach_shared_obj();
}


void thread_init(void)
{
   pthread_key_create(&key, free_agent);
}

// register when thread/module/process start
void register_error_cb(log_error_cb error_cb)
{
    _error_cb  = error_cb;
}

PerThreadAgent* get_agent()
{
    void* spec = pthread_getspecific(key);
    if( unlikely(spec == NULL) ){
        try{
            PerThreadAgent * agent = new PerThreadAgent(global_agent_info);
            spec = (void*)agent;
            pthread_setspecific(key,agent);
        }catch(...){
            pp_trace("create PerThreadAgent failed");
            return NULL;
        }
    }
    return static_cast<PerThreadAgent*>(spec);
}

void free_agent(void *agent)
{
    if(agent){
        delete static_cast<PerThreadAgent*>(agent);
    }
}


const char* PerThreadAgent::formatLogging(const char *format,va_list ap)
{
    char* pstart = this->log_buf;
    int n = snprintf(this->log_buf,LOG_SIZE,"[pinpoint] [%d] ",getOSPid());
    vsnprintf(pstart+n, LOG_SIZE -n - 1 ,format, ap);
    va_end(ap);
    return pstart;
}

#elif _WIN32
#include <processthreadsapi.h>
#define getOSPid GetCurrentProcessId
#else
#error "not support"
#endif

void pp_trace(const char *format,...)
{
    PerThreadAgent* p_agent = get_agent();
    if(p_agent == NULL)
    {
        return ;
    }
    va_list args;
    va_start(args, format);
    const char* pstart = p_agent->formatLogging(format,args);
    if (_error_cb){
        _error_cb(pstart);
    }else{
        fprintf(stderr,"%s\n",pstart);
    }
}

int32_t pinpoint_start_trace()
{
    PerThreadAgent* p_agent = get_agent();
    if(p_agent == NULL)
    {
        return 0;
    }
    return p_agent->startTrace(p_agent);

}


int32_t pinpoint_end_trace()
{
    PerThreadAgent* p_agent = get_agent();
    if(p_agent == NULL)
    {
        return 0;
    }
    return p_agent->endTrace(p_agent);
}

uint64_t get_current_msec_stamp()
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec*1000 + tv.tv_usec /1000;
}

void pinpoint_add_clue(const  char* key,const  char* value)
{
    PerThreadAgent* p_agent = get_agent();
    if(p_agent == NULL)
    {
        return ;
    }
    return p_agent->AddClue(key,value);
}

void pinpoint_add_clues(const  char* key, const char* value)
{
    PerThreadAgent* p_agent = get_agent();
    if(p_agent == NULL)
    {
        return ;
    }
    return p_agent->AddClue(key,value);
}

/**
 * True: drop this trace as time_limit
 * False: do nothing
 */
bool check_tracelimit(int64_t timestamp)
{
    PerThreadAgent* p_agent = get_agent();
    if(p_agent == NULL)
    {
        return false;
    }

    return p_agent->checkTraceLimit(timestamp);
}

int64_t generate_unique_id()
{
    PerThreadAgent* p_agent = get_agent();
    if(p_agent == NULL)
    {
        return 0;
    }
    return (int64_t)p_agent->generateUniqueId();
}

void test_trace()
{
    PerThreadAgent* p_agent = get_agent();
    if(p_agent == NULL)
    {
        return ;
    }
    p_agent->setLimit(E_TRACE_PASS);
}

void pinpoint_drop_trace()
{
    PerThreadAgent* p_agent = get_agent();
    if(p_agent == NULL)
    {
        return ;
    }
    p_agent->setLimit(E_TRACE_BLOCK); 
}