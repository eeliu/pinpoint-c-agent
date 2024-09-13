////////////////////////////////////////////////////////////////////////////////
// Copyright 2020 NAVER Corp
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not
// use this file except in compliance with the License.  You may obtain a copy
// of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
// License for the specific language governing permissions and limitations under
// the License.
////////////////////////////////////////////////////////////////////////////////
/*
 * NodePool.cpp
 *
 *  Created on: Aug 19, 2020
 *      Author: eeliu
 */

#include "PoolManager.h"

#include "common.h"
#include <cassert>
#include <thread>

namespace PP {
namespace NodePool {
void PoolManager::FreeNodeTree(NodeID root) {
  if (root == E_INVALID_NODE || root == E_ROOT_NODE) {
    return;
  }
  for (NodeID next_id = ReturnNode(root); next_id != E_INVALID_NODE;) {
    next_id = ReturnNode(next_id);
  }
}
NodeID PoolManager::ReturnNode(NodeID id) {
  NodeID next = E_INVALID_NODE;
  for (int i = 0; i < 1000; i++) {
    // this node was in using: ref is not zero
    if (this->returnNode(id, next, false)) {
      return next;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  pp_trace("[🐛]Restore node failed:  [%d]; node restore forcefully", id);
  this->returnNode(id, next, true);
  return next;
}

bool PoolManager::returnNode(NodeID id, NodeID& next, bool force) {

  int32_t index = (int32_t)id - 1;

  if (this->indexInUsedVec(index) == false) {
    pp_trace("%d not alive !!!", id);
    return true;
  }

  // check refcount
  TraceNode& node = this->getUsedNode(id);

  if (node.IsNotReference() == false && force == false) {
    // DO NOT TOUCH this node
    return false;
  } else {
    this->usedNodeSet_[index] = false;
    this->_freeNodeList.push(index);
    next = node.next_;
    return true;
  }
}

TraceNode& PoolManager::getUsedNode(NodeID id) {

  if (id == E_ROOT_NODE) {
    throw std::out_of_range("id should not be 0");
  }

  int32_t index = int32_t(id) - 1;

  if (this->indexInUsedVec(index) == false) {
    std::string msg = "#";
    msg += std::to_string(id) + " is not alive";
    throw std::out_of_range(msg);
  }

  return this->nodeIndexVec[index / CELL_SIZE][index % CELL_SIZE];
}

TraceNode& PoolManager::getReadyNode() noexcept { // create a new node
  if (this->_freeNodeList.empty()) {
    this->expandOnce();
  }
  // as it holds a _lock, so no more _freeNodeList is empty
  int32_t index = this->_freeNodeList.top();
  this->_freeNodeList.pop();
  this->usedNodeSet_[index] = true;
  return this->nodeIndexVec[index / CELL_SIZE][index % CELL_SIZE].Reset(NodeID(index + 1));
}

void PoolManager::expandOnce() {
  // pp_trace("Node pool expanding self! Old size:%ld", this->nodeIndexVec.size() * CELL_SIZE);
  // append new nodes into nodeIndexVec
  this->nodeIndexVec.push_back(std::unique_ptr<TraceNode[]>(new TraceNode[CELL_SIZE]));
  // this->nodeIndexVec.push_back(std::make_unique<TraceNode[]>(CELL_SIZE));
  // append new bitflag into aliveNodeSet
  this->usedNodeSet_.insert(this->usedNodeSet_.end(), this->readyNodeSet_.begin(),
                            this->readyNodeSet_.end());
  for (int32_t id = this->maxId; id < (this->maxId + CELL_SIZE); id++) {
    this->_freeNodeList.push(id);
  }
  this->maxId += CELL_SIZE;
  // pp_trace("Node pool expanding is done! news size:%ld", this->nodeIndexVec.size() * CELL_SIZE);
  assert(this->nodeIndexVec.size() * CELL_SIZE == this->usedNodeSet_.size());
}

static Json::Value empty(Json::nullValue);

Value_Ptr PoolManager::EncodeTraceToJsonSpan(WrapperTraceNodePtr& node) {
  if (node->runUserOptionFunc() == false) {
    return nullptr;
  }

  if (!node->IsRootNode()) {
    pp_trace("current node:%d is not root", node->id_);
    return nullptr;
  }

  for (NodeID next = node->next_; next != E_INVALID_NODE;) {
    auto next_node = ReferNode(next);
    next_node->EndTrace();
    node->AppendAnnotation("event", *next_node->moveToSpan());
  }

  return node->moveToSpan();
}

} // namespace NodePool
} // namespace PP