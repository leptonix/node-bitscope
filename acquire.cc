#include <nan.h>
#include <bitlib.h>

#include "init.h"

using namespace v8;

class BitscopeAcquireWorker : public NanAsyncWorker {
  public:
    BitscopeAcquireWorker(NanCallback* callback, int channel, int size) : NanAsyncWorker(callback), channel(channel), size(size) {
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
      NanScope();
      if (size > 0) {
        Local<Array> a = NanNew<Array>(size);
        Local<Value> argv[] = {NanNull(), a};
        for (int i = 0; i < size; i++) {
          a->Set(i, NanNew(buffer[i]));
        }
        callback->Call(2, argv);
      } else {
        Local<Value> argv[] = {NanNew("unable to acquire data from bitscope channel")};
        callback->Call(1, argv);
      }
    }

  private:
    int channel;
    int size;
    double* buffer;
};

NAN_METHOD(bitscope_acquire) {
  NanScope();
  Local<Value> channel = args[0].As<Value>();
  Local<Value> size = args[1].As<Value>();
  NanCallback* callback = new NanCallback(args[2].As<Function>());
  NanAsyncQueueWorker(
    new BitscopeAcquireWorker(
      callback,
      channel->Int32Value(),
      size->Int32Value()
    )
  );
  NanReturnUndefined();
}
