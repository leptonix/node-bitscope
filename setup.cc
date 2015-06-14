#include <nan.h>
#include <bitlib.h>

#include "setup.h"

using namespace v8;

class BitscopeSetupWorker : public NanAsyncWorker {
  public:
    BitscopeSetupWorker(NanCallback* callback, int channel) : NanAsyncWorker(callback), channel(channel) {}
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
      NanScope();
      callback->Call(0, NULL);
    }

  private:
    int channel;
};

NAN_METHOD(bitscope_setup) {
  NanScope();
  Local<Value> channel = args[0].As<Value>();
  NanCallback* callback = new NanCallback(args[1].As<Function>());
  NanAsyncQueueWorker(
    new BitscopeSetupWorker(
      callback,
      channel->Int32Value()
    )
  );
  NanReturnUndefined();
}
