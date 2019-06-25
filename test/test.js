var uareuprint = require("../index");

var inits = uareuprint.init();
// let hReader = 
if(inits){ 
    // uareuprint.startScan(12,function(data,data2,data3){
    //     console.log("\nstartScan data\n");
    //     console.log(data);
    //     console.log(data2);
    //     console.log(data3);
    // });
    uareuprint.startEnroll(function(status,fimage,fid){
        console.log("\n\ntest status startEnroll ",status);
        console.log("\ntest fimage startEnroll ",fimage);
        console.log("\ntest fmd startEnroll",fid);
    });
}
