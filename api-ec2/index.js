var express = require('express');
var app = express();
const request = require('request');

app.get('/', function (req, res) {
  var dato = req.query.dato;

  request('https://poywhb3qv6.execute-api.us-east-1.amazonaws.com/testing/modulo-esp?dato=' + dato, { json: true }, (err, res, body) => {
    if (err) { return console.log(err); }
    console.log(body.url);
    console.log(body.explanation);
  });

  res.status(200).send();
});

app.listen(3000, function () {
  console.log('Example app listening on port 3000!');
});