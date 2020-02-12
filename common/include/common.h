/*******************************************************************************
 * Copyright 2019 NAVER Corp
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License.  You may obtain a copy
 * of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations under
 * the License.
 ******************************************************************************/
#ifndef COMMON_H_
#define COMMON_H_

#include <stdlib.h>
#include <stdint.h>

#ifdef __linux__
#define likely(x)        __builtin_expect(!!(x), 1)
#define unlikely(x)      __builtin_expect(!!(x), 0)
#elif _WIN32
#else

#endif

//fix #129
#ifndef uint
#define uint unsigned int
#endif

#define MAX_VEC 512
#define LOG_SIZE 4096
#define IN_MSG_BUF_SIZE 4096
#define NAMING_SIZE 128
#define PHP 1500

typedef enum{
    RESPONSE_AGENT_INFO = 0,
    REQ_UPDATE_SPAN = 1
}MSG_TYPE;

#pragma pack (1)
typedef  struct {
    uint type;
    uint length;
}Header;
#pragma pack ()

typedef struct collector_agent_s{
    uint64_t start_time;
    char*   appid;
    char*   appname;
}CollectorAgentInfo;

#define LOG_SIZE 4096

typedef struct pp_agent_s{
    const char* co_host; // tcp:ip:port should support dns
    uint  timeout_ms;
    int   trace_limit;
}PPAgentT;

typedef void(*log_error_cb)(const char*);
void register_error_cb(log_error_cb error_cb);
void pp_trace(const char *format,...);

/**
 *pinpoint_start_trace
 *pinpoint_end_trace
 *pinpoint_add_clue
 *pinpoint_add_clues
 *pinpoint_unique_id
 *pinpoint_tracelimit $timestamp
 *pinpoint_drop_trace
 *pinpoint_app_name
 *pinpoint_app_id
 *pinpoint_start_time
 */

int32_t pinpoint_start_trace();
int32_t pinpoint_end_trace();
void pinpoint_add_clues(const  char* key,const  char* value);
void pinpoint_add_clue(const  char* key,const  char* value);
bool check_tracelimit(int64_t timestamp);
int64_t generate_unique_id();
void pinpoint_drop_trace();
void test_trace();
#endif /* COMMON_H_ */
