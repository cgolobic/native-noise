const noise = require('../../');

var res = noise.generate();
let arrayBufferView = new Uint8Array(res);
for (var i = 0; i < arrayBufferView.length; i+=3) {
  console.log(arrayBufferView[i], arrayBufferView[i + 1], arrayBufferView[i + 2]);
}

var buffer = noise.load('../electron/01_HiRes.png');
console.log(buffer);
let imageView = new Uint8Array(buffer.data);
for (var i = 0; i < imageView.length; i+=3) {
  console.log(imageView[i], imageView[i + 1], imageView[i + 2]);
}