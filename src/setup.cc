#include <nan.h>
#include <bitlib.h>

#include "setup.h"

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

class BitscopeSetupWorker : public AsyncWorker {
  public:
    BitscopeSetupWorker(Callback* callback, int channel) : AsyncWorker(callback), channel(channel) {}
    ~BitscopeSetupWorker() {}

    void Execute () {
      BL_Intro(BL_ZERO);
      BL_Delay(BL_ZERO);
      BL_Select(BL_SELECT_CHANNEL, channel);
      BL_Trigger(BL_ZERO, BL_TRIG_RISE);
      BL_Select(BL_SELECT_SOURCE, BL_SOURCE_POD);
      BL_Range(BL_Count(BL_COUNT_RANGE));
      BL_Offset(BL_ZERO);
      BL_Enable(1);
    }

    void HandleOKCallback () {
      HandleScope scope;
      callback->Call(0, NULL);
    }

  private:
    int channel;
};

NAN_METHOD(bitscope_setup) {
  HandleScope scope;
  Local<Value> channel = info[0].As<Value>();
  Callback* callback = new Callback(info[1].As<Function>());
  AsyncQueueWorker(
    new BitscopeSetupWorker(
      callback,
      channel->Int32Value()
    )
  );
}
