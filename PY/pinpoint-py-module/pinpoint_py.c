#include <Python.h>
#include <string.h>
#include "pinpoint_py.h"


PPAgentT global_agent_info;
static PyObject *py_obj_msg_callback;
static char* g_collector_host ;

#ifndef CYTHON_UNUSED
# if defined(__GNUC__)
#   if !(defined(__cplusplus)) || (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4))
#     define CYTHON_UNUSED __attribute__ ((__unused__))
#   else
#     define CYTHON_UNUSED
#   endif
# elif defined(__ICC) || (defined(__INTEL_COMPILER) && !defined(_MSC_VER))
#   define CYTHON_UNUSED __attribute__ ((__unused__))
# else
#   define CYTHON_UNUSED
# endif
#endif


/**
 * void pinpoint_add_clues(const  char* key,const  char* value);
 * */
static PyObject *py_pinpoint_add_clues(PyObject *self, PyObject *args) 
{
    char* key = NULL;
    char* value = NULL;
    if(PyArg_ParseTuple(args,"ss",&key,&value))
    {
        pinpoint_add_clues(key,value);
    }
    return Py_BuildValue("O",Py_True);
}

/**
 * void pinpoint_add_clue(const  char* key,const  char* value);
*/
static PyObject *py_pinpoint_add_clue(PyObject *self, PyObject *args)
{
    char* key = NULL;
    char* value = NULL;
    if(PyArg_ParseTuple(args,"ss",&key,&value))
    {
       pinpoint_add_clue(key,value);
    }
    return Py_BuildValue("O",Py_True);
}


/**
 * void pinpoint_set_special_key(const  char* key,const  char* value);
*/
static PyObject *py_pinpoint_set_key(PyObject *self, PyObject *args)
{
    char* key = NULL;
    char* value = NULL;
    if(PyArg_ParseTuple(args,"ss",&key,&value))
    {
       pinpoint_set_special_key(key,value);
    }
    return Py_BuildValue("O",Py_True);
}


/**
 * const  char* pinpoint_get_special_key(const  char* key);
*/
static PyObject *py_pinpoint_get_key(PyObject *self, PyObject *args)
{
    char* key = NULL;
    if(PyArg_ParseTuple(args,"s",&key))
    {
        const char* data = pinpoint_get_special_key(key);
        return Py_BuildValue("s",data);
    }
    else
    {
        return Py_BuildValue("O",Py_False);
    }
}

/**
 * bool check_tracelimit(int64_t timestamp);
*/
static PyObject *py_check_tracelimit(PyObject *self, PyObject *args)
{
    int64_t timestamp= -1;
    bool ret;
    if(! PyArg_ParseTuple(args,"|l",&timestamp))
    {
        return NULL;
    }
    ret = check_tracelimit(timestamp);
    if(ret == true){
         return Py_BuildValue("O",Py_True);
    }else{
        return Py_BuildValue("O",Py_False);
    }

}

static PyObject *py_pinpoint_start_trace(PyObject *self,CYTHON_UNUSED  PyObject *unused)
{
    int ret = 0;
    if(global_agent_info.debug_report == 1)
    {
        ret = pinpoint_start_trace();
    }else{
        Py_BEGIN_ALLOW_THREADS
        ret = pinpoint_start_trace();
        Py_END_ALLOW_THREADS
    }

    return Py_BuildValue("i", ret);
}


static PyObject *py_pinpoint_end_trace(PyObject *self, CYTHON_UNUSED PyObject *unused)
{
    int ret = 0;
    if(global_agent_info.debug_report == 1)
    {
        ret = pinpoint_end_trace();
    }else
    {
        Py_BEGIN_ALLOW_THREADS
        ret = pinpoint_end_trace();
        Py_END_ALLOW_THREADS
    }

    return Py_BuildValue("i", ret);
}

static PyObject *py_generate_unique_id(PyObject *self, CYTHON_UNUSED PyObject *unused)
{
    uint64_t ret = generate_unique_id();
    return Py_BuildValue("l", ret);
}

