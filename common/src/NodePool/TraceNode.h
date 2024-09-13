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
 * TraceNode.h
 *
 *  Created on: Aug 19, 2020
 *      Author: eeliu
 */

#ifndef COMMON_SRC_TRACENODE_H_
#define COMMON_SRC_TRACENODE_H_
#include "common.h"
#include "Context/ContextType.h"
#include "json/json.h"
#include "json/value.h"
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <map>
#include <mutex>
#include <stdarg.h>
#include <memory>
#include <utility>
#include <vector>
#include <inttypes.h>
#include <vector>
#include <assert.h>
namespace PP {
namespace NodePool {
namespace Json = AliasJson;
using Context::ContextType;
using Context::LongContextType;
using Context::StringContextType;
class WrapperTraceNodePtr;
const static int MAX_SUB_TRACE_NODES_LIMIT = 2048;
typedef std::shared_ptr<ContextType> ContextType_Ptr;
// protect value by unique, it's friendly to json::value append
using Value_Ptr = std::unique_ptr<Json::Value>;

using UserOptionalSettingFunc = std::function<bool()>;
using UserOptionalSettingFunc_Vec = std::vector<UserOptionalSettingFunc>;
class WrapperTraceNodePtr;
class TraceNode {

private:
  using Lock_t = std::mutex;
  using ContextMap_t = std::map<std::string, ContextType_Ptr>;
  struct RootTraceNodeExtra {
    Lock_t lock;
    NodeID last;
    NodeID next;
    // uint64_t fellows_limit;
    E_AGENT_STATUS status;
    std::atomic<int32_t> sequence;
    ContextMap_t context_map;
    RootTraceNodeExtra() {
      last = E_INVALID_NODE;
      next = E_INVALID_NODE;
      status = E_TRACE_PASS;
      sequence = 0;
    }
    ~RootTraceNodeExtra() { context_map.clear(); }
    int32_t createNewSequence() {
      sequence++;
      return sequence.load();
    }
  };

  std::unique_ptr<RootTraceNodeExtra> root_node_extra_ptr_ = {nullptr};

public:
  void UpgradeToRootNode() {
    root_node_extra_ptr_ = std::unique_ptr<RootTraceNodeExtra>(new RootTraceNodeExtra());
  }

  int32_t CreateNewSequence() {
    return root_node_extra_ptr_ ? root_node_extra_ptr_->createNewSequence() : -1;
  }

  NodeID GetLastNode() {
    return root_node_extra_ptr_ ? root_node_extra_ptr_->last : E_INVALID_NODE;
  }

  void SetLastNode(NodeID last) {
    if (root_node_extra_ptr_) {
      root_node_extra_ptr_->last = last;
    }
  }

  void SetStatus(E_AGENT_STATUS status) {
    if (root_node_extra_ptr_) {
      root_node_extra_ptr_->status = status;
    }
  }

  E_AGENT_STATUS GetStatus() {
    if (root_node_extra_ptr_) {
      return root_node_extra_ptr_->status;
    } else {
      return E_AGENT_INTERNAL_ERROR;
    }
  }

public:
  NodeID root_id_ = {E_INVALID_NODE};
  NodeID parent_id_ = {E_INVALID_NODE};
  NodeID id_ = {E_INVALID_NODE};
  NodeID next_ = {E_INVALID_NODE};

  int32_t depth_ = {1};
  int32_t sequence_ = {-1};
  uint64_t trace_start_time_ = {0};
  uint64_t parent_start_time_ = {0};

  // the naming from pinpoint protocol
  uint64_t expired_time = {0};
  bool set_exp_ = {false};

public:
  void StartTrace();

  void BindParentTrace(WrapperTraceNodePtr&);

  void EndTrace();

public:
  void AddSubTraceNode(WrapperTraceNodePtr& root, TraceNode& sub_node);
  inline bool IsRootNode() const { return this->root_id_ == id_; }

public:
  TraceNode() {}

  virtual ~TraceNode() = default;

  TraceNode& Reset(NodeID id);

  NodeID getId() const { return this->id_; }

