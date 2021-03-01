const express = require('express');
const bodyParser = require('body-parser');
const axios = require('axios');
const util = require('util');



async function getVersion(m_name,res) {
//curl -X PUT -H "Content-Type: application/json" -d '{"name":"RPi","status":"running"}' "https://us-west-2.aws.webhooks.mongodb-realm.com/api/client/v2.0/app/application-0-jqcqf/service/google_assistant_hook2/incoming_webhook/webhook2"

    const { stdout } = await execFile('node', ['--version']);
    console.log(stdout);
    return stdout;
}


class Arduino {
    constructor(path){
        this.path = path;
    }

    // Methods
    async device_lookup() {
        try {
            const { stdout, stderr } = await exec('$HOME/device_lookup');
            if (stderr) {
                console.error(`stderr: ${stderr}`);
            }
            if (stdout.includes("Arduino")) {
                // console.debug(`stdout: ${stdout}`);
                let json_out = JSON.parse(stdout);
                arduino.path = json_out['path'];
            }
            else {
                arduino.path = "null";
                throw "Could not find path to Arduino"
            }
            console.log(`arduino.path1: ${arduino.path}`);
            return
        } 
        catch (err) {
            console.log(err);
            return
        }
    }
}

// child_process Setup
const execFile = util.promisify(require('child_process').execFile);
const exec = util.promisify(require('child_process').exec);

// Find Arduino Port 
let arduino = new Arduino(path="");
arduino.device_lookup();

// SerialPort Setup
const serialport = new SerialPort(arduino.path)

// Express Setup
var app = express()
var port = 3000;
app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());
const mongo_url = "https://us-west-2.aws.webhooks.mongodb-realm.com/api/client/v2.0/app/application-0-jqcqf/service/google_assistant_hook2/incoming_webhook/webhook2"
// Routes
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
// Starting Server
app.listen(port);
console.log('Server started! At http://localhost:' + port);
