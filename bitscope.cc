#include <nan.h>
#include "capture.h"
#include "test.h"

using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;
using v8::String;

void InitAll(Handle<Object> exports) {
  exports->Set(NanNew<String>("capture"), NanNew<FunctionTemplate>(capture)->GetFunction());
  exports->Set(NanNew<String>("test"), NanNew<FunctionTemplate>(test)->GetFunction());
}

NODE_MODULE(bitscope, InitAll)
