#include"TransLayer.h"
#include <netdb.h>

/**
 * remote: localhost:port
*/
int TransLayer::connect_stream_remote(const char* remote)
{

    int offset = strlen(remote) - 1;

    while( remote[offset] !=':' )
    {
        offset--;
        if(offset <= 0 ){
            pp_trace("get an invalid remote %s",remote);
            return -1;
        }
    }

    std::string hostname(remote,offset);
    const char* port_str = remote+offset+1;
    int port = atoi(remote+offset+1);

    struct addrinfo hints;
    struct addrinfo *result, *rp;
    memset(&hints,0,sizeof(hints));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
    hints.ai_flags = 0;
    hints.ai_protocol = 0;          /* Any protocol */

    int s = 0;
    s = getaddrinfo(hostname.c_str() ,port_str , &hints, &result);
    if (s != 0) {
        pp_trace("getaddrinfo failed: %s\n", gai_strerror(s));
        return -1;
    }
    int sfd = -1;
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype,
                    rp->ai_protocol);

        // mark it as nonblock
        fcntl(sfd, F_SETFL, fcntl(sfd, F_GETFL, 0) | O_NONBLOCK);

        if (sfd == -1)
            continue;
        int ret = connect(sfd, rp->ai_addr, rp->ai_addrlen);
        if ( ret == 0)
        {
            break;
        }else if( ret == -1 ){
            if( errno == EALREADY || errno ==  EINPROGRESS )
                break; 
        }

        close(sfd);
        sfd = -1;
    }
    freeaddrinfo(result);
    
    return sfd;
}


int TransLayer::connect_unix_remote(const char* remote)
{
    int fd = -1;
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
        pp_trace("agent try to connect:(%s)",co_host);
        connect_remote(co_host);
        if(c_fd == -1)
        {
            return -1;
        }
    }
    
    int fd = c_fd;
    fd_set wfds,efds,rfds;

    FD_ZERO(&efds);
    FD_SET(fd,&efds);

    FD_ZERO(&wfds);
    FD_SET(fd,&wfds);

    FD_ZERO(&rfds);
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

const char* TransLayer::UNIX_SOCKET="unix:";
const char* TransLayer::TCP_SOCKET="tcp:";