  void getContext(const char* key, std::string& value) {
    ContextType_Ptr& ctx = this->context_map_.at(key);
    value = ctx->asStringValue();
  }

  void getContext(const char* key, long& value) {
    ContextType_Ptr& ctx = this->context_map_.at(key);
    value = ctx->asLongValue();
  }

  void setContext(const char* key, const char* buf) {
    ContextType_Ptr context(std::make_shared<StringContextType>(buf));
    this->context_map_[key] = context;
  }

  void setContext(const char* key, long l) {
    ContextType_Ptr context(std::make_shared<LongContextType>(l));
    this->context_map_[key] = context;
  }

public:
  TraceNode& operator=(const TraceNode&) = delete;
  TraceNode(const TraceNode&) = delete;

  bool operator==(TraceNode const& _node) const { return this->id_ == _node.id_; }

  bool operator!=(TraceNode const& _node) const { return this->id_ != _node.id_; }

private:
  struct Error {
    std::string message;
    std::string file_name;
    uint32_t line_nu;
    bool set;
  } error_;

public:
  void SetErrorInfo(std::string&& msg, std::string&& filename, uint32_t line) {
    error_.message = std::move(msg);
    error_.file_name = std::move(filename);
    error_.line_nu = line;
    error_.set = true;
  }

public:
  void AddAnnotation(const char* key, const char* v) { (*value_ptr_)[key] = v; }

  void AddAnnotation(const char* key, int v) { (*value_ptr_)[key] = v; }

  void AddAnnotation(const char* key, uint64_t v) { (*value_ptr_)[key] = v; }

  void AddAnnotation(const char* key, const Json::Value& v) {
    // std::lock_guard<std::mutex> _safe(this->mlock);
    (*value_ptr_)[key] = v;
  }

  void AppendAnnotation(const char* key, Json::Value&& v) {
    // std::lock_guard<std::mutex> _safe(this->mlock);
    (*value_ptr_)[key].append(v);
  }

  void AppendAnnotation(const char* key, Json::Value v) {
    // std::lock_guard<std::mutex> _safe(this->mlock);
    (*value_ptr_)[key].append(v);
  }

  void AppendAnnotation(const char* key, const char* v) {
    // std::lock_guard<std::mutex> _safe(this->mlock);
    (*value_ptr_)[key].append(v);
  }

public:
  void setNodeUserOption(const char* opt, va_list* args);
  bool runUserOptionFunc();

private:
  void parseUserOption(std::string key, std::string value);

private:
  friend WrapperTraceNodePtr;
  int addReference() {
    reference_count_++;
    return reference_count_.load();
  }

  int decReference() {
    reference_count_--;
    return reference_count_.load();
  }

public:
  bool IsNotReference() { return reference_count_.load() == 0; }

public:
  std::string ToString() {
    //  TODO
    char pbuf[1024] = {0};
    int len = snprintf(pbuf, 1024, "TODO ...");
    return std::string(pbuf, len);
  }

private:
  std::atomic<int> reference_count_;

public:
  Value_Ptr moveToSpan() { return std::move(value_ptr_); }

private:
  Value_Ptr value_ptr_;
  std::map<std::string, ContextType_Ptr> context_map_;
  UserOptionalSettingFunc_Vec user_optional_setting_func_;
};

class WrapperTraceNodePtr {
public:
  WrapperTraceNodePtr() = delete;
  WrapperTraceNodePtr(const WrapperTraceNodePtr& other) = delete;
  WrapperTraceNodePtr(WrapperTraceNodePtr& other) = delete;

  WrapperTraceNodePtr(WrapperTraceNodePtr&& other) : traceNode_(other.traceNode_) {
    traceNode_.addReference();
  }

  WrapperTraceNodePtr& operator=(const WrapperTraceNodePtr& other) = delete;

  WrapperTraceNodePtr(TraceNode& node) : traceNode_(node) { traceNode_.addReference(); }
  TraceNode* operator->() { return &traceNode_; }
  ~WrapperTraceNodePtr() { traceNode_.decReference(); }

private:
  TraceNode& traceNode_;
};
} // namespace NodePool
} // namespace PP
#endif /* COMMON_SRC_TRACENODE_H_ */
