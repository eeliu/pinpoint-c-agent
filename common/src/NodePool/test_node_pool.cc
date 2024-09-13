#include "NodePool/TraceNode.h"
#include "PoolManager.h"
#include "common.h"
#include "header.h"
#include <thread>
#include <gtest/gtest.h>

using namespace testing;
using PP::NodePool::PoolManager;
using PP::NodePool::ThreadSafePoolManager;
using PP::NodePool::TraceNode;

TEST(poolManger, get_and_give_back) {
  PoolManager pool;
  // new
  TraceNode& _node = pool.GetNode();
  void* p = &_node;
  NodeID id = _node.getId();
  NodeID next;
  // give back
  next = pool.ReturnNode(id);
  EXPECT_EQ(next, E_INVALID_NODE);
  TraceNode& _node_01 = pool.GetNode();

  TraceNode& new_node = pool.GetNode();
  NodeID new_id = new_node.id_;

  auto ref_node = pool.ReferNode(new_id);
  EXPECT_EQ(ref_node->id_, new_node.id_);

  next = pool.ReturnNode(new_id);
  EXPECT_THROW(pool.ReferNode(new_id), std::out_of_range);

  // ref current
  auto ref_new_node = pool.ReferNode(_node_01.getId());

  EXPECT_EQ(p, &_node);
  // EXPECT_EQ(ref_new_node, _node);
  // reuse the same id
  EXPECT_EQ(id, _node.getId());
  EXPECT_THROW(pool.ReferNode(NodeID(100)), std::out_of_range);
  EXPECT_THROW(pool.ReferNode(NodeID(10000)), std::out_of_range);
}

static ThreadSafePoolManager thread_safe_pool;

void test_node_pool(bool& result) {
  NodeID it = E_INVALID_NODE;
  for (int i = 0; i < 1000; i++) {
    TraceNode& _node = thread_safe_pool.GetNode();
    usleep(1000);
    if (_node.getId() == it) {
      result = false;
      return;
    }
    thread_safe_pool.ReturnNode(_node.id_);
  }
  result = true;
}

TEST(poolManger, get_and_give_back_tls) {
  bool rth1, rth2;
  std::thread t1(test_node_pool, std::ref(rth1));
  std::thread t2(test_node_pool, std::ref(rth2));

  t1.join();
  t2.join();
  EXPECT_TRUE(rth1);
  EXPECT_TRUE(rth2);
}
