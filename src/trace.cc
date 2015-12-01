#include <nan.h>
#include <bitlib.h>

#include "trace.h"

using v8::Function;
using v8::Local;
using v8::Number;
using v8::Value;
using Nan::AsyncWorker;
using Nan::AsyncQueueWorker;
using Nan::Callback;
using Nan::HandleScope;
using Nan::New;
using Nan::Null;

#define MY_DEVICE 0
#define MY_MODE BL_MODE_FAST

class BitscopeTraceWorker : public AsyncWorker {
  public:
    BitscopeTraceWorker(Callback* callback, int rate, int size) : AsyncWorker(callback), rate(rate), size(size) {}
    ~BitscopeTraceWorker() {}

    void Execute() {
      BL_Rate(rate);
      size = BL_Size(size);
      success = BL_Trace(BL_TRACE_FORCED, BL_SYNCHRONOUS);
    }

    void HandleOKCallback() {
      HandleScope scope;
      if (success) {
        Local<Value> argv[] = {Null(), New(size)};
        callback->Call(2, argv);
      } else {
        Local<Value> argv[] = {New("bitscope trace failed").ToLocalChecked()};
        callback->Call(1, argv);
      }
    }

  private:
    int rate;
    int size;
    int success;
};

NAN_METHOD(bitscope_trace) {
  HandleScope scope;
  Local<Value> rate = info[0].As<Value>();
  Local<Value> size = info[1].As<Value>();
  Callback* callback = new Callback(info[2].As<Function>());
  AsyncQueueWorker(
    new BitscopeTraceWorker(
      callback,
      rate->IsNumber() ? rate->Int32Value() : 1000,
      size->IsNumber() ? size->Int32Value() : 10
    )
  );
}
