var uareuprint = require("../index");

var inits = uareuprint.init();
console.log(inits)
// if(inits){
//     // console.log(uareuprint.selectfpd());
// }
// // console.log(uareuprint.selectfpd());
uareuprint.startScan(function(data){
    console.log(data);
});