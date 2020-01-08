#include"TransLayer.h"

int TransLayer::connect_unix_remote(const char* remote)
{
    int fd = -1,len = -1;
    struct sockaddr_un u_sock = {0};
    if((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        pp_trace(" get socket error,(%s)",strerror(errno));
        goto ERROR;
    }

    // u_sock.sun_family = AF_UNIX;
    // sprintf(u_sock.sun_path, "agent:%d", getpid());
    // len = offsetof(struct sockaddr_un, sun_path) + strlen(u_sock.sun_path);

    u_sock.sun_family = AF_UNIX;
    strncpy(u_sock.sun_path,remote,sizeof(u_sock.sun_path) -1);
    // len =  offsetof(struct sockaddr_un, sun_path) + strlen(u_sock.sun_path);

    // mark fd as non blocking
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);

    if(connect(fd,(struct sockaddr *)&u_sock, sizeof(u_sock)) != 0)
    {
        if( errno != EALREADY || errno !=  EINPROGRESS )
        {
            pp_trace("connect:(%s) failed as (%s)",remote,strerror(errno));
            goto ERROR;
        }
    }
    pp_trace("connected to %s",remote);

    return fd;

ERROR:
    if(fd > 0)
    {
        close(fd);
    }

    return -1;
}

size_t TransLayer::trans_layer_pool()
{
    if(c_fd  == -1 )
    {
        connect_remote(co_host);
        if(c_fd == -1)
        {
            return -1;
        }
    }
    int fd = c_fd;
    fd_set wfds,efds,rfds;
    FD_ZERO(&wfds);
    FD_ZERO(&efds);
    FD_ZERO(&rfds);
    FD_SET(fd,&wfds);
    FD_SET(fd,&efds);
    FD_SET(fd,&rfds);

    struct timeval tv = {0,w_timeout_ms *1000};

    int retval = select(fd+1,&rfds,&wfds,&efds,&tv);
    if(retval == -1)
    {
        //pp_trace("select return error:(%s)",strerror(errno));
        return -1;
    }else if(retval >0 ){

        if(FD_ISSET(fd,&efds)){
            pp_trace("select fd:(%s) ",strerror(errno));
            goto ERROR;
        }

        if(FD_ISSET(fd,&wfds)){
            if(send_msg_to_collector() == -1){
                goto ERROR;
            }
        }

        if(FD_ISSET(fd,&rfds)){
            if(recv_msg_from_collector() == -1){
                goto ERROR;
            }
        }
    }else{
        // timeout do nothing
        // total =0  ,timeout
    }

    return 0;

ERROR:

    reset_remote();
    // TODO share the offline
    // agent.limit = E_OFFLINE;

    connect_remote(co_host);
    return -1;
}