const noise = require('bindings')('noise');

let res = noise.generate();
console.log(res);