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
 * TraceNode.cpp
 *
 *  Created on: Aug 19, 2020
 *      Author: eeliu
 */

#include "TraceNode.h"
#include "common.h"
#include "header.h"
#include "json/value.h"
#include <cstdint>
#include <memory>

namespace PP {
namespace NodePool {

TraceNode& TraceNode::Reset(NodeID id) {
  id_ = id;

  root_id_ = parent_id_ = next_ = E_INVALID_NODE;

  parent_start_time_ = 0;
  trace_start_time_ = 0;

  reference_count_ = 0;
  root_node_extra_ptr_ = nullptr;

  if (!this->user_optional_setting_func_.empty()) {
    this->user_optional_setting_func_.clear();
  }

  value_ptr_ = std::unique_ptr<Json::Value>(new Json::Value());

  return *this;
}

void TraceNode::StartTrace() { trace_start_time_ = get_unix_time_ms(); }

void TraceNode::EndTrace() {
  uint64_t end_time = get_unix_time_ms();
  this->expired_time += (end_time - this->trace_start_time_);

  this->AddAnnotation(":E", this->expired_time);
  this->AddAnnotation(":S", this->trace_start_time_ - this->parent_start_time_);

  if (this->set_exp_) {
    this->AddAnnotation("EA", 1);
  }
}

void TraceNode::BindParentTrace(WrapperTraceNodePtr& parent) {
  parent_start_time_ = parent->trace_start_time_;
  parent_id_ = parent->id_;
  depth_ = parent->depth_ + 1;
  root_id_ = parent->root_id_;
}

void TraceNode::parseUserOption(std::string key, std::string value) {
  pp_trace(" [%d] add opt: key:%s value:%s", id_, key.c_str(), value.c_str());
  if (key == "TraceMinTimeMs") {
    int64_t min = std::stoll(value);
    auto cb = [=]() -> bool {
      pp_trace("checkOpt:  [%d] TraceMinTimeMs:%" PRIu64 " cumulative_time:%" PRIu64 "", this->id_,
               min, this->expired_time);
      if ((int64_t)this->expired_time >= min) {
        return true;
      } else {
        return false;
      }
    };

    this->user_optional_setting_func_.push_back(cb);
  } else if (key == "TraceOnlyException") {
    auto cb = [=]() -> bool { return this->set_exp_; };

    this->user_optional_setting_func_.push_back(cb);
  }
}

bool TraceNode::runUserOptionFunc() {
  bool ret = true;
  for (auto& cb : this->user_optional_setting_func_) {
    if ((ret = cb()) == true)
      return ret;
  }
  return ret;
}

void TraceNode::setNodeUserOption(const char* opt, va_list* args) {
  const char* var = opt;

  while (var != nullptr) {
    const char* delimit = strchr(var, ':');
    if (delimit == nullptr) {
      // key:value format
      this->parseUserOption(std::string(var), "");
    } else {
      // command format
      std::string key(var, delimit - var);
      std::string value(delimit + 1);
      this->parseUserOption(key, value);
    }
    var = va_arg(*args, const char*);
  }
}
} // namespace NodePool
} // namespace  PP