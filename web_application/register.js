// called when a message arrives
client.onMessageArrived = function (message) {
    console.log("onMessageArrived:"+message.payloadString);
    let respondObj = JSON.parse(message.payloadString);
    if (respondObj['data_type'] === 'web_register') {
        // do something here
    }
};

let button = document.getElementById('register_button');
button.onclick = function () {
    let username = document.getElementById('username').value;
    let password = document.getElementById('user_pass').value;
    if (!checkUserInput(username, password)) {
        return;
    }
    let dataObj = {
        data_type: 'web_register',
        login_info: {
            user: username,
            pass: password,
            access: 2
        }
    };
    let data = JSON.stringify(dataObj);
    let message = new Paho.MQTT.Message(data);
    message.destinationName = topicName;
    client.send(message);
};

function checkUserInput(username, password) {
    if (!username || !password) {
        alert('Username and password cannot be empty.');
        return false;
    }
    else if (!checkUsernameFormat(username)) {
        alert('Username can only contain numbers, English letters, and underlines.');
        return false;
    }
    else if (!checkPasswordFormat(password)) {
        alert('Password should not contain any blank characters.');
        return false;
    }
    else {
        return true;
    }
}