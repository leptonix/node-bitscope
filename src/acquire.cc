#include <nan.h>
#include <bitlib.h>

#include "acquire.h"

using v8::Array;
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

class BitscopeAcquireWorker : public AsyncWorker {
  public:
    BitscopeAcquireWorker(Callback* callback, int channel, int size) : AsyncWorker(callback), channel(channel), size(size) {
      buffer = new double[size];
    }
    ~BitscopeAcquireWorker() {
      delete buffer;
    }

    void Execute () {
      BL_Select(BL_SELECT_CHANNEL, channel);
      size = BL_Acquire(size, buffer);
    }

    void HandleOKCallback () {
      HandleScope scope;
      if (size > 0) {
        Local<Array> a = New<Array>(size);
        Local<Value> argv[] = {Null(), a};
        for (int i = 0; i < size; i++) {
          a->Set(i, New(buffer[i]));
        }
        callback->Call(2, argv);
      } else {
        Local<Value> argv[] = {New("unable to acquire data from bitscope channel").ToLocalChecked()};
        callback->Call(1, argv);
      }
    }

  private:
    int channel;
    int size;
    double* buffer;
};

NAN_METHOD(bitscope_acquire) {
  HandleScope scope;
  Local<Value> channel = info[0].As<Value>();
  Local<Value> size = info[1].As<Value>();
  Callback* callback = new Callback(info[2].As<Function>());
  AsyncQueueWorker(
    new BitscopeAcquireWorker(
      callback,
      channel->Int32Value(),
      size->Int32Value()
    )
  );
}
