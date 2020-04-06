// called when a message arrives
client.onMessageArrived = function (message) {
    console.log("onMessageArrived:"+message.payloadString);
    let respondObj = JSON.parse(message.payloadString);
    let username = document.getElementById('username').value;
    if (respondObj['data_type'] === 'web_login' && username === respondObj.info['username']) {
        if (respondObj.info['status'] === 1 ) {
            setCookie('username', respondObj.info['username']);
            window.location.href = 'user_account.html';
        }
        if (respondObj.info['status'] === 0) {
            alert('Wrong username or password!');
        }
    }
};

client.connect({onSuccess: function() {
        //submit user information when the button is clicked
        let button = document.getElementById('login_button');
        button.onclick = function () {
            client.subscribe(topicName, {});
            let username = document.getElementById('username').value;
            let password = document.getElementById('user_pass').value;
            if (!username || !password) {
                alert("Username or password cannot be empty.");
                return;
            }
            let messageBody = {
                username: username,
                password: password,
                status: 2
            };
            let message = buildMessage('web_login', messageBody);
            client.send(message);
        };
    }
});



