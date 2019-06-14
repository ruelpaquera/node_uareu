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
    uareuprint.startEnroll(function(status,fimage,fmd){
        console.log("\ntest status startEnroll %d \n",status);
        console.log("\ntest fimage startEnroll %d \n",fimage);
        console.log("\ntest fmd startEnroll %d \n",fmd);
    });
}
