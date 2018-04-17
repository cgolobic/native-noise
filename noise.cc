#define STB_PERLIN_IMPLEMENTATION

#include <nan.h>
#include <iostream>
#include <random>
#include "stb_perlin.h"
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
  unsigned char* pixelData = new unsigned char[40000]();
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0, 1);
  float rand;
  for (int i = 0; i < 39996; i += 4) {
    rand = dis(gen);
    float res = stb_perlin_noise3(rand, rand, rand, 0, 0, 0);
    pixelData[i] = res * 255;
    pixelData[i + 1] = res * 255;
    pixelData[i + 2] = res * 255;
    pixelData[i + 3] = 255;
  }

  auto buf = Nan::NewBuffer((char *)pixelData, 40000, del_cb, pixelData);
  info.GetReturnValue().Set(buf.ToLocalChecked());
}

NAN_MODULE_INIT(Init) {
  Nan::Set(target, New<String>("generate").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(Generate)).ToLocalChecked());
}

NODE_MODULE(noise, Init);