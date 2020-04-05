let topicName = 'PIMS';

client = new Paho.MQTT.Client("broker.mqttdashboard.com", 8000, make_id());

client.onConnectionLost = onConnectionLost;
client.onMessageArrived = onMessageArrived;

client.connect(    {onSuccess: onConnect});

// called when the client connects
function onConnect() {
    client.subscribe(topicName, {});
    console.log("Connected");
}

function onMessageArrived(message) {
    console.log(JSON.parse(message.payloadString));
}

// called when the client loses its connection
function onConnectionLost(responseObject) {
    if (responseObject.errorCode !== 0) {
        console.log("onConnectionLost:"+responseObject.errorMessage);
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