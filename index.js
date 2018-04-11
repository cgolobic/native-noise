const noise = require('bindings')('noise');

var res = noise.generate();
let arrayBufferView = new Uint8Array(res);
for (var i = 0; i < arrayBufferView.length; i+=3) {
  console.log(arrayBufferView[i], arrayBufferView[i + 1], arrayBufferView[i + 2]);
}
setTimeout(() => {
  // force a GC sweep after 5 seconds
  global.gc();
}, 5000);