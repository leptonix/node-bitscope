#include <nan.h>
#include <bitlib.h>

#include "init.h"

using namespace v8;

class BitscopeInitWorker : public NanAsyncWorker {
  public:
    BitscopeInitWorker(NanCallback* callback, int device) : NanAsyncWorker(callback), device(device) {}
    ~BitscopeInitWorker() {}

    void Execute () {
      success = BL_Open("", device) && BL_Select(BL_SELECT_DEVICE, device) == device;
      BL_Mode(BL_MODE_DUAL);
    }

    void HandleOKCallback () {
      NanScope();
      Local<Value> argv[1];
      if (success) {
        argv[0] = NanNull();
      } else {
        argv[0] = NanNew("unable to initialize bitscope device");
      }
      callback->Call(1, argv);
    }

  private:
    int device;
    int success;
};

NAN_METHOD(bitscope_init) {
  NanScope();
  Local<Value> opt_device = args[0].As<Value>();
  NanCallback* callback = new NanCallback(args[1].As<Function>());
  NanAsyncQueueWorker(
    new BitscopeInitWorker(
      callback,
      opt_device->IsNumber() ? opt_device->Int32Value() : 0
    )
  );
  NanReturnUndefined();
}
