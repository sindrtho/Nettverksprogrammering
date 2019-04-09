let https = require('https');
let http = require('http');
let fs = require('fs');
let express = require('express');

let app = express();
app.use(express.static('public'));

let key = fs.readFileSync('sslcert/server.key', 'utf8');
let cert = fs.readFileSync('sslcert/server.crt', 'utf8');

let credentials = {key, cert};
let http_app = express();
http_app.get('*', function(req, res) {

    return res.redirect('https://' + req.headers.host + req.url)
});
let httpserver = http.createServer(http_app);
let httpsserver = https.createServer(credentials, app);
httpsserver.listen(443);
console.log("Listening securely on port", 443);
httpserver.listen(80);
console.log("listening on port", 80);