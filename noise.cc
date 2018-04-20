#define STB_PERLIN_IMPLEMENTATION

#include <nan.h>
#include <iostream>
#include "stb_perlin.h"
using namespace std;
using namespace Nan;
using namespace v8;

void del_cb(char* data, void* hint) {
  std::cout << "pixelData deleted" << std::endl;
  delete[](data);
}

void fill(unsigned char* buffer, int width, int height, float z) {
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      float ni = (i / (float)width);
      float nj = (j / (float)height);
      float res = stb_perlin_ridge_noise3(nj, ni , z, 1.95f, 0.5f,1.0f, 6, 0, 0, 0);
      int baseIndex = ((height * i) + j) * 4;
      buffer[baseIndex]      = res * 255;
      buffer[baseIndex + 1]  = res * 255;
      buffer[baseIndex + 2]  = res * 255;
      buffer[baseIndex + 3]  = 255;
    }
  }
}


NAN_METHOD(Generate) {
  // statically declaring pixelData array here will cause it to go out of scope outside this function
  // other option is to statically declare and use Nan::CopyBuffer to create result buffer, but this
  // incurs the cost of an implicit memcpy
  unsigned char* pixelData = new unsigned char[40000]();
  fill(pixelData, 100, 100, 0.8f);
  auto buf = Nan::NewBuffer((char *)pixelData, 40000, del_cb, pixelData);
  info.GetReturnValue().Set(buf.ToLocalChecked());
}

NAN_METHOD(GenerateShared) {
  unsigned char* sharedBuffer = (unsigned char*) node::Buffer::Data(info[0]->ToObject());
  int width = info[1]->Uint32Value();
  int height = info[2]->Uint32Value();
  float z = info[3]->NumberValue();
  fill(sharedBuffer, width, height, z);
}

NAN_MODULE_INIT(Init) {
  Nan::Set(target, New<String>("generate").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(Generate)).ToLocalChecked());
  Nan::Set(target, New<String>("generateShared").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(GenerateShared)).ToLocalChecked());
}

NODE_MODULE(noise, Init);