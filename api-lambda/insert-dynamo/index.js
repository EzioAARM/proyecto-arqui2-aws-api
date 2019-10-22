const AWS = require('aws-sdk');
const moment = require('moment');
const ddb = new AWS.DynamoDB.DocumentClient({
    apiVersion: '2012-08-10'
});
const dynamoTableName = process.env.TABLE_NAME;

exports.handler = (event, context, callback) => {
    let connectionData;
    
    try {
        connectionData = await ddb.scan({ 
            TableName: TABLE_NAME, 
            ProjectionExpression: 'connectionId' })
            .promise();
    } catch (e) {
        return { 
            statusCode: 500, 
            body: e.stack 
        };
    }

    const apigwManagementApi = new AWS.ApiGatewayManagementApi({
        apiVersion: '2018-11-29',
        endpoint: event.requestContext.domainName + '/' + event.requestContext.stage
    });

    const postData = JSON.parse(event.body).data;

    const postCalls = connectionData.Items.map(async ({ connectionId }) => {
        try {
            await apigwManagementApi.postToConnection({ ConnectionId: connectionId, Data: postData }).promise();
            console.log(postData);
        } catch (e) {
            console.log("Se ha perdido la conexión");
        }
    });


    try {
        await Promise.all(postCalls);
    } catch (e) {
        return { 
            statusCode: 500, 
            body: e.stack 
        };
    }

    return { 
        statusCode: 200, 
        body: 'Información enviada' 
    };

    
    /*AWS.config.update({region: 'us-east-1'});
    var params = {
        Item: {
            id: moment().unix(),
            dato: postData.dato
        },
        TableName: 'arqui-lab'
    };
    let putItem = new Promise((res, rej) => {
        ddb.put(params, function(err, data) {
            return result.status(200);
        }); 
    });
    const result = await putItem;
    return result.status(200);*/
};
