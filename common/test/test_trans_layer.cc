#include <gtest/gtest.h>
#include "TransLayer.h"
using namespace testing;

void output(const char* msg)
{

}

TEST(test, unix_layer)
{
    register_error_cb(output);
    TransLayer layer("/tmp/pinpoint.sock",10);
    layer.trans_layer_pool();
}