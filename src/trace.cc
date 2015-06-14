#include <nan.h>
#include <bitlib.h>

#include "trace.h"

using namespace v8;

#define MY_DEVICE 0
#define MY_MODE BL_MODE_FAST

class BitscopeTraceWorker : public NanAsyncWorker {
  public:
    BitscopeTraceWorker(NanCallback* callback, int rate, int size) : NanAsyncWorker(callback), rate(rate), size(size) {}
    ~BitscopeTraceWorker() {}

    void Execute() {
      BL_Rate(rate);
      size = BL_Size(size);
      success = BL_Trace(BL_TRACE_FORCED, BL_SYNCHRONOUS);
    }

    void HandleOKCallback() {
      NanScope();
      if (success) {
        Local<Value> argv[] = {NanNull(), NanNew(size)};
        callback->Call(2, argv);
      } else {
        Local<Value> argv[] = {NanNew("bitscope trace failed")};
        callback->Call(1, argv);
      }
    }

  private:
    int rate;
    int size;
    int success;
};

NAN_METHOD(bitscope_trace) {
  NanScope();
  Local<Value> rate = args[0].As<Value>();
  Local<Value> size = args[1].As<Value>();
  NanCallback* callback = new NanCallback(args[2].As<Function>());
  NanAsyncQueueWorker(
    new BitscopeTraceWorker(
      callback,
      rate->IsNumber() ? rate->Int32Value() : 1000,
      size->IsNumber() ? size->Int32Value() : 10
    )
  );
  NanReturnUndefined();
}
