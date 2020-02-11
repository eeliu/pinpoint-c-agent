/*
 * PPTransLayer.h
 *
 *  Created on: Jan 6, 2020
 *      Author: eeliu
 */

#ifndef INCLUDE_PPTRANSLAYER_H_
#define INCLUDE_PPTRANSLAYER_H_

#include "common.h"
#include "Chunk.h"

#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <functional>
#include <iostream>
#include <arpa/inet.h>
#include <iostream>
#include <functional>

// typedef struct trans_layer_t TransLayer;
// typedef int  (*TransHandleCB)(TransLayer*);
// typedef struct trans_layer_t{
//     int           c_fd;         // collector fd, use to send data;
//     Chunks*        chunks;      // A fixed size for span [0,MAX_VEC]
//     TransHandleCB socket_read_cb;
//     TransHandleCB socket_write_cb;
//     char           in_buf[IN_MSG_BUF_SIZE];
// }TransLayer;

const static char* UNIX_SOCKET = "unix:";
const static char* TCP_SOCKET = "tcp:";

class TransLayer{

enum E_STATE{S_WRITTING,S_READING};

public:
explicit TransLayer(const char* co_host,uint w_timeout_ms):
    c_fd(-1),
    co_host(co_host),
    w_timeout_ms(w_timeout_ms),
    _state(S_READING),
    chunks(1024*1024,1024)
    {
    }

    void registerPeerMsgCallback(std::function<void(int type,const char* buf,size_t len)> _peerMsgCallback)
    {
        if(_peerMsgCallback){
            this->peerMsgCallback = _peerMsgCallback;
        }
    }

    size_t trans_layer_pool();
    
    void sendMsgToAgent(const std::string &data)
    {
        uint32_t len = data.size();
        if ( this->chunks.copyDataIntoChunks(data.data(),len) != 0)
        {
            pp_trace("Send buffer is full. size:[%d]",len);
            return ;
        }
    }

private:

    int connect_unix_remote(const char* remote);
   

    int connect_remote(const char* statement)
    {
        int fd = -1;
        const char* substring = NULL;
        if(statement == NULL || statement[0] == '\0')
        {
            goto DONE;
        }

        /// unix
        substring = strcasestr(statement,UNIX_SOCKET);
        if( substring == statement )
        {
            // sizeof = len +1, so substring -> /tmp/collector.sock
            substring = substring + strlen(UNIX_SOCKET);
            fd = connect_unix_remote(substring);
            c_fd = fd;
            goto DONE;
        }

        ///  tcp

        ///  udp

        pp_trace("remote is not valid:%s",statement);
    DONE:
        /// Add  whoamI info

        return fd;
    }

    int send_msg_to_collector()
    {
        return chunks.drainOutWithPipe(std::bind(&TransLayer::do_write_data,this,std::placeholders::_1,std::placeholders::_2));
    }

    void reset_remote( )
    {
        if(c_fd > 0)
        {
            pp_trace("reset peer:%d",c_fd);
            close(c_fd);
            c_fd = -1;
        }

        chunks.resetChunks();
    }

    int do_write_data(const char *data,uint length)
    {
        const char* buf = data;
        uint buf_ofs = 0;
        while(buf_ofs < length){
    #ifdef __APPLE__
            ssize_t ret = send(c_fd,buf + buf_ofs,length -buf_ofs ,0);
    #else
            ssize_t ret = send(c_fd,buf + buf_ofs,length -buf_ofs ,MSG_NOSIGNAL);
    #endif

            if(ret > 0){
                buf_ofs += (uint) ret;
            }else if(ret == -1){
                if(errno == EAGAIN || errno == EWOULDBLOCK || errno== EINTR){
                    return buf_ofs;
                }
                pp_trace("%d send data error:(%s) fd:(%d)",__LINE__,strerror(errno),c_fd);
                return -1;
            }
            else{
                pp_trace("%d send data return 0 error:(%s) fd:(%d)",__LINE__,strerror(errno),c_fd);
                return -1;
            }
        }
        return length;
    }

    int recv_msg_from_collector()
    {
        int next_size = 0;
        while(next_size < IN_MSG_BUF_SIZE){
            int ret = recv(c_fd,in_buf + next_size,IN_MSG_BUF_SIZE -next_size ,0);
            if (ret > 0){
                int total = ret + next_size;
                int msg_offset = handle_msg_from_collector(in_buf,total);
                if(msg_offset < total){
                    next_size = total - msg_offset;
                    memcpy(in_buf,in_buf + msg_offset ,next_size);
                }else{
                    next_size = 0 ;
                }
            }else if(ret == 0){
                // peer close
                return -1;
            }else{
                if(errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR){
                    return 0;
                }
                pp_trace("recv with error:%s",strerror(errno));
                return -1;
            }
        }
        return 0;
    }

    int handle_msg_from_collector(const char* buf,size_t len)
    {
        size_t offset = 0;
        while( offset + 8 <= len ){

            Header * header= (Header*)buf;

            uint body_len = ntohl(header->length);

            if( 8+ body_len > len ){
                return offset ;
            }

            uint type = ntohl(header->type);
            if(peerMsgCallback)
            {
                peerMsgCallback(type, buf+8,len - 8);
            }

            // switch(type){
            // case RESPONSE_AGENT_INFO:
            // // TODO add agent_info update
            //     // handle_agent_info(RESPONSE_AGENT_INFO, buf+8,len - 8);
            //     break;
            // default:
            //     pp_trace("unsupport type:%d",type);
            // }

            offset += (8 +body_len );

        }
        return offset;
    }

private:
   
    Chunks        chunks;
    const char*   co_host;
    uint          w_timeout_ms;
    E_STATE       _state;
    char          in_buf[IN_MSG_BUF_SIZE]= {0};
    std::function<void(int)> stateChangeCallBack;
    std::function<void(int type,const char* buf,size_t len)> peerMsgCallback;
public:
    int           c_fd;
};


#endif /* INCLUDE_PPTRANSLAYER_H_ */
