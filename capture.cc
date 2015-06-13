#include <nan.h>
#include <bitlib.h>

#include "capture.h"

using v8::Function;
using v8::Local;
using v8::Null;
using v8::Number;
using v8::Value;

#define MY_DEVICES 1
#define MY_DEVICE 0
#define MY_CHANNEL 1
#define MY_MODE BL_MODE_FAST
#define MY_RATE 1000000
#define MY_SIZE 4

class CaptureWorker : public NanAsyncWorker {
  public:
    CaptureWorker(NanCallback* callback) : NanAsyncWorker(callback) {}
    ~CaptureWorker() {}

    void Execute () {
      if (!BL_Open("", MY_DEVICE)) {
        error = "Failed to find a devices";
        goto exit;
      }
      if (BL_Select(BL_SELECT_DEVICE, MY_DEVICE) != MY_DEVICE) {
        error = "Failed to select device";
        goto exit;
      }
      if (BL_Select(BL_SELECT_CHANNEL, MY_CHANNEL) != MY_CHANNEL) {
        error = "Failed to select channel";
        goto exit;
      }

      if (BL_Mode(MY_MODE) != MY_MODE) {
        error = "Failed to select mode";
        goto exit;
      }
      BL_Intro(BL_ZERO);
      BL_Delay(BL_ZERO);
      BL_Rate(MY_RATE);
      BL_Size(MY_SIZE);
      BL_Select(BL_SELECT_CHANNEL, MY_CHANNEL);
      BL_Trigger(BL_ZERO, BL_TRIG_RISE);
      BL_Select(BL_SELECT_SOURCE, BL_SOURCE_POD);
      BL_Range(BL_Count(BL_COUNT_RANGE));
      BL_Offset(BL_ZERO);
      BL_Enable(1);

      //printf("   Trace: %d samples @ %.0fHz = %fs\n",BL_Size(BL_ASK),BL_Rate(BL_ASK), BL_Time(BL_ASK));

      if (BL_Trace(BL_TRACE_FORCED, BL_SYNCHRONOUS)) {
          int i, n = MY_SIZE; double d[n];
          BL_Select(BL_SELECT_CHANNEL, MY_CHANNEL);
          if (BL_Acquire(n, d)  == n ) {
            printf("Acquired:");
            for (i = 0; i < n; i++) {
              printf(" %f", d[i]);
            }
            printf("\n\n");
          }
      }
      //printf("Data acquisition complete. Dump Log...\n");
      //printf("%s\n",BL_Log());
      exit: BL_Close();
    }

    void HandleOKCallback () {
      NanScope();
      Local<Value> argv[] = {NanNull(), NanNew<Number>(43)};
      callback->Call(2, argv);
    }

  private:
    std::string error;
};

NAN_METHOD(capture) {
  NanScope();
  NanCallback* callback = new NanCallback(args[1].As<Function>());
  NanAsyncQueueWorker(new CaptureWorker(callback));
  NanReturnUndefined();
}
