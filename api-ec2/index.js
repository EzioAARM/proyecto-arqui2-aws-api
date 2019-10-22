var express = require('express');
var app = express();
const request = require('request');

app.get('/', function (req, res) {
    request('url api gateway', { json: true }, (err, res, body) => {
        if (err) { return console.log(err); }
    });
    res.status(200).send();
});

app.listen(3000, function () {
  console.log('Listening port 3000');
});