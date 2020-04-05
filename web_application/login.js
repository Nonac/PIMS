let topicName = 'PIMS';

client = new Paho.MQTT.Client("broker.mqttdashboard.com", 8000, make_id());

client.onConnectionLost = onConnectionLost;
client.onMessageArrived = onMessageArrived;

client.connect(    {onSuccess: onConnect});

//submit user information when the button is clicked
let button = document.getElementById('login_button');
button.onclick = function () {
    console.log('clicked');
    let username = document.getElementById('username').value;
    let password = document.getElementById('user_pass').value;
    let dataObj = {
        data_type: 'web_login',
        user: username,
        pass: password,
        access: 2
    };
    let data = JSON.stringify(dataObj);
    client.subscribe(topicName, {});
    let message = new Paho.MQTT.Message(data);
    message.destinationName = topicName;
    client.send(message);
};

// called when the client connects
function onConnect() {
    // Once a connection has been made report.
    console.log("Connected");
}

// called when the client loses its connection
function onConnectionLost(responseObject) {
    if (responseObject.errorCode !== 0) {
        console.log("onConnectionLost:"+responseObject.errorMessage);
    }
}

// called when a message arrives
function onMessageArrived(message) {
    console.log("onMessageArrived:"+message.payloadString);
    let respondObj = JSON.parse(message.payloadString);
    // console.log(respondObj);
    if (respondObj['data_type'] === 'web_login') {
        if (respondObj['access'] === 1) {
            window.location.href = 'userAccount.html'
        }
        if (respondObj['access'] === 0) {
            alert('Wrong username or password!');
        }
    }
}

// called to generate the IDs
function make_id(length) {
    let result           = '';
    let characters       = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
    let charactersLength = characters.length;
    for ( let i = 0; i < length; i++ ) {
        result += characters.charAt(Math.floor(Math.random() * charactersLength));
    }
    return result;
}