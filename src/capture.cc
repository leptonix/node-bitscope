#include <nan.h>
#include <bitlib.h>

#include "capture.h"

using namespace v8;
using Nan::AsyncWorker;
using Nan::AsyncQueueWorker;
using Nan::Callback;
using Nan::HandleScope;

#define MY_DEVICE 0
#define MY_MODE BL_MODE_FAST

class CaptureWorker : public AsyncWorker {
  public:
    CaptureWorker(Callback* callback, int channel, int rate, int size) : AsyncWorker(callback), channel(channel), rate(rate), size(size) {
      buffer = new double[size];
    }
    ~CaptureWorker() {
      delete buffer;
    }

    void Execute () {
      if (!BL_Open("", MY_DEVICE)) {
        error = "Failed to find a devices";
        goto exit;
      }
      if (BL_Select(BL_SELECT_DEVICE, MY_DEVICE) != MY_DEVICE) {
        error = "Failed to select device";
        goto exit;
      }
      if (BL_Select(BL_SELECT_CHANNEL, channel) != channel) {
        error = "Failed to select channel";
        goto exit;
      }
      if (BL_Mode(MY_MODE) != MY_MODE) {
        error = "Failed to select mode";
        goto exit;
      }
      BL_Intro(BL_ZERO);
      BL_Delay(BL_ZERO);
      BL_Rate(rate);
      BL_Size(size);
      BL_Select(BL_SELECT_CHANNEL, channel);
      BL_Trigger(BL_ZERO, BL_TRIG_RISE);
      BL_Select(BL_SELECT_SOURCE, BL_SOURCE_POD);
      BL_Range(BL_Count(BL_COUNT_RANGE));
      BL_Offset(BL_ZERO);
      BL_Enable(1);

      if (BL_Trace(BL_TRACE_FORCED, BL_SYNCHRONOUS)) {
        BL_Select(BL_SELECT_CHANNEL, channel);
        if (BL_Acquire(size, buffer) != size ) {
          error = "Failed to acquire data";
        }
      }
      exit: BL_Close();
    }

    void HandleOKCallback () {
      HandleScope scope;
      Local<Value> argv[2];
      if (error.length() > 0) {
        argv[0] = Nan::New<v8::String>(error.c_str(), error.length());
        argv[1] = Nan::Null();
      } else {
        Local<Array> a = Nan::New<Array>(size);
        for (int i = 0; i < size; i++) {
          a->Set(i, Nan::New(buffer[i]));
        }
        argv[0] = Nan::Null();
        argv[1] = a;
      }
      callback->Call(2, argv);
    }

  private:
    int channel;
    int rate;
    int size;
    double* buffer;
    std::string error;
};

NAN_METHOD(capture) {
  HandleScope scope;
  Local<Object> options = args[0].As<Object>();
  Local<Value> opt_channel = options->Get(Nan::New("channel"));
  Local<Value> opt_rate = options->Get(Nan::New("rate"));
  Local<Value> opt_size = options->Get(Nan::New("size"));
  Callback* callback = new Callback(args[1].As<Function>());
  AsyncQueueWorker(
    new CaptureWorker(
      callback,
      opt_channel->IsNumber() ? opt_channel->Int32Value() : 0,
      opt_rate->IsNumber() ? opt_rate->Int32Value() : 1000000,
      opt_size->IsNumber() ? opt_size->Int32Value() : 4
    )
  );
}
