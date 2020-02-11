//
// Created by Bluse on 12/8/2019.
//

#ifndef COMMON_SHAREDOBJ_H
#define COMMON_SHAREDOBJ_H

#include "common.h"

#ifdef __cplusplus 
extern "C"{
#endif

// bool pre_init_shared_object();
// bool init_shared_obj();
void* fetch_shared_obj_addr();
int  fetch_shared_obj_length();
bool checking_and_init();
void detach_shared_obj();

#ifdef __cplusplus
}
#endif

#endif //COMMON_SHAREDOBJ_H
