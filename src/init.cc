#include <nan.h>
#include <bitlib.h>

#include "init.h"

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

class BitscopeInitWorker : public AsyncWorker {
  public:
    BitscopeInitWorker(Callback* callback, int device) : AsyncWorker(callback), device(device) {}
    ~BitscopeInitWorker() {}

    void Execute () {
      success = BL_Open("", device) && BL_Select(BL_SELECT_DEVICE, device) == device;
      BL_Mode(BL_MODE_DUAL);
    }

    void HandleOKCallback () {
      HandleScope scope;
      Local<Value> argv[1];
      if (success) {
        argv[0] = Null();
      } else {
        argv[0] = New("unable to initialize bitscope device").ToLocalChecked();
      }
      callback->Call(1, argv);
    }

  private:
    int device;
    int success;
};

NAN_METHOD(bitscope_init) {
  HandleScope scope;
  Local<Value> opt_device = info[0].As<Value>();
  Callback* callback = new Callback(info[1].As<Function>());
  AsyncQueueWorker(
    new BitscopeInitWorker(
      callback,
      opt_device->IsNumber() ? opt_device->Int32Value() : 0
    )
  );
}
