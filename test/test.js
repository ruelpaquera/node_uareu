var uareuprint = require("../index");

var inits = uareuprint.init();
// let hReader = 
if(inits){ 
    // uareuprint.startEnroll(1,function(status,fimage,fid){
    //     // console.log("\n\ntest status startEnroll ",status);
    //     // console.log("\ntest fimage startEnroll ",fimage);
    //     console.log("\ntest fmd startEnroll",fid); 
    // });
    let fmt  = "Rk1SACAyMAABKAAz/v8AAAFlAYgAxQDFAQAAAFYsgGMAplRigEsA11xgQBoAsWJfQH4A/EdegKAAy0ldQOcBMkFbgGgBBJ5YgGIAvlJYgG0BQEFTgD8AQ0pSQOsBGz9SQMsA/phRgLYBI0FRgMwBQUZRQDoA4XFQQNcA6JlQQPEBJp9QgCgAalhQQKsBSKdPgFEBGplOQEEBG5dOQFgBYj9OQJIAxqdNgEABQphMQDoAhFVKQBUAl1pJgB0AalZEgFwBSJlCgDUAP6RCgFABAJ5BQBsASE1BQBkAVlJBgPEBTUtAQNYAn0k+gN4BU2A6QNIBTVI3QMYBXg02AK0BaUgzAEYA+IswAP8BHD0uAE0A/KAuAKYBckgsAFsBcZ4rAA8ArGAoAAA=";
    let fmt2 = "Rk1SACAyMAAAAAGqAAABQAFoAMgAyAEAAABWQkCAARu5YECSAVC+W4DyAG7qW4B8ALiwW4CYAMi5WkBoAKCmWUDCAFh7V4DGAQzNVkCwATPPVoCkAGqQVUCwAPvHVYDeATxMVUCwAFYAVECKAPq7VEB4AHGaU0D0AUBWU4EQAIBqU0CUATnFU0BkANG1UkBWAOm3UkCeAQ3JUIBVAJQhTkC6AVxSTkB5AGIQTEC0AK+qSYDEAJ2LSYDOANPOSIBoAQezRoDaAQdURkDWAVxYRYCwAKqpRUA+AMSoRECuAKapQ0EqALBcQUBhAMimQYBiAFiSQEDBAMK5QEDEAK+bQIDWARPOP0A4ANqsPUCiAI4LO0AwAMSyOUBiAP01OIBAAM2pOIBnAQOuN0DgAR/XN4BMANSuN4AoAL80NYBVAMcrNQA9ALsrNQAyALA4NACCAVywNACeAJSfMwBJAPixMQCWAWDDMQBCAOisMQCFAWGvMABOAMisLwB8AVqwLgDMAMXcLQDHAL6sKwEbAF/tKwBEAO6vKwEuAKndKgDKAM3JKgDBAKfpKAAA";
    // let fmt = "11"; 115246 1562
    // console.log(fmt); 434
    
    uareuprint.startVerify({fmt: fmt,stat:1},function(status,fimage,fid){
        // console.log("\n\ntest status startEnroll ",status);
        // console.log("\ntest fimage startEnroll ",fimage);
        // console.log("\ntest fmd startEnroll",fid); 
    });    
}