// called when a message arrives
client.onMessageArrived = function (message) {
    console.log("onMessageArrived:"+message.payloadString);
    let respondObj = JSON.parse(message.payloadString);
    if (respondObj['data_type'] === 'web_login') {
        if (respondObj.login_info['access'] === 1 ) {
            setCookie('username', respondObj.login_info['user']);
            window.location.href = 'user_account.html';
            console.log(getCookie('username'));
        }
        if (respondObj.login_info['access'] === 0) {
            alert('Wrong username or password!');
        }
    }
};

//submit user information when the button is clicked
let button = document.getElementById('login_button');
button.onclick = function () {
    let username = document.getElementById('username').value;
    let password = document.getElementById('user_pass').value;
    let dataObj = {
        data_type: 'web_login',
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