static PyObject *py_pinpoint_drop_trace(PyObject *self, CYTHON_UNUSED PyObject *unused)
{
    pinpoint_drop_trace();
    return Py_BuildValue("O",Py_True);
}

static PyObject *py_pinpoint_app_id(PyObject *self, CYTHON_UNUSED PyObject *unused)
{
    const char* app_id = pinpoint_app_id();

    return Py_BuildValue("s",app_id);
}

static PyObject *py_pinpoint_app_name(PyObject *self, CYTHON_UNUSED PyObject *unused)
{
    const char* app_name = pinpoint_app_name();

    return Py_BuildValue("s",app_name);
}


static PyObject *py_pinpoint_start_time(PyObject *self, CYTHON_UNUSED PyObject *unused)
{
    uint64_t start_time = pinpoint_start_time();
    return Py_BuildValue("l",start_time);
}

//origin from https://docs.python.org/3/extending/extending.html#calling-python-functions-from-c

static void msg_log_error_cb(char* msg)
{
    if(py_obj_msg_callback && msg)
    {
        PyObject *arglist;
        PyObject *result;
        arglist = Py_BuildValue("(s)", msg);
        result = PyObject_CallObject(py_obj_msg_callback, arglist);
        if(result == NULL)
        {
            fprintf(stderr,"%s",msg);
            PyErr_SetString(PyExc_TypeError, msg);
            abort();
            return ;
        }
        Py_XDECREF(result); // I don't care return
        Py_DECREF(arglist);
    }
}


static PyObject *py_pinpoint_enable_utest(PyObject *self, PyObject *args)
{
    global_agent_info.debug_report = 1;

    PyObject *temp;
    if (PyArg_ParseTuple(args, "O:callback", &temp)) 
    {
        if (PyCallable_Check(temp)) 
        {
            Py_XINCREF(temp);
            Py_XDECREF(py_obj_msg_callback);
            py_obj_msg_callback = temp;  
        }
    }

// disable GIL 

// register msg callback
    register_error_cb(msg_log_error_cb);
    enable_trace_utest();

    return Py_BuildValue("O",Py_True);
}

bool set_collector_host(char* host)
{
    if(strcasestr(host,"unix") || strcasestr(host,"tcp"))
    {
        if(g_collector_host)
        {
            free(g_collector_host);
            g_collector_host = NULL;
        }

        g_collector_host = strdup(host);
        
        // NOTE: co_host must be protected when writting
        
        global_agent_info.co_host = g_collector_host;

        return true;
    }
    PyErr_SetString(PyExc_TypeError, "collector_host must start with unix/tcp");
    return false;
}


static PyObject *py_set_collector(PyObject *self, PyObject *args, PyObject *keywds)
{
    // PyObject* setting;
    bool ret = false;
    static char *collector_list[] = {"collector_host", "trace_limit", NULL};
    char* collector_host = "unix:/tmp/collector-agent.sock";
    long trace_limit = -1;
    if(PyArg_ParseTupleAndKeywords(args,keywds,"s|l|",collector_list,&collector_host, &trace_limit))
    {

        global_agent_info.get_write_lock();
        ret = set_collector_host(collector_host);
        if( !ret )
        {
            goto END_OF_PARSE;
        }

        global_agent_info.trace_limit = trace_limit;
        pp_trace("collector_host:%s",collector_host);
        pp_trace("trace_limit:%ld",trace_limit);
END_OF_PARSE:
        global_agent_info.release_lock();

        if( ret == true ){
            return Py_BuildValue("O",Py_True);
        }
        return NULL;

//         PyObject* py_collector_host = Py_BuildValue("s","collector_host");
//         PyObject* py_trace_limit = Py_BuildValue("s","trace_limit");
//         global_agent_info.get_write_lock();

//         if(PyDict_Contains(setting,py_collector_host) == 1)
//         {
//             PyObject* py_host = PyDict_GetItem(setting,py_collector_host);
//             if(PyBytes_Check(py_host))
//             {
//                 char* host = strdup(PyBytes_AsString(py_host));
//                 ret = set_collector_host(host);
//                 free(host);
//                 if( !ret )
//                 {
//                     goto END_OF_PARSE;
//                 }
//             }else{
//                 PyErr_SetString(PyExc_TypeError, "collector_host must a string");
//                 goto END_OF_PARSE;
//             }
//         }

//         if(PyDict_Contains(setting,py_trace_limit) == 1)
//         {
//             PyObject* py_limit = PyDict_GetItem(setting,py_trace_limit);
//             if(!PyLong_Check(py_limit))
//             {
//                 PyErr_SetString(PyExc_TypeError, "trace_limit must a long");
//                 goto END_OF_PARSE;
//             }
//             global_agent_info.trace_limit = PyLong_AsLong(py_limit);
//         }

//  END_OF_PARSE:
//         global_agent_info.release_lock();
//         Py_DECREF(py_collector_host);
//         Py_DECREF(py_trace_limit);
//         if( ret == true ){
//              return Py_BuildValue("O",Py_True);
//         }
//         return NULL;
    }
    else
    {
        PyErr_SetString(PyExc_TypeError, "parameters should be collector_host=\"unix:/tmp/collector-agent.sock or tcp:host:port\",trace_limit=100");
        return NULL;
    }
}



