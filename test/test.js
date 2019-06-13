var uareuprint = require("../index");

var inits = uareuprint.init(); 
console.log("init : ",inits);
if(inits){ 
    // uareuprint.startScan(12,function(data,data2,data3){
    //     console.log("\nstartScan data\n");
    //     console.log(data);
    //     console.log(data2);
    //     console.log(data3);
    // });
    uareuprint.startEnroll(function(data,data2,data3){
        console.log("\nstartScan data\n");
        console.log(data);
    });
}
