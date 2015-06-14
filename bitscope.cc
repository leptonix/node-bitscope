#include <nan.h>
#include "init.h"
#include "setup.h"
#include "trace.h"
#include "acquire.h"
#include "close.h"

using namespace v8;

void InitAll(Handle<Object> exports) {
  exports->Set(NanNew<String>("init"), NanNew<FunctionTemplate>(bitscope_init)->GetFunction());
  exports->Set(NanNew<String>("setup"), NanNew<FunctionTemplate>(bitscope_setup)->GetFunction());
  exports->Set(NanNew<String>("trace"), NanNew<FunctionTemplate>(bitscope_trace)->GetFunction());
  exports->Set(NanNew<String>("acquire"), NanNew<FunctionTemplate>(bitscope_acquire)->GetFunction());
  exports->Set(NanNew<String>("close"), NanNew<FunctionTemplate>(bitscope_close)->GetFunction());
}

NODE_MODULE(bitscope, InitAll)
