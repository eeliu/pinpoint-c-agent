#include <gtest/gtest.h>
#include "SharedObj.h"
using namespace testing;
using namespace std;

TEST(test, shm)
{
    // string strMapName("ShareMemory");                // 内存映射对象名称
    // string strComData("This is common data!");        // 共享内存中的数据
    // LPVOID pBuffer;                                    // 共享内存指针

    // // 首先试图打开一个命名的内存映射文件对象
    // HANDLE hMap = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, TRUE, strMapName.c_str());
    // if (NULL == hMap)
    // {    // 打开失败，创建之
    //     hMap = ::CreateFileMapping(INVALID_HANDLE_VALUE,
    //                                NULL,
    //                                PAGE_READWRITE,
    //                                0,
    //                                4096,
    //                                strMapName.c_str());
    //     // 映射对象的一个视图，得到指向共享内存的指针，设置里面的数据
    //     pBuffer = ::MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    //     strcpy((char*)pBuffer, strComData.c_str());
    //     cout << "xx" << (char *)pBuffer << endl;
    // }
    // else
    // {    // 打开成功，映射对象的一个视图，得到指向共享内存的指针，显示出里面的数据
    //     pBuffer = ::MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    //     cout << "read" << (char *)pBuffer << endl;
    // }

    // getchar();            // 注意，进程关闭后，所有句柄自动关闭，所以要在这里暂停

    // // 解除文件映射，关闭内存映射文件对象句柄
    // ::UnmapViewOfFile(pBuffer);
    // ::CloseHandle(hMap);
}

TEST(test, create_and_use_the_shared_mem)
{
//create the tmpfile
    if(!checking_and_init()){
        EXPECT_TRUE(0)<<"failed to init shared_obj";
    }

    char* address = (char*)fetch_shared_obj_addr();

    strcpy(address,"hello pinpoint");
    detach_shared_obj();
// fork a child to check result
    pid_t pid =fork();
    if(pid == 0){
        if(!checking_and_init()){
               EXPECT_TRUE(0)<<"failed to init shared_obj";
        }
        char* address = (char*)fetch_shared_obj_addr();
        EXPECT_STREQ(address,"hello pinpoint");
        detach_shared_obj();
        exit(0);
    }
}
