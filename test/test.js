var uareuprint = require("../index");

var inits = uareuprint.init();
// let hReader = 
if(inits){ 
    // uareuprint.startEnroll(1,function(status,fimage,fid){
    //     // console.log("\n\ntest status startEnroll ",status);
    //     // console.log("\ntest fimage startEnroll ",fimage);
    //     // console.log("\ntest fmd startEnroll",fid); 
    // });
    let fmt = "Rk1SACAyMAABuAAz/v8AAAFlAYgAxQDFAQAAAFZEQDQAvnRkQGEAkF5kQJgApE9dQFAAm2VbQNAAoJ1WgOEBGTxVgHoA1F5SQC8A1xlSQNcAukRQQPwBV4ZQQFcAsGhPgGEA7INNgOABQCtNQEIA4hdLgGEBZJRJgIQBWzVFQPEBN4dFQJgBTTpFQLoA/kFEgLcBH5RDQNcBQYtDgG8BU4xCgNQBHJZCQMUBITxBQHYA/plBQLcBLJFBQNYBY4dBQMABQjpAQKMBCT5AQQkBMo5AQOUBYYNAQGcBYpM/QOYBaYY/QNoBWzU/QKMBHzo9QMQBVEE8gLwBKo88gJABU4w7gL8BBDo6QNQBSy86QO4BQTM5gNYBLY85QMEBNDw5QMsBJzw4gHoBXII4QKYBUDU3QOEBaCk3gHYA+Zs3gQUAzJ02QHEA+pQ2gKMBAJQ2gJ4BQYs2QLoBTZQ2QJ4BWDI2QMsBOzw2QJ0BR4c2AN4BIUE1AMsBQok0AL4BYTU0ANEBNI8zAIYBTpYzAOABKjgxAP8BXYMxALYBApcxAK0BVo4xAMABFj0wAHQBZy0wAHEA7YEwAAA=";
    // let fmt = "11";
    // console.log(fmt);
    uareuprint.startVerify(fmt,function(status,fimage,fid){
        // console.log("\n\ntest status startEnroll ",status);
        // console.log("\ntest fimage startEnroll ",fimage);
        // console.log("\ntest fmd startEnroll",fid); 
    });    
}
