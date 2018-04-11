#include <nan.h>
using namespace std;
using namespace Nan;
using namespace v8;

NAN_METHOD(Generate) {
  unsigned char pixelData[9] = {
    255, 0, 0,
    0, 255, 0,
    0, 0, 255
  };
  auto buf = Nan::CopyBuffer((char *)&pixelData[0], sizeof(pixelData));
  info.GetReturnValue().Set(buf.ToLocalChecked());
}

NAN_MODULE_INIT(Init) {
  Nan::Set(target, New<String>("generate").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(Generate)).ToLocalChecked());
}

NODE_MODULE(noise, Init);