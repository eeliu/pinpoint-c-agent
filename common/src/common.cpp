//
// Created by Bluse on 1/3/2020.
//

#include <stdarg.h>
#include <cstdio>
#include <processthreadsapi.h>
#include "common.h"

typedef void(*log_error_cb)(const char*);
typedef struct Logger_s{
    char logBuffer[4096];
    log_error_cb error_cb;
}Logger_t;

static Logger_t logger;

void pp_trace(const char *format,...)
{
    int n = snprintf(&logger.logBuffer[0],LOG_SIZE,"[%ld] ",GetCurrentProcessId());
    va_list ap;
    va_start(ap, format);
    vsnprintf(&logger.logBuffer[n], LOG_SIZE -n ,format, ap);
    va_end(ap);

    if (logger.error_cb){
        logger.error_cb(logger.logBuffer);
    }else{
        fprintf(stderr,"%s\n",logger.logBuffer);
    }
}