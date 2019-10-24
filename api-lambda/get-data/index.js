var AWS = require('aws-sdk');
var moment = require('moment');
var ddb = new AWS.DynamoDB.DocumentClient({region: 'us-east-1'});

exports.handler = (event, context, callback) => {

    var params = {
        TableName: process.env.TABLE_NAME,
        ScanIndexForward: false
    };

    ddb.scan(params, (err, data) => {
        if (err){
            callback(err, null);
        } else {
            var sortedData = [];
            sortedData = data.Items;
            var posSortingR1 = 0;
            var posSortingR2 = 0;
            var tempJson = {};
            var timeDiffVar = 0;
            var vueltas = 0;
            while (posSortingR1 <= data.Count - 1) {
                posSortingR2 = 0;
                while ( posSortingR2 <= data.Count - 1) {
                    if (sortedData[posSortingR1]['modified-date'] < sortedData[posSortingR2]['modified-date']) {
                        tempJson = sortedData[posSortingR1];
                        sortedData[posSortingR1] = sortedData[posSortingR2];
                        sortedData[posSortingR2] = tempJson;
                    }
                    posSortingR2++;
                }
                posSortingR1++;
            }
            console.log(sortedData);
            var actualRow = 0;
            while (actualRow < sortedData.length) {
                vueltas = vueltas + parseInt(sortedData[actualRow]['dato']);
                actualRow++;
            }
            if (data.Count >= 1) {
                var primeraMedicion = moment(sortedData[0]['modified-date']);
                var ultimaMedicion = moment(sortedData[sortedData.length - 1]['modified-date']).diff(primeraMedicion, 'seconds', true);
                console.log(timeDiffVar);
            } else {
                ultimaMedicion = 0;
            }
            callback(null, {
                vueltas: vueltas,
                timeDiff: ultimaMedicion,
                info: 'data',
                statusCode: 200
            });
        }
    });
};
