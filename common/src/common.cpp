//
// Created by Bluse on 1/3/2020.
//

#include <stdarg.h>
#include <cstdio>
#include "common.h"

static log_error_cb _error_cb;

#ifdef __linux__
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>


#define getOSPid getpid
pthread_key_t key;
pthread_once_t init_done = PTHREAD_ONCE_INIT;
void thread_init(void)
{
   pthread_key_create(&key, free);
}

void register_error_cb(log_error_cb error_cb)
{
    pthread_once(&init_done,thread_init);
    _error_cb  = error_cb;
}

#elif _WIN32
#include <processthreadsapi.h>
#define getOSPid GetCurrentProcessId
#else
#error "not support"
#endif

void pp_trace(const char *format,...)
{
    void *value = pthread_getspecific(key);
    if( unlikely( value == NULL )){
        value = malloc(LOG_SIZE);
        pthread_setspecific(key,value);
    }

    char * pstart = (char*)value;
    int n = snprintf(pstart ,LOG_SIZE,"[%d] ",getOSPid());
    va_list ap;
    va_start(ap, format);
    vsnprintf(pstart+n, LOG_SIZE -n - 1 ,format, ap);
    va_end(ap);

    if (_error_cb){
        _error_cb(pstart);
    }else{
        fprintf(stderr,"%s\n",pstart);
    }
}
