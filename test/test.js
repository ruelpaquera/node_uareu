var biometric = require("../index"); 
var parseString = require('xml2js').parseString;

var inits = biometric.init(); 
if(inits){ 
    // uareuprint.startEnroll(1,function(status,fimage,fid){
    //     // console.log("\n\ntest status startEnroll ",status);
    //     // console.log("\ntest fimage startEnroll ",fimage);
    //     console.log("\ntest fmd startEnroll",fid); 
    // });
    let FID_DATA = '<?xml version="1.0" encoding="UTF-8"?><Fid><Bytes>Rk1SACAyMAABKAAz/v8AAAFlAYgAxQDFAQAAAFYsgGMAplRigEsA11xgQBoAsWJfQH4A/EdegKAAy0ldQOcBMkFbgGgBBJ5YgGIAvlJYgG0BQEFTgD8AQ0pSQOsBGz9SQMsA/phRgLYBI0FRgMwBQUZRQDoA4XFQQNcA6JlQQPEBJp9QgCgAalhQQKsBSKdPgFEBGplOQEEBG5dOQFgBYj9OQJIAxqdNgEABQphMQDoAhFVKQBUAl1pJgB0AalZEgFwBSJlCgDUAP6RCgFABAJ5BQBsASE1BQBkAVlJBgPEBTUtAQNYAn0k+gN4BU2A6QNIBTVI3QMYBXg02AK0BaUgzAEYA+IswAP8BHD0uAE0A/KAuAKYBckgsAFsBcZ4rAA8ArGAoAAA=</Bytes><Format>1769473</Format><Version>1.0.0</Version></Fid>'
    // let fmt = "11"; 115246 1562
    // console.log(fmt);// 434
    let result_FID = '';
    parseString(FID_DATA, {trim: true},function (err, result) { 
        result_FID = result.Fid.Bytes[0];
    });
    biometric.startVerify({fmt: result_FID,stat:1},function(status,fimage){
        console.log("\n\ntest status startEnroll ",status);
        console.log("\ntest fimage startEnroll ",fimage); 
    });    
}