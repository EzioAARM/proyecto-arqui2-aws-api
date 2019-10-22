var AWS = require('aws-sdk');
var ddb = new AWS.DynamoDB.DocumentClient({region: 'us-east-1'});

exports.handler = (event, context, callback) => {

    var params = {
        TableName: process.env.TABLE_NAME
    };

    ddb.scan(params, (err, data) => {
        if (err){
            callback(err, null);
        } else {
            callback(null, data);
        }
    });
};
