// called when a message arrives
// Only two pages need the functions like this, no need to implement a factory function.
client.onMessageArrived = function (message) {
    console.log("onMessageArrived:"+message.payloadString);
    let respondObj = JSON.parse(message.payloadString);
    let username = document.getElementById('username').value;
    if (respondObj['data_type'] === 'web_register' && username === respondObj.info['username']) {
        if (respondObj.info['status'] === 1 ) {
            setCookie('username', respondObj.info['user']);
            window.location.href = 'user_account.html';
        }
        if (respondObj.info['status'] === 0) {
            alert('Wrong username or password!');
        }
    }
};

let button = document.getElementById('register_button');
button.onclick = function () {
    client.subscribe(topicName, {});
    let username = document.getElementById('username').value;
    let password = document.getElementById('user_pass').value;
    if (!checkUserInput(username, password)) {
        return;
    }
    let messageBody = {
            username: username,
            password: password,
            status: 2
        };
    let message = buildMessage('web_register', messageBody);
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