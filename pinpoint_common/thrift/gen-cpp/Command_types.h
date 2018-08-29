/**
 * Autogenerated by Thrift Compiler (0.11.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef Command_TYPES_H
#define Command_TYPES_H

#include <iosfwd>

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/TBase.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include <thrift/stdcxx.h>




struct TThreadDumpType {
  enum type {
    TARGET = 0,
    PENDING = 1
  };
};

extern const std::map<int, const char*> _TThreadDumpType_VALUES_TO_NAMES;

std::ostream& operator<<(std::ostream& out, const TThreadDumpType::type& val);

struct TThreadState {
  enum type {
    NEW = 0,
    RUNNABLE = 1,
    BLOCKED = 2,
    WAITING = 3,
    TIMED_WAITING = 4,
    TERMINATED = 5
  };
};

extern const std::map<int, const char*> _TThreadState_VALUES_TO_NAMES;

std::ostream& operator<<(std::ostream& out, const TThreadState::type& val);

struct TRouteResult {
  enum type {
    OK = 0,
    BAD_REQUEST = 200,
    EMPTY_REQUEST = 201,
    NOT_SUPPORTED_REQUEST = 202,
    BAD_RESPONSE = 210,
    EMPTY_RESPONSE = 211,
    NOT_SUPPORTED_RESPONSE = 212,
    TIMEOUT = 220,
    NOT_FOUND = 230,
    NOT_ACCEPTABLE = 240,
    NOT_SUPPORTED_SERVICE = 241,
    UNKNOWN = -1
  };
};

extern const std::map<int, const char*> _TRouteResult_VALUES_TO_NAMES;

std::ostream& operator<<(std::ostream& out, const TRouteResult::type& val);

class TCommandThreadDump;

class TMonitorInfo;

class TThreadDump;

class TCommandThreadDumpResponse;

class TCmdActiveThreadCount;

class TCmdActiveThreadCountRes;

class TActiveThreadDump;

class TCmdActiveThreadDump;

class TCmdActiveThreadDumpRes;

class TCommandEcho;

class TCommandTransfer;

class TCommandTransferResponse;

typedef struct _TCommandThreadDump__isset {
  _TCommandThreadDump__isset() : type(true), name(false), pendingTimeMillis(false) {}
  bool type :1;
  bool name :1;
  bool pendingTimeMillis :1;
} _TCommandThreadDump__isset;

class TCommandThreadDump : public virtual ::apache::thrift::TBase {
 public:

  TCommandThreadDump(const TCommandThreadDump&);
  TCommandThreadDump& operator=(const TCommandThreadDump&);
  TCommandThreadDump() : type((TThreadDumpType::type)0), name(), pendingTimeMillis(0) {
    type = (TThreadDumpType::type)0;

  }

  virtual ~TCommandThreadDump() throw();
  TThreadDumpType::type type;
  std::string name;
  int64_t pendingTimeMillis;

  _TCommandThreadDump__isset __isset;

  void __set_type(const TThreadDumpType::type val);

  void __set_name(const std::string& val);

  void __set_pendingTimeMillis(const int64_t val);

  bool operator == (const TCommandThreadDump & rhs) const
  {
    if (!(type == rhs.type))
      return false;
    if (__isset.name != rhs.__isset.name)
      return false;
    else if (__isset.name && !(name == rhs.name))
      return false;
    if (__isset.pendingTimeMillis != rhs.__isset.pendingTimeMillis)
      return false;
    else if (__isset.pendingTimeMillis && !(pendingTimeMillis == rhs.pendingTimeMillis))
      return false;
    return true;
  }
  bool operator != (const TCommandThreadDump &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const TCommandThreadDump & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(TCommandThreadDump &a, TCommandThreadDump &b);

std::ostream& operator<<(std::ostream& out, const TCommandThreadDump& obj);

typedef struct _TMonitorInfo__isset {
  _TMonitorInfo__isset() : stackDepth(false), stackFrame(false) {}
  bool stackDepth :1;
  bool stackFrame :1;
} _TMonitorInfo__isset;

class TMonitorInfo : public virtual ::apache::thrift::TBase {
 public:

  TMonitorInfo(const TMonitorInfo&);
  TMonitorInfo& operator=(const TMonitorInfo&);
  TMonitorInfo() : stackDepth(0), stackFrame() {
  }

  virtual ~TMonitorInfo() throw();
  int32_t stackDepth;
  std::string stackFrame;

  _TMonitorInfo__isset __isset;

  void __set_stackDepth(const int32_t val);

  void __set_stackFrame(const std::string& val);

  bool operator == (const TMonitorInfo & rhs) const
  {
    if (!(stackDepth == rhs.stackDepth))
      return false;
    if (!(stackFrame == rhs.stackFrame))
      return false;
    return true;
  }
  bool operator != (const TMonitorInfo &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const TMonitorInfo & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(TMonitorInfo &a, TMonitorInfo &b);

std::ostream& operator<<(std::ostream& out, const TMonitorInfo& obj);

typedef struct _TThreadDump__isset {
  _TThreadDump__isset() : threadName(false), threadId(false), blockedTime(false), blockedCount(false), waitedTime(false), waitedCount(false), lockName(false), lockOwnerId(false), lockOwnerName(false), inNative(false), suspended(false), threadState(false), stackTrace(false), lockedMonitors(false), lockedSynchronizers(false) {}
  bool threadName :1;
  bool threadId :1;
  bool blockedTime :1;
  bool blockedCount :1;
  bool waitedTime :1;
  bool waitedCount :1;
  bool lockName :1;
  bool lockOwnerId :1;
  bool lockOwnerName :1;
  bool inNative :1;
  bool suspended :1;
  bool threadState :1;
  bool stackTrace :1;
  bool lockedMonitors :1;
  bool lockedSynchronizers :1;
} _TThreadDump__isset;

class TThreadDump : public virtual ::apache::thrift::TBase {
 public:

  TThreadDump(const TThreadDump&);
  TThreadDump& operator=(const TThreadDump&);
  TThreadDump() : threadName(), threadId(0), blockedTime(0), blockedCount(0), waitedTime(0), waitedCount(0), lockName(), lockOwnerId(0), lockOwnerName(), inNative(0), suspended(0), threadState((TThreadState::type)0) {
  }

  virtual ~TThreadDump() throw();
  std::string threadName;
  int64_t threadId;
  int64_t blockedTime;
  int64_t blockedCount;
  int64_t waitedTime;
  int64_t waitedCount;
  std::string lockName;
  int64_t lockOwnerId;
  std::string lockOwnerName;
  bool inNative;
  bool suspended;
  TThreadState::type threadState;
  std::vector<std::string>  stackTrace;
  std::vector<TMonitorInfo>  lockedMonitors;
  std::vector<std::string>  lockedSynchronizers;

  _TThreadDump__isset __isset;

  void __set_threadName(const std::string& val);

  void __set_threadId(const int64_t val);

  void __set_blockedTime(const int64_t val);

  void __set_blockedCount(const int64_t val);

  void __set_waitedTime(const int64_t val);

  void __set_waitedCount(const int64_t val);

  void __set_lockName(const std::string& val);

  void __set_lockOwnerId(const int64_t val);

  void __set_lockOwnerName(const std::string& val);

  void __set_inNative(const bool val);

  void __set_suspended(const bool val);

  void __set_threadState(const TThreadState::type val);

  void __set_stackTrace(const std::vector<std::string> & val);

  void __set_lockedMonitors(const std::vector<TMonitorInfo> & val);

  void __set_lockedSynchronizers(const std::vector<std::string> & val);

  bool operator == (const TThreadDump & rhs) const
  {
    if (!(threadName == rhs.threadName))
      return false;
    if (!(threadId == rhs.threadId))
      return false;
    if (!(blockedTime == rhs.blockedTime))
      return false;
    if (!(blockedCount == rhs.blockedCount))
      return false;
    if (!(waitedTime == rhs.waitedTime))
      return false;
    if (!(waitedCount == rhs.waitedCount))
      return false;
    if (!(lockName == rhs.lockName))
      return false;
    if (!(lockOwnerId == rhs.lockOwnerId))
      return false;
    if (!(lockOwnerName == rhs.lockOwnerName))
      return false;
    if (!(inNative == rhs.inNative))
      return false;
    if (!(suspended == rhs.suspended))
      return false;
    if (!(threadState == rhs.threadState))
      return false;
    if (!(stackTrace == rhs.stackTrace))
      return false;
    if (!(lockedMonitors == rhs.lockedMonitors))
      return false;
    if (!(lockedSynchronizers == rhs.lockedSynchronizers))
      return false;
    return true;
  }
  bool operator != (const TThreadDump &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const TThreadDump & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(TThreadDump &a, TThreadDump &b);

std::ostream& operator<<(std::ostream& out, const TThreadDump& obj);

typedef struct _TCommandThreadDumpResponse__isset {
  _TCommandThreadDumpResponse__isset() : threadDumps(false) {}
  bool threadDumps :1;
} _TCommandThreadDumpResponse__isset;

class TCommandThreadDumpResponse : public virtual ::apache::thrift::TBase {
 public:

  TCommandThreadDumpResponse(const TCommandThreadDumpResponse&);
  TCommandThreadDumpResponse& operator=(const TCommandThreadDumpResponse&);
  TCommandThreadDumpResponse() {
  }

  virtual ~TCommandThreadDumpResponse() throw();
  std::vector<TThreadDump>  threadDumps;

  _TCommandThreadDumpResponse__isset __isset;

  void __set_threadDumps(const std::vector<TThreadDump> & val);

  bool operator == (const TCommandThreadDumpResponse & rhs) const
  {
    if (!(threadDumps == rhs.threadDumps))
      return false;
    return true;
  }
  bool operator != (const TCommandThreadDumpResponse &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const TCommandThreadDumpResponse & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(TCommandThreadDumpResponse &a, TCommandThreadDumpResponse &b);

std::ostream& operator<<(std::ostream& out, const TCommandThreadDumpResponse& obj);


class TCmdActiveThreadCount : public virtual ::apache::thrift::TBase {
 public:

  TCmdActiveThreadCount(const TCmdActiveThreadCount&);
  TCmdActiveThreadCount& operator=(const TCmdActiveThreadCount&);
  TCmdActiveThreadCount() {
  }

  virtual ~TCmdActiveThreadCount() throw();

  bool operator == (const TCmdActiveThreadCount & /* rhs */) const
  {
    return true;
  }
  bool operator != (const TCmdActiveThreadCount &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const TCmdActiveThreadCount & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(TCmdActiveThreadCount &a, TCmdActiveThreadCount &b);

std::ostream& operator<<(std::ostream& out, const TCmdActiveThreadCount& obj);

typedef struct _TCmdActiveThreadCountRes__isset {
  _TCmdActiveThreadCountRes__isset() : histogramSchemaType(false), activeThreadCount(false), timeStamp(false) {}
  bool histogramSchemaType :1;
  bool activeThreadCount :1;
  bool timeStamp :1;
} _TCmdActiveThreadCountRes__isset;

class TCmdActiveThreadCountRes : public virtual ::apache::thrift::TBase {
 public:

  TCmdActiveThreadCountRes(const TCmdActiveThreadCountRes&);
  TCmdActiveThreadCountRes& operator=(const TCmdActiveThreadCountRes&);
  TCmdActiveThreadCountRes() : histogramSchemaType(0), timeStamp(0) {
  }

  virtual ~TCmdActiveThreadCountRes() throw();
  int32_t histogramSchemaType;
  std::vector<int32_t>  activeThreadCount;
  int64_t timeStamp;

  _TCmdActiveThreadCountRes__isset __isset;

  void __set_histogramSchemaType(const int32_t val);

  void __set_activeThreadCount(const std::vector<int32_t> & val);

  void __set_timeStamp(const int64_t val);

  bool operator == (const TCmdActiveThreadCountRes & rhs) const
  {
    if (!(histogramSchemaType == rhs.histogramSchemaType))
      return false;
    if (!(activeThreadCount == rhs.activeThreadCount))
      return false;
    if (__isset.timeStamp != rhs.__isset.timeStamp)
      return false;
    else if (__isset.timeStamp && !(timeStamp == rhs.timeStamp))
      return false;
    return true;
  }
  bool operator != (const TCmdActiveThreadCountRes &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const TCmdActiveThreadCountRes & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(TCmdActiveThreadCountRes &a, TCmdActiveThreadCountRes &b);

std::ostream& operator<<(std::ostream& out, const TCmdActiveThreadCountRes& obj);

typedef struct _TActiveThreadDump__isset {
  _TActiveThreadDump__isset() : execTime(false), threadDump(false) {}
  bool execTime :1;
  bool threadDump :1;
} _TActiveThreadDump__isset;

class TActiveThreadDump : public virtual ::apache::thrift::TBase {
 public:

  TActiveThreadDump(const TActiveThreadDump&);
  TActiveThreadDump& operator=(const TActiveThreadDump&);
  TActiveThreadDump() : execTime(0) {
  }

  virtual ~TActiveThreadDump() throw();
  int64_t execTime;
  TThreadDump threadDump;

  _TActiveThreadDump__isset __isset;

  void __set_execTime(const int64_t val);

  void __set_threadDump(const TThreadDump& val);

  bool operator == (const TActiveThreadDump & rhs) const
  {
    if (!(execTime == rhs.execTime))
      return false;
    if (!(threadDump == rhs.threadDump))
      return false;
    return true;
  }
  bool operator != (const TActiveThreadDump &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const TActiveThreadDump & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(TActiveThreadDump &a, TActiveThreadDump &b);

std::ostream& operator<<(std::ostream& out, const TActiveThreadDump& obj);

typedef struct _TCmdActiveThreadDump__isset {
  _TCmdActiveThreadDump__isset() : execTime(false) {}
  bool execTime :1;
} _TCmdActiveThreadDump__isset;

class TCmdActiveThreadDump : public virtual ::apache::thrift::TBase {
 public:

  TCmdActiveThreadDump(const TCmdActiveThreadDump&);
  TCmdActiveThreadDump& operator=(const TCmdActiveThreadDump&);
  TCmdActiveThreadDump() : execTime(0) {
  }

  virtual ~TCmdActiveThreadDump() throw();
  int64_t execTime;

  _TCmdActiveThreadDump__isset __isset;

  void __set_execTime(const int64_t val);

  bool operator == (const TCmdActiveThreadDump & rhs) const
  {
    if (__isset.execTime != rhs.__isset.execTime)
      return false;
    else if (__isset.execTime && !(execTime == rhs.execTime))
      return false;
    return true;
  }
  bool operator != (const TCmdActiveThreadDump &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const TCmdActiveThreadDump & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(TCmdActiveThreadDump &a, TCmdActiveThreadDump &b);

std::ostream& operator<<(std::ostream& out, const TCmdActiveThreadDump& obj);

typedef struct _TCmdActiveThreadDumpRes__isset {
  _TCmdActiveThreadDumpRes__isset() : threadDumps(false) {}
  bool threadDumps :1;
} _TCmdActiveThreadDumpRes__isset;

class TCmdActiveThreadDumpRes : public virtual ::apache::thrift::TBase {
 public:

  TCmdActiveThreadDumpRes(const TCmdActiveThreadDumpRes&);
  TCmdActiveThreadDumpRes& operator=(const TCmdActiveThreadDumpRes&);
  TCmdActiveThreadDumpRes() {
  }

  virtual ~TCmdActiveThreadDumpRes() throw();
  std::vector<TActiveThreadDump>  threadDumps;

  _TCmdActiveThreadDumpRes__isset __isset;

  void __set_threadDumps(const std::vector<TActiveThreadDump> & val);

  bool operator == (const TCmdActiveThreadDumpRes & rhs) const
  {
    if (!(threadDumps == rhs.threadDumps))
      return false;
    return true;
  }
  bool operator != (const TCmdActiveThreadDumpRes &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const TCmdActiveThreadDumpRes & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(TCmdActiveThreadDumpRes &a, TCmdActiveThreadDumpRes &b);

std::ostream& operator<<(std::ostream& out, const TCmdActiveThreadDumpRes& obj);

typedef struct _TCommandEcho__isset {
  _TCommandEcho__isset() : message(false) {}
  bool message :1;
} _TCommandEcho__isset;

class TCommandEcho : public virtual ::apache::thrift::TBase {
 public:

  TCommandEcho(const TCommandEcho&);
  TCommandEcho& operator=(const TCommandEcho&);
  TCommandEcho() : message() {
  }

  virtual ~TCommandEcho() throw();
  std::string message;

  _TCommandEcho__isset __isset;

  void __set_message(const std::string& val);

  bool operator == (const TCommandEcho & rhs) const
  {
    if (!(message == rhs.message))
      return false;
    return true;
  }
  bool operator != (const TCommandEcho &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const TCommandEcho & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(TCommandEcho &a, TCommandEcho &b);

std::ostream& operator<<(std::ostream& out, const TCommandEcho& obj);

typedef struct _TCommandTransfer__isset {
  _TCommandTransfer__isset() : applicationName(false), agentId(false), startTime(false), payload(false) {}
  bool applicationName :1;
  bool agentId :1;
  bool startTime :1;
  bool payload :1;
} _TCommandTransfer__isset;

class TCommandTransfer : public virtual ::apache::thrift::TBase {
 public:

  TCommandTransfer(const TCommandTransfer&);
  TCommandTransfer& operator=(const TCommandTransfer&);
  TCommandTransfer() : applicationName(), agentId(), startTime(0), payload() {
  }

  virtual ~TCommandTransfer() throw();
  std::string applicationName;
  std::string agentId;
  int64_t startTime;
  std::string payload;

  _TCommandTransfer__isset __isset;

  void __set_applicationName(const std::string& val);

  void __set_agentId(const std::string& val);

  void __set_startTime(const int64_t val);

  void __set_payload(const std::string& val);

  bool operator == (const TCommandTransfer & rhs) const
  {
    if (!(applicationName == rhs.applicationName))
      return false;
    if (!(agentId == rhs.agentId))
      return false;
    if (__isset.startTime != rhs.__isset.startTime)
      return false;
    else if (__isset.startTime && !(startTime == rhs.startTime))
      return false;
    if (!(payload == rhs.payload))
      return false;
    return true;
  }
  bool operator != (const TCommandTransfer &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const TCommandTransfer & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(TCommandTransfer &a, TCommandTransfer &b);

std::ostream& operator<<(std::ostream& out, const TCommandTransfer& obj);

typedef struct _TCommandTransferResponse__isset {
  _TCommandTransferResponse__isset() : routeResult(false), payload(false), message(false) {}
  bool routeResult :1;
  bool payload :1;
  bool message :1;
} _TCommandTransferResponse__isset;

class TCommandTransferResponse : public virtual ::apache::thrift::TBase {
 public:

  TCommandTransferResponse(const TCommandTransferResponse&);
  TCommandTransferResponse& operator=(const TCommandTransferResponse&);
  TCommandTransferResponse() : routeResult((TRouteResult::type)0), payload(), message() {
  }

  virtual ~TCommandTransferResponse() throw();
  TRouteResult::type routeResult;
  std::string payload;
  std::string message;

  _TCommandTransferResponse__isset __isset;

  void __set_routeResult(const TRouteResult::type val);

  void __set_payload(const std::string& val);

  void __set_message(const std::string& val);

  bool operator == (const TCommandTransferResponse & rhs) const
  {
    if (!(routeResult == rhs.routeResult))
      return false;
    if (!(payload == rhs.payload))
      return false;
    if (__isset.message != rhs.__isset.message)
      return false;
    else if (__isset.message && !(message == rhs.message))
      return false;
    return true;
  }
  bool operator != (const TCommandTransferResponse &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const TCommandTransferResponse & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(TCommandTransferResponse &a, TCommandTransferResponse &b);

std::ostream& operator<<(std::ostream& out, const TCommandTransferResponse& obj);



#endif
