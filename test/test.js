var biometric = require("../index"); 
var parseString = require('xml2js').parseString;

var inits = biometric.init(); 
 
if(inits){ 
    //let FID_DATA = '<?xml version="1.0" encoding="UTF-8"?><Fid><Bytes>Rk1SACAyMAABKAAz/v8AAAFlAYgAxQDFAQAAAFYsgGMAplRigEsA11xgQBoAsWJfQH4A/EdegKAAy0ldQOcBMkFbgGgBBJ5YgGIAvlJYgG0BQEFTgD8AQ0pSQOsBGz9SQMsA/phRgLYBI0FRgMwBQUZRQDoA4XFQQNcA6JlQQPEBJp9QgCgAalhQQKsBSKdPgFEBGplOQEEBG5dOQFgBYj9OQJIAxqdNgEABQphMQDoAhFVKQBUAl1pJgB0AalZEgFwBSJlCgDUAP6RCgFABAJ5BQBsASE1BQBkAVlJBgPEBTUtAQNYAn0k+gN4BU2A6QNIBTVI3QMYBXg02AK0BaUgzAEYA+IswAP8BHD0uAE0A/KAuAKYBckgsAFsBcZ4rAA8ArGAoAAA=</Bytes><Format>1769473</Format><Version>1.0.0</Version></Fid>'
    let FID_DATA = [];
    FID_DATA[0] = '<?xml version="1.0" encoding="UTF-8"?><Fid><Bytes>Rk1SACAyMAABagAz/v8AAAFlAYgAxQDFAQAAAFY3QF0AxmxigJsAmVhhQH0AyWxggGkAM1FfQOgBDEleQE8AsWxdQHcBFYVcgLQA6lxcgEABDH1cQHgAZFZbQIgAg1pbgMoAulVbQQkA3k9ZQKMA9HFXQFsAGk9WgCMAsG9WgG4BYTNWQEQA0xJVgNQBUEFVgJsAXlBUgMcA0VVUgF0BO4lUQE8AemNUQC4AzRdUQGUBLYlUQEoAYGBTQIwBaJRSQNAAPUhRQGcAR1VQQFAAbQJQQIoAFE5PgPUA16ZOgCkBC3lOgNEBFJ5OQHwAsWdMQMAANaZKgG4BQI1KgLkBKplIQL4BWJlIgKoBKpdHQLkAIk5GQB8AVmBFQNAAG0hCQGMA5RNCgLkBE55BQI8AXa4+QEYAbmM5QN4AJkk4gCgA+Rs4gSAAvUo3QBoBBXc2ARQAjqQxABoBCXcwANABcpwsAO0ALUgpAAA=</Bytes><Format>1769473</Format><Version>1.0.0</Version></Fid>';
    FID_DATA[1] = '<?xml version="1.0" encoding="UTF-8"?><Fid><Bytes>Rk1SACAyMAABrAAz/v8AAAFlAYgAxQDFAQAAAFZCQIYBG4lkQMAAO1ViQGMAgXZfgHQAlnpdQLEAOQFcQKkBQjhbgCoAvHdagJIApYJaQHQA/oJYgJ0BTUdYQIkBZ2JYgJ8AS2dXQFABWw9XgCUA5HlWgMoA841WgEoBJyRWgIMBNXhVQMUBQjpVQJAA6IdUQLcBaE1UQF0AtH1UQHYAU25TgO4AVqNTQFEAqh5TgIIA2YNTgL8BV0ZTgM0BHzNSQGAA9HxRQNABXkZRQKUA2ItQgFMAdRdPQGEBShxNQNQBApJNQK8AjnVMQKABFY9MQCwAqnFLQHQBSx5LgOABJjhKQOgBZEtJQDIBQ3hIQDwAdWxIQPMBVkhIgMUAsJFHQMAAjmxGQKUAhHdDgKkAiXhDQL4AflNBQDQBSx48QLkAnoI6QL4A6o06QEABYhY4QHoBbgI4ABgAvR0zAEsAUWkzAKQBcVYzAIQBQ6IyACgAcg8xACEAsRcxACwBQXoxAB0AwX8xAFYAR2owAIIBSgsvABUAoHEuAMEAopsuADcAbRIqABUA13QpAAA=</Bytes><Format>1769473</Format><Version>1.0.0</Version></Fid>';
    FID_DATA[2] = '<?xml version="1.0" encoding="UTF-8"?><Fid><Bytes>Rk1SACAyMAABdgAz/v8AAAFlAYgAxQDFAQAAAFY5QNcA4YlkgJgApXljQP4Am01dgEoBDIFcQPgATadbgKsAx4FZgGUBBIFZQKYAdA9YQK8ASQ1XQGEBWzNWgPMBLZJUQJkAFBJTQG8AgxpTgL8BNY9TgOcAsWVQQCYA13hOgSMA0ZNNgIYBYzNNQHgAJxdJgOAA041GgE8BSy9FQKUBbpJEgC4BCIE+gCEBGSI6QCwA5H86QEAAXnQ5QDQBC4M5gDAApCA4QCwBGYA4gDIAuCA4QCwBAno4gB8BBiU4QB8AwSA3QDYAXnw3AEQAaRk1ACoA7iI0ADUAchs0ACUAeng0ADYAzh40ABoAzCUzAFUBYZszADQAZxcyADQAvR4yACYAun4wADUAxngwANsAzH8vAC4BSigvAB8Aqn4uACUAxiQuABoA7nYuACwBJyktABgAsXotAHQBckMrAEABUJMqANsBcpYqADsBSC8pABUAvnkpAAA=</Bytes><Format>1769473</Format><Version>1.0.0</Version></Fid>';
    FID_DATA[3] = '<?xml version="1.0" encoding="UTF-8"?><Fid><Bytes>Rk1SACAyMAABEAAz/v8AAAFlAYgAxQDFAQAAAFYoQK8Agw1gQNgA84dfgGUBFX9cgJkAtnhaQP4AYqlaQHcAlRlZQKAAKhFYQEoBWy1XQLMAXQ1XgEgBHH9WQFgAU3ZUgEcAlRtTgLkBRotTQFUBaDFSgGUAIh1RgOEA54tRQIIAPxZQgK0A2H9QgO0BR4xPgP8ArU9OgOsAw2NLgIQAERtJgCUAnHdGQFcAXR4/gDYBWDM+gCEApB4+QCUAhXY7gB8Aih06QCwAjn04QOIA05Q3gN0A3n42ADABU481ABoA8x80ACAAqiIzABgAsSYvAB0A6HYuACEBKX4uAOIAzZ8rAB8Ath4qABkApXopAAA=</Bytes><Format>1769473</Format><Version>1.0.0</Version></Fid>';
    FID_DATA[4] = '<?xml version="1.0" encoding="UTF-8"?><Fid><Bytes>Rk1SACAyMAABuAAz/v8AAAFlAYgAxQDFAQAAAFZEgNoBTj9dgLMBZERdQKsBSjhaQDoA13hZQH0BMjVYQFsBGX5XQLwAkKdXQQwBFJhWgNcAe1FVgGUAznxVgFYAXRdUQKoAoGBUQHwBJY1UgKMAPwJTQOgAi55QgDABIXpQQGgBFYdPQIMBSjlPQGcAfhFOQKAAT2lNQJgAWgRJQHEBSDVIgJgBZ0RIgEoAfRdGQKkA74xEQCgAzHdCQDYA3XVBQDIAq3U/QJkBCYw/QHwBTUA/QEoAgRc+gKYA4pA+gGcBSkg+gEYBVoM+QO0BN0A9QIIBLDE8QB8Aih87QC4AdHU6gCMBOyc6QCkBSoE6gBoAzCs5QCgAdBk4gDcAkCA4QCoAm3U4QCwAino3gEQBMiA3gGcBRkU3gCEAwSA3gKYBCC02QKkA0Zs2AB0AqoU1ALEA8pA1AB0A/h81AKUBFpI1ADYAtCI0ACYBKXszACMBCHozAD8Al34yADsAxhsyACkA0hwyAC4BJyUyAEgBNSAyAE8BRxswAC4AtnMvADQBS4AtANoBakotAEEBIXorAK8BBJIrAAA=</Bytes><Format>1769473</Format><Version>1.0.0</Version></Fid>';
    // let fmt = "11"; 115246 1562
    // console.log(fmt);// 434
    let result_FID = [];
    for(x = 0; x < FID_DATA.length; x++){
        parseString(FID_DATA[x], {trim: true},function (err, result) { 
            // console.log("\n\n\n",result.Fid.Bytes[0])
            result_FID[x] = result.Fid.Bytes[0];
        });    
    }
    // console.log(result_FID); 
    // let loop = setInterval(() => {        
        // biometric.startVerify({fmt: result_FID},20000,function(status,fimage){
        //     console.log("\ntest status startEnroll ",status);
        //     console.log("\ntest status startEnroll image",fimage);
        // });
        biometric.startVerifyMulti({fmt: result_FID},5000,function(f1,f2,f3,f4,f5){
            console.log("\ntest status very 2",f1,f2,f3,f4,f5);
            // callback(f1,f2,f3,f4,f5);
        }); 
    // }, 2000);
    
    // setTimeout(()=>{
    //     console.log("\nTimeout 1 ");
    //     biometric.stopVerify(inits,function(){            
    //         console.log("\nStopTimeout"); 
    //     });   
    // },5000);   
  
}
 
