#include "common.h"
#include "json/value.h"
#include <gtest/gtest.h>
#include "header.h"
#define COMMON_DEBUG
#include "NodePool/PoolManager.h"
#include <condition_variable>
#include <thread>
#include <chrono>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>

using PP::NodePool::PoolManager;
using PP::NodePool::TraceNode;
using PP::NodePool::WrapperTraceNodePtr;
using namespace testing;
namespace Json = AliasJson;
PoolManager nodePool_;
std::mutex cv_m;
std::condition_variable cv;
NodeID rootId = E_ROOT_NODE;

// note: as it known, there may leak some nodes
void func() {
  std::unique_lock<std::mutex> lk(cv_m);
  cv.wait(lk);
  pinpoint_add_clues(rootId, "xxxx", "bbbbbbss", E_LOC_CURRENT);
  pinpoint_add_clue(rootId, "xxx", "bbbbbb", E_LOC_CURRENT);
  for (int i = 0; i < 100; ++i) {
    rootId = pinpoint_start_trace(rootId);
    pinpoint_set_context_key(rootId, "xxxx", "bbbbbb");
    std::this_thread::yield();
    char buf[1024] = {0};
    auto len = pinpoint_get_context_key(rootId, "xxxx", buf, 1024);
    assert(len > 0);
    std::cout << rootId << "read value:" << buf << std::endl;
    pinpoint_add_clues(rootId, "xxxx", "bbbbbbss", E_LOC_CURRENT);
    std::this_thread::yield();
    pinpoint_add_clue(rootId, "xxx", "bbbbbb", E_LOC_CURRENT);
    std::this_thread::yield();
    rootId = pinpoint_end_trace(rootId);
    std::this_thread::yield();
  }
  pinpoint_add_clues(rootId, "xxxx", "bbbbbbss", E_LOC_CURRENT);
  pinpoint_add_clue(rootId, "xxx", "bbbbbb", E_LOC_CURRENT);
}

TEST(node, multipleThread) {
  // no crash, works fine
  NodeID root = pinpoint_start_trace(E_ROOT_NODE);

  std::vector<std::thread> threads;

  for (int i = 0; i < 10; i++) {
    threads.push_back(std::thread(func));
  }

  sleep(2);
  cv.notify_all();

  for (int i = 0; i < 10; i++) {
    threads[i].join();
  }
  pinpoint_end_trace(root);
  pinpoint_end_trace(root);
  // EXPECT_TRUE(PoolManager::getInstance().NoNodeLeak());
}

void test_opt(TraceNode& node, const char* opt, ...) {
  va_list args;
  va_start(args, opt);
  node.setNodeUserOption(opt, &args);
  va_end(args);
}

TEST(node, opt) {

  TraceNode& node = nodePool_.GetNode();

  test_opt(node, "TraceMinTimeMs:23", "TraceOnlyException", nullptr);

  node.expired_time = 22;
  EXPECT_FALSE(node.runUserOptionFunc());
  node.expired_time = 23;
  EXPECT_TRUE(node.runUserOptionFunc());

  node.expired_time = 0;
  node.set_exp_ = true;
  EXPECT_TRUE(node.runUserOptionFunc());
  node.set_exp_ = false;
  EXPECT_FALSE(node.runUserOptionFunc());

  nodePool_.ReturnNode(node.id_);
  EXPECT_TRUE(nodePool_.NoNodeLeak());
}

TEST(node, leak_node) {
  auto count = nodePool_.freeNodesCount();
  NodeID root, child1, child2;
  root = pinpoint_start_trace(E_ROOT_NODE);
  child1 = pinpoint_start_trace(root);
  child2 = pinpoint_start_trace(child1);
  // root goes first
  pinpoint_end_trace(root);

  pinpoint_end_trace(child1);
  pinpoint_end_trace(child2);

  EXPECT_EQ(count, nodePool_.freeNodesCount());
  show_status();
}

