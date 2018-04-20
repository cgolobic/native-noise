const noise = require('bindings')('noise');

module.exports = {
  generate: noise.generate,
  generateShared: noise.generateShared
}
