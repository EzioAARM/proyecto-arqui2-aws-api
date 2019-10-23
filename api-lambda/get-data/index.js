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
            var sessionsGroups = [];
            var actualSession = [];
            var actualRow = 0;
            while (actualRow <= sortedData.length - 1) {
                if (actualRow == sortedData.length - 1) {
                    actualSession.push(sortedData[actualRow]);
                    sessionsGroups.push(actualSession);
                } else {
                    var firstDate = moment(sortedData[actualRow]['modified-date']);
                    var secondDate = moment(sortedData[actualRow + 1]['modified-date']);
                    var dateDiff = secondDate.diff(firstDate, 'seconds', true);
                    if (dateDiff < 5) {
                        actualSession.push(sortedData[actualRow]);
                        
                    } else {
                        sessionsGroups.push(actualSession);
                        actualSession = [];
                    }
                }
                actualRow++;
            }
            callback(null, {
                sessions: sessionsGroups,
                groups: sessionsGroups.length
            });
        }
    });
};
