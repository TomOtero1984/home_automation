const express = require('express');
const bodyParser = require('body-parser');
const axios = require('axios');

const util = require('util');
const execFile = util.promisify(require('child_process').execFile);

var app = express()
var port = 3000;

app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());

const mongo_url = "https://us-west-2.aws.webhooks.mongodb-realm.com/api/client/v2.0/app/application-0-jqcqf/service/google_assistant_hook2/incoming_webhook/webhook2"

async function getVersion(m_name,res) {
//curl -X PUT -H "Content-Type: application/json" -d '{"name":"RPi","status":"running"}' "https://us-west-2.aws.webhooks.mongodb-realm.com/api/client/v2.0/app/application-0-jqcqf/service/google_assistant_hook2/incoming_webhook/webhook2"

    const { stdout } = await execFile('node', ['--version']);
    console.log(stdout);
    return stdout;
}

app.post('/', function(req, res) {
    console.log('receiving data ...');
    console.log('body is ',req.body);
    m_name = req.body["name"];
    m_status = req.body["status"];
    if (m_name == 'RPi' && m_status == 'running'){
        getVersion(m_name,res)
        const msg = '{"msg": "recieved"}'
        res.json(msg)
        mongo_res = `{"name":"${m_name}","status":"ready"}`
        console.log(mongo_res);
        axios.put(mongo_url, mongo_res)
    }
});

app.get('/node-version', async function(req, res) {
    const node_version = await getVersion();
    res.send(`{node = ${node_version}}`);
});

app.listen(port);
console.log('Server started! At http://localhost:' + port);
