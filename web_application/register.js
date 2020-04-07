// called when a message arrives
// Only two pages need the functions like this, no need to implement a factory function.
client.onMessageArrived = function (message) {
    console.log("onMessageArrived:"+message.payloadString);
    let respondObj = JSON.parse(message.payloadString);
    let username = document.getElementById('username').value;
    if (respondObj['data_type'] === 'web_register' && username === respondObj.info['username']) {
        if (respondObj.info['status'] === 1 ) {
            setCookie('username', respondObj.info['username']);
            window.location.href = 'user_account.html';
        }
        if (respondObj.info['status'] === 0) {
            alert('Wrong username or password!');
        }
    }
};

client.connect(    {onSuccess: function() {
        let button = document.getElementById('register_button');
        button.onclick = function () {
            client.subscribe(topicName, {});
            let username = document.getElementById('username');
            let password = document.getElementById('user_pass');
            let confirmPassword = document.getElementById('user_pass_verify');
            if (!checkUserInput(username, password, confirmPassword)) {
                return;
            }
            let messageBody = {
                username: username.value,
                password: password.value,
                status: 2
            };
            let message = buildMessage('web_register', messageBody);
            client.send(message);
        };
    }
});


function checkUserInput(username, password, verify) {
    let alertInfo;
    if (!username.value) {
        alertInfo = 'Username cannot be empty.';
    }
    else if (!password.value) {
        alertInfo = 'Password cannot be empty.'
    }
    else if (!checkUsernameFormat(username.value)) {
        alertInfo = 'Username can only contain numbers, English letters, and underlines.';
    }
    else if (!checkPasswordFormat(password.value)) {
        alertInfo = 'Password should not contain any blank characters.';
    }
    else if (password.value !== verify.value) {
        alertInfo = 'Passwords you entered are inconsistent.';
    }
    else {
        return true;
    }
    alert(alertInfo);
    return false;
}

// Add an advanced error style.