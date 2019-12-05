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


typedef struct trans_layer_t TransLayer;
typedef int  (*TransHandleCB)(TransLayer*);

class Chunks;

typedef struct trans_layer_t{
    int           c_fd;      // collector fd, use to send data;
    Chunks*        chunks; // A fixed size for span [0,MAX_VEC]
    TransHandleCB socket_read_cb;
    TransHandleCB socket_write_cb;
    char           in_buf[IN_MSG_BUF_SIZE];
}TransLayer;

typedef struct collector_agent_s{
    uint64_t start_time;
    char*   appid;
    char*   appname;
}CollectorAgentInfo;

enum E_ANGET_STATUS{
    E_OFFLINE= 0,
    E_TRACE_PASS,
    E_TRACE_BLOCK
};

enum E_SHM_OFFSET{
    UNIQUE_ID_OFFSET = 0,
    TRACE_LIMIT= 8,
};


void pp_trace(const char *format,...);


//typedef struct list{
//
//    void *value;
//
//    struct list* next;
//    struct list* pre;
//    int size;
//}Node;
//
//inline void init_list(Node* list)
//{
//    list->value = NULL;
//    list->pre = list->next = list;
//    list->size = 0;
//}
//
//inline int is_bottom(Node* list)
//{
//    return ( list->next == list ) ? (1):(0);
//}
//
//inline void* get_top(Node* list)
//{
//    return list->pre->value;
//}
//
//inline uint get_size(Node* list)
//{
//    return (uint)list->size;
//}
//
//inline int is_empty(Node* list)
//{
//    return list->size == 0;
//}
//
//inline Node* push_back(Node *list)
//{
//    if(is_empty(list))
//    {
//        list->size++;
//        return list;
//    }
//
//    Node* node = (Node*)malloc(sizeof(Node));
//    if(node){
//
//        Node* last = list->pre;
//
//        /// [last] ->  <- [node]
//        last->next = node;
//        node->pre  = last;
//
//        ///    [node] ->  <- [list]
//        list->pre  = node;
//        node->next = list;
//
//        node->value = NULL;
//        list->size++;
//    }
//    return node;
//}
//
//inline void* pop_back(Node*list)
//{
//
//    if(is_empty(list) ) // empty
//    {
//        return NULL;
//    }
//    else
//    {
//        void* value = NULL;
//
//        if(list->next == list)
//        {
//            value = list->value;
//            list->value = NULL;
//        }
//        else
//        {
//            Node* last = list->pre;
//            Node* pre  = last->pre;
//            list->pre = pre;
//            pre->next = list;
//            value = last->value;
//            free(last);
//        }
//
//        list->size--;
//        return value;
//    }
//}

#endif /* COMMON_H_ */
