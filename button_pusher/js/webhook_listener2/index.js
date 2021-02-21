var express = require('express')
var bodyParser = require('body-parser')

var app = express()
var port = 3000;

app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());

// app.post('/', function (req, res) {
//     var body = req.body;
//     var trackingNumber = body.msg.tracking_number;
//     var slug = body.msg.slug;
//     var token = body.msg.unique_token;

//     console.log(trackingNumber, slug, token);

//     res.json({
//         message: 'ok got it!'
//     });
// });

app.post('/', function(req, res) {
    console.log('receiving data ...');
    console.log('body is ',req.body);
    res.send(req.body + req.body);
});

// var server = app.listen(port, function () {

//     var host = server.address().address
//     var port = server.address().port

//     console.log('Example app listening at http://%s:%s', host, port)

// });

app.listen(port);
console.log('Server started! At http://localhost:' + port);