static void free_pinpoint_module(void * module)
{
    Py_XDECREF(py_obj_msg_callback);
    if (g_collector_host)
    {
        free(g_collector_host);
    }

}


/* Module method table */
static PyMethodDef PinpointMethods[] = {
    {"start_trace", py_pinpoint_start_trace, METH_NOARGS, "def start_trace():# create a new trace and insert into trace chain"},
    {"end_trace", py_pinpoint_end_trace, METH_NOARGS, "def end_trace():# end currently matched trace"},
    {"unique_id", py_generate_unique_id, METH_NOARGS, "def unique_id()-> long"},
    {"drop_trace", py_pinpoint_drop_trace, METH_NOARGS, "def drop_trace():# drop this trace"},
    {"app_id", py_pinpoint_app_id, METH_NOARGS, "def app_id()->string"},
    {"app_name", py_pinpoint_app_name, METH_NOARGS, "def app_name()->string"},
    {"start_time", py_pinpoint_start_time, METH_NOARGS, "def start_time()->long"},
    {"add_clues", py_pinpoint_add_clues, METH_VARARGS, "def add_clues(string key,string value)"},
    {"add_clue", py_pinpoint_add_clue, METH_VARARGS, "def add_clue(string key,string value)"},
    {"set_special_key", py_pinpoint_set_key, METH_VARARGS, "def set_special_key(string key,string value): # create a key-value pair that bases on current trace chain"},
    {"get_special_key", py_pinpoint_get_key, METH_VARARGS, "def get_special_key(key)->string "},
    {"check_tracelimit", py_check_tracelimit, METH_VARARGS, "check_tracelimit(long timestamp): check trace whether is limit"},
    {"enable_debug", py_pinpoint_enable_utest, METH_VARARGS, "enable logging output(callback )"},
    {"set_collector",(PyCFunction)py_set_collector, METH_VARARGS|METH_KEYWORDS, "def set_collector(collector_host=\"unix:/tmp/collector-agent.sock or tcp:host:port\",trace_limit=100)"},
    { NULL, NULL, 0, NULL}
};

/* Module structure */
static struct PyModuleDef pinpointmodule = {
    PyModuleDef_HEAD_INIT,
    "pinpoint",           /* name of module */
    "python agent for pinpoint platform",  /* Doc string (may be NULL) */
    -1,                 /* Size of per-interpreter state or -1 */
    PinpointMethods,       /* Method table */
    NULL,
    NULL,
    NULL,
    free_pinpoint_module /* free global variables*/
};



/* Module initialization function */
PyMODINIT_FUNC
PyInit_pinpoint(void) {
    
    global_agent_info.agent_type=1700;
    global_agent_info.co_host = "unix:/tmp/collector.sock";
    global_agent_info.debug_report = 0;
    global_agent_info.timeout_ms = 0;
    global_agent_info.trace_limit = -1;

  return PyModule_Create(&pinpointmodule);
}
