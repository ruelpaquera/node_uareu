var uareuprint = require("../index");

var inits = uareuprint.init();
// let hReader = 
if(inits){ 
    uareuprint.startEnroll(3,function(status,fimage,fid){
        // console.log("\n\ntest status startEnroll ",status);
        // console.log("\ntest fimage startEnroll ",fimage);
        // console.log("\ntest fmd startEnroll",fid); 
    });
}
