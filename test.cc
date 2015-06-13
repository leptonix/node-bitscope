#include <nan.h>
#include "test.h"

using v8::Number;

NAN_METHOD(test) {
  NanScope();

  //int points = args[0]->Uint32Value();

  NanReturnValue(NanNew<Number>(42));
}
