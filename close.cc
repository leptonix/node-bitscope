#include <nan.h>
#include <bitlib.h>

#include "close.h"

using namespace v8;

class BitscopeCloseWorker : public NanAsyncWorker {
  public:
    BitscopeCloseWorker(NanCallback* callback) : NanAsyncWorker(callback) {}
    ~BitscopeCloseWorker() {}

    void Execute () {
      BL_Close();
    }

    void HandleOKCallback () {
      NanScope();
      callback->Call(0, NULL);
    }
};

NAN_METHOD(bitscope_close) {
  NanScope();
  NanCallback* callback = new NanCallback(args[0].As<Function>());
  NanAsyncQueueWorker(new BitscopeCloseWorker(callback));
  NanReturnUndefined();
}
