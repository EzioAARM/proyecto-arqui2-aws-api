var AWS = require('aws-sdk');
var moment = require('moment');
exports.handler = async (event) => {
    AWS.config.update({region: 'us-east-1'});
    var ddb = new AWS.DynamoDB.DocumentClient({apiVersion: '2012-08-10'});
    JSON.stringify(event, null, 2);
    console.log(event);
    var params = {
        Item: {
        "modified-date": moment().unix(),
        "dato": event.queryStringParameters.dato
        },
        TableName: process.env.TABLE_NAME
    };
    console.log(params);
    let putItem = new Promise((res, rej) => {
        ddb.put(params, function(err, data) {
            if (err) {
                rej(err);
            } else {
                res({
                    statusCode: 200
                });
            }
        }); 
    });

    const result = await putItem; 
    return result;
};
