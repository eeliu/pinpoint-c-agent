//
// Created by Bluse on 12/8/2019.
//

#include "SharedObj.h"
#include <string.h>
#include <assert.h>

#ifdef __linux__
#define SHARE_OBJ_NAME "/dev/shm/pinpoint-php.shm"

#elif _WIN32
#include <windows.h>

typedef struct shared_object_s{
    HANDLE hMap;
    LPVOID region;
    int length;
    const char *address;
}SharedObject_T;

static SharedObject_T object;

#define SHARE_OBJ_NAME "pinpoint-php.shm"


bool pre_init_shared_object()
{
    memset(&object,0, sizeof(SharedObject_T));
    object.address = SHARE_OBJ_NAME;
    return true;
}

bool init_shared_obj()
{
    assert(object.address);

    LPVOID pBuffer = NULL;
    HANDLE hMap = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, TRUE, object.address);
    if(hMap == NULL)
    {
        hMap = ::CreateFileMapping(INVALID_HANDLE_VALUE,
                                   NULL,PAGE_READWRITE, 0,4096,object.address);
        if(hMap == NULL)
        {
            goto FAILED;
        }
        pBuffer = ::MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
        if(pBuffer == NULL){
            goto FAILED;
        }
        object.region = pBuffer;
    }else{
        object.hMap =hMap;
        pBuffer = ::MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
        if(pBuffer == NULL){
            goto FAILED;
        }
        object.region = pBuffer;
    };

    return true;
FAILED:
    if(hMap){
        ::CloseHandle(hMap);
    }

    //::UnmapViewOfFile(pBuffer);

    pp_trace("init_shared_obj failed with [%ld]",GetLastError());
    return false;
}


void* fetch_shared_address()
{
    return object.region;
}


#else


#error "not support platform"
#endif




