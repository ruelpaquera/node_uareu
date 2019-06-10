var uareuprint = require("../index");

var inits = uareuprint.init(); 
console.log("init : ",inits);
if(inits){ 
    var t = uareuprint.startScan(12,function(data){
        console.log("data");
        console.log(data);
    }); 
    console.log("t",t);
}