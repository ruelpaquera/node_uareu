var biometric = require("../index"); 
var parseString = require('xml2js').parseString;

var inits = biometric.init(); 
 
if(inits){ 
    let FID_DATA = [];
    let result_FID = [];
    biometric.scanning(function(fingerdata,image){
        console.log("test fingerdata",fingerdata);
        console.log("test image",image);
        // result_FID.push(fingerdata);
        // setTimeout(() => {
        //     biometric.startVerifyMulti({fmt: result_FID},5000,function(f1,f2,f3,f4,f5){
        //         console.log("\ntest status very 2",f1,f2,f3,f4,f5);
        //         // callback(f1,f2,f3,f4,f5);
        //     }); 
        // }, 2000);
    }); 

  
}
 
