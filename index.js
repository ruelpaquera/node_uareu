// var biometric = require("bindings")("biometric");
// process.env.LD_LIBRARY_PATH='./lib';
var biometric = require("./build/Release/biometric.node");
module.exports = biometric;