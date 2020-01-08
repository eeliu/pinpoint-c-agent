#include <gtest/gtest.h>
#include "common.h"

int main(int argc, char **argv) {
    register_error_cb(NULL);
    ::testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS();
}