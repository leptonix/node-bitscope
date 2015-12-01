#include <nan.h>
#include <bitlib.h>

#include "close.h"

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

class BitscopeCloseWorker : public AsyncWorker {
  public:
    BitscopeCloseWorker(Callback* callback) : AsyncWorker(callback) {}
    ~BitscopeCloseWorker() {}

    void Execute () {
      BL_Close();
    }

    void HandleOKCallback () {
      HandleScope scope;
      callback->Call(0, NULL);
    }
};

NAN_METHOD(bitscope_close) {
  HandleScope scope;
  Callback* callback = new Callback(info[0].As<Function>());
  AsyncQueueWorker(new BitscopeCloseWorker(callback));
}
