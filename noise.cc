#define STB_PERLIN_IMPLEMENTATION

#include <nan.h>
#include <iostream>
#include <random>
#include <math.h>
#include "stb_perlin.h"
using namespace std;
using namespace Nan;
using namespace v8;

void del_cb(char* data, void* hint) {
  std::cout << "pixelData deleted" << std::endl;
  delete[](data);
}

float get_random() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0, 1);
  return dis(gen);
}

NAN_METHOD(Generate) {
  // statically declaring pixelData array here will cause it to go out of scope outside this function
  // other option is to statically declare and use Nan::CopyBuffer to create result buffer, but this
  // incurs the cost of an implicit memcpy
  unsigned char* pixelData = new unsigned char[40000]();
  for (int i = 0; i < 100; i++) {
    for (int j = 0; j < 100; j++) {
      float ni = (i / 100.0f);
      float nj = (j / 100.0f);
      float res = stb_perlin_ridge_noise3(nj, ni , 0.0f, 1.95f, 0.5f,1.0f, 6, 0, 0, 0);
      int baseIndex = ((100 * i) + j) * 4;
      pixelData[baseIndex]      = res * 255;
      pixelData[baseIndex + 1]  = res * 255;
      pixelData[baseIndex + 2]  = res * 255;
      pixelData[baseIndex + 3]  = 255;
    }
  }
  auto buf = Nan::NewBuffer((char *)pixelData, 40000, del_cb, pixelData);
  info.GetReturnValue().Set(buf.ToLocalChecked());
}

NAN_MODULE_INIT(Init) {
  Nan::Set(target, New<String>("generate").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(Generate)).ToLocalChecked());
}

NODE_MODULE(noise, Init);