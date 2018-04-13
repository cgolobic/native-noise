#include <nan.h>
#include <iostream>
using namespace std;
using namespace Nan;
using namespace v8;

void del_cb(char* data, void* hint) {
  std::cout << "pixelData deleted" << std::endl;
  delete[](data);
}

NAN_METHOD(Generate) {
  // statically declaring pixelData array here will cause it to go out of scope outside this function
  // other option is to statically declare and use Nan::CopyBuffer to create result buffer, but this
  // incurs the cost of an implicit memcpy
  unsigned char* pixelData = new unsigned char[12]();
  pixelData[0] = 255;
  pixelData[3] = 255;

  pixelData[5] = 255;
  pixelData[7] = 255;

  pixelData[10] = 255;
  pixelData[11] = 255;
  auto buf = Nan::NewBuffer((char *)pixelData, 12, del_cb, pixelData);
  info.GetReturnValue().Set(buf.ToLocalChecked());
}

NAN_MODULE_INIT(Init) {
  Nan::Set(target, New<String>("generate").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(Generate)).ToLocalChecked());
}

NODE_MODULE(noise, Init);