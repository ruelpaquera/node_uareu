var uareuprint = require("../index");

var inits = uareuprint.init(); 
console.log(inits);
if(inits){
    uareuprint.getDevice();
    uareuprint.startScan();   
}