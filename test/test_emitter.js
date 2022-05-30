var biometric = require("../index"); 
var parseString = require('xml2js').parseString;
var emitter = require('events').EventEmitter

var inits = biometric.init(); 
 
if(inits){

    function loopProcess() {
        var test = new emitter();
        biometric.scanning(function(fingerdata,image){
            test.emit('onCapture',(fingerdata,image));
            return test ;
        }); 
    }
    let lp = new loopProcess();
    lp.on('onCapture', function (returns) {
        console.log(returns);
    });
}
 
