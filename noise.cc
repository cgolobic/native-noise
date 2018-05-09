#define STB_PERLIN_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include <nan.h>
#include <iostream>
#include "stb_perlin.h"
#include "stb_image.h"
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

NAN_METHOD(Desaturate) {
  unsigned char* sharedBuffer = (unsigned char*) node::Buffer::Data(info[0]->ToObject());
  float r_sat = info[1]->NumberValue();
  float g_sat = info[2]->NumberValue();
  float b_sat = info[3]->NumberValue();
  int width = info[4]->Uint32Value();
  int height = info[5]->Uint32Value();
  int l = width * height;
  for(int i = 0; i < l; i++) {
    sharedBuffer[i * 4] = sharedBuffer[i] * r_sat;
    sharedBuffer[(i * 4) + 1] = sharedBuffer[i] * g_sat;
    sharedBuffer[(i * 4) + 2] = sharedBuffer[i] * b_sat;
  }
}

NAN_METHOD(Load) {
  v8::Local<v8::Object> img_data = Nan::New<v8::Object>();
  Nan::Utf8String s(info[0]);
  int w, h, n;
  unsigned char *data = stbi_load(*s, &w, &h, &n, 4);
  std::cout << w << ',' << h << ',' << n << std::endl;
  auto buffer = Nan::NewBuffer((char *)data, w * h * 4, del_cb, data);
  Nan::Set(img_data, Nan::New("w").ToLocalChecked(), Nan::New<v8::Integer>(w));
  Nan::Set(img_data, Nan::New("h").ToLocalChecked(), Nan::New<v8::Integer>(h));
  Nan::Set(img_data, Nan::New("data").ToLocalChecked(), buffer.ToLocalChecked());
  
  //img_data.data = buffer.ToLocalChecked();
  //info.GetReturnValue().Set(img_data.ToLocalChecked());
  info.GetReturnValue().Set(img_data);
}

NAN_MODULE_INIT(Init) {
  Nan::Set(target, New<String>("generate").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(Generate)).ToLocalChecked());

  Nan::Set(target, New<String>("generateShared").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(GenerateShared)).ToLocalChecked());

  Nan::Set(target, New<String>("desaturate").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(Desaturate)).ToLocalChecked());

  Nan::Set(target, New<String>("load").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(Load)).ToLocalChecked());
}

NODE_MODULE(noise, Init);