TEST(node, tons_of_nodes_01) {
  pinpoint_set_agent("tcp:127.0.0.1:9999", 7000, -1, 7000);
  // auto count = usedNode();
  NodeID root = pinpoint_start_trace(E_ROOT_NODE);
  for (int i = 0; i < 1000; i++) {
    NodeID child1 = pinpoint_start_trace(root);
    pinpoint_end_trace(child1);
  }
  pinpoint_end_trace(root);

  // EXPECT_EQ(count, usedNode()); //);

  root = pinpoint_start_trace(E_ROOT_NODE);
  NodeID child1 = root;
  for (int i = 0; i < 1000; i++) {
    NodeID child = pinpoint_start_trace(child1);
    change_trace_status(root, E_OFFLINE);
    pinpoint_end_trace(child);
    child1 = child;
  }
  change_trace_status(root, E_TRACE_BLOCK);
  pinpoint_end_trace(root);
}
// HACK here reproduce a bug in win11, not found any solution
// unknown file: error: SEH exception with code 0xc00000fd thrown in the test body.
// static int loop = 0;
// int test_loop(Json::Value& value, int i) {
//   loop++;
//   if (i == 0) {
//     return 0;
//   }
//   printf("%d \n", loop);
//   if (i % 2) {
//     Json::Value v;
//     test_loop(v, i - 1);
//     value["calls"].append(v);
//   } else {
//     test_loop(value, i - 1);
//   }

//   if (i % 3) {
//     value["id"] = 2;
//   }
//   return 0;
// }

// TEST(node, test_json) {
//   // std::srand(std::time(nullptr));
//   // Json::Value pvalue;
//   // // for (int i = 0; i < 2000; i++) {
//   // test_loop(pvalue, 1000);
//   // // }
//   // printf("%s", pvalue.toStyledString().c_str());
//   char* p = (char*)10;
//   *p = 10;
// }
// HACK: window crash when nodes reached 1k
// solution: use pspanchunk protocol
TEST(node, tons_of_nodes_1k) {
  NodeID root = pinpoint_start_trace(E_ROOT_NODE);
  NodeID next = root;
  for (int i = 0; i < 1000; i++) {
    NodeID child = pinpoint_start_trace(next);
    pinpoint_add_clue(child, "id", std::to_string(child).c_str(), E_LOC_CURRENT);
    pinpoint_end_trace(child);

    if (i % 2 == 0) {
      next = child;
    }
  }
  pinpoint_end_trace(root);
  show_status();
}

TEST(node, tons_of_nodes_leak) {
  NodeID root, child_1, child_2;
  root = pinpoint_start_trace(E_ROOT_NODE);

  child_1 = pinpoint_start_trace(root);
  child_2 = pinpoint_start_trace(child_1);

  pinpoint_end_trace(child_2);

  pinpoint_end_trace(root);
  pinpoint_end_trace(child_1);
}

TEST(node, tons_of_nodes_free_all) {

  NodeID root, child_1, child_2;
  root = pinpoint_start_trace(E_ROOT_NODE);

  child_1 = pinpoint_start_trace(root);
  child_2 = pinpoint_start_trace(child_1);
  pinpoint_end_trace(child_2);

  pinpoint_end_trace(root);
  pinpoint_end_trace(child_1);
}
//./bin/TestCommon --gtest_filter=node.free_when_add
TEST(node, free_when_add) {
  auto getUsedNode = [&]() { return nodePool_.totalNodesCount() - nodePool_.freeNodesCount(); };
  auto usedNode = getUsedNode();
  NodeID root;
  auto make_it_busy = [&]() {
    auto& w_root = nodePool_.GetNode();
    root = w_root.id_;
    auto& w_child = nodePool_.GetNode();

    WrapperTraceNodePtr parent_ptr = w_root;
    w_child.BindParentTrace(parent_ptr);

    w_child.AddAnnotation("E_ROOT_NODE", 234);
    std::this_thread::sleep_for(std::chrono::seconds(2));
  };
  std::thread t(make_it_busy);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  auto start = get_unix_time_ms();
  nodePool_.FreeNodeTree(root);
  nodePool_.FreeNodeTree(E_INVALID_NODE);
  nodePool_.FreeNodeTree(E_ROOT_NODE);
  auto end = get_unix_time_ms() - start;
  printf("it takes: %ld to free ", (long)end);
  t.join();
  EXPECT_EQ(usedNode, getUsedNode());
  EXPECT_TRUE(end >= 100);
}

