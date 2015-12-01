#include <nan.h>
#include "init.h"
#include "setup.h"
#include "trace.h"
#include "acquire.h"
#include "close.h"

using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;
using v8::String;
using Nan::GetFunction;
using Nan::New;
using Nan::Set;

NAN_MODULE_INIT(InitAll) {
  Set(target, New<String>("init").ToLocalChecked(), GetFunction(New<FunctionTemplate>(bitscope_init)).ToLocalChecked());
  Set(target, New<String>("setup").ToLocalChecked(), GetFunction(New<FunctionTemplate>(bitscope_setup)).ToLocalChecked());
  Set(target, New<String>("trace").ToLocalChecked(), GetFunction(New<FunctionTemplate>(bitscope_trace)).ToLocalChecked());
  Set(target, New<String>("acquire").ToLocalChecked(), GetFunction(New<FunctionTemplate>(bitscope_acquire)).ToLocalChecked());
  Set(target, New<String>("close").ToLocalChecked(), GetFunction(New<FunctionTemplate>(bitscope_close)).ToLocalChecked());
}

NODE_MODULE(bitscope, InitAll)
