var uareuprint = require("../index");

var inits = uareuprint.init();

uareuprint.startScan(function(data){
    console.log(data);
});