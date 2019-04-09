let express = require('express');
let path = require('path');

export function create_app() {
    let app = express();
    app.use(express.static('public'));

    return app;
}