//./bin/TestCommon --gtest_filter=node.orphan_node
TEST(node, orphan_node) {
  NodeID root, child_1, orphan;
  root = pinpoint_start_trace(E_ROOT_NODE);
  child_1 = pinpoint_start_trace(root);
  orphan = pinpoint_start_trace(child_1);
  pinpoint_end_trace(child_1);
  pinpoint_end_trace(root);

  root = pinpoint_start_trace(E_ROOT_NODE);
  child_1 = pinpoint_start_trace(E_ROOT_NODE);
  debug_nodeid(orphan);
  pinpoint_end_trace(orphan);
  debug_nodeid(orphan);
  pinpoint_end_trace(child_1);
  pinpoint_end_trace(root);
}
//./bin/TestCommon --gtest_filter=node.orphan_node_01
TEST(node, orphan_node_01) {
  pinpoint_set_agent("tcp:127.0.0.1:9999", 7000, -1, 7000);
  NodeID root, child_1, orphan;
  root = pinpoint_start_trace(E_ROOT_NODE);
  child_1 = pinpoint_start_trace(root);
  pinpoint_end_trace(root);
  orphan = pinpoint_start_trace(child_1);

  pinpoint_end_trace(child_1);
  pinpoint_end_trace(orphan);
}
//./bin/TestCommon --gtest_filter=node.orphan_root_parent_end
TEST(node, orphan_parent_root_end) {
  pinpoint_set_agent("tcp:127.0.0.1:9999", 7000, -1, 7000);
  NodeID root, child_1, orphan;
  root = pinpoint_start_trace(E_ROOT_NODE);
  child_1 = pinpoint_start_trace(root);
  orphan = pinpoint_start_trace(child_1);
  pinpoint_end_trace(child_1);
  pinpoint_end_trace(root);

  pinpoint_end_trace(orphan);
}

TEST(node, orphan_root_parent_end) {
  pinpoint_set_agent("tcp:127.0.0.1:9999", 7000, -1, 7000);
  NodeID root, child_1, orphan;
  root = pinpoint_start_trace(E_ROOT_NODE);
  child_1 = pinpoint_start_trace(root);
  orphan = pinpoint_start_trace(child_1);
  pinpoint_end_trace(root);
  pinpoint_end_trace(child_1);

  pinpoint_end_trace(orphan);
}
// ./bin/TestCommon --gtest_filter=node.end_trace_in_mt
TEST(node, end_trace_in_mt) {
  pinpoint_set_agent("tcp:127.0.0.1:9999", 7000, -1, 7000);
  NodeID root = pinpoint_start_trace(E_ROOT_NODE);
  NodeID next = root;
  // limit size 100; due to
  // https://github.com/pinpoint-apm/pinpoint-c-agent/runs/6806024797?check_suite_focus=true bus
  // error under macos
  for (int i = 0; i < 100; i++) {
    next = pinpoint_start_trace(next);
    pinpoint_end_trace(next);
  }

  std::mutex mtx;
  std::condition_variable cv;

  auto thread_func = [&]() {
    std::unique_lock<std::mutex> lck(mtx);
    cv.wait(lck);
    pp_trace("%lu ", std::this_thread::get_id());
    pinpoint_end_trace(root);
  };
  std::vector<std::thread> threads;
  for (int i = 0; i < 10; i++) {
    // std::thread t(thread_func);
    threads.emplace_back(thread_func);
  }
  // wait for all threads running
  sleep(2);
  cv.notify_all();
  for (auto& thread : threads)
    thread.join();
}
// ./bin/TestCommon --gtest_filter=node.max_sub_nodes
TEST(node, max_sub_nodes) {
  pinpoint_set_agent("tcp:127.0.0.1:9999", 7000, -1, 7000);
  NodeID root = pinpoint_start_trace(E_ROOT_NODE);
  while (true) {
    NodeID next = pinpoint_start_trace(root);
    if (next == E_INVALID_NODE) {
      break;
    }
    pinpoint_end_trace(next);
  }
  pinpoint_end_trace(root);
}