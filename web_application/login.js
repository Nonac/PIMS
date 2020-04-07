(function () {
    let usernameEle = document.getElementById('username');
    let passwordEle = document.getElementById('user_pass');
    let checkEle = document.getElementById('check')
    let check = localStorage.getItem("check");
    console.log(check);
    if (check === 'true') {
        console.log(typeof check);
        usernameEle.value = localStorage.getItem('username');
        passwordEle.value = localStorage.getItem('password');
        checkEle.checked = true;
    }
}) ();

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
            let username = document.getElementById('username');
            let password = document.getElementById('user_pass');
            if (!checkUserInput(username, password)) {
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

function checkUserInput(username, password) {
    let errInfo;
    let check = document.getElementById('check').checked;
    if (!username.value) {
        errInfo = 'Username cannot be empty.';
    }
    else if (!password.value) {
        errInfo = 'Password cannot be empty.'
    }
    else {
        localStorage.setItem("username", username.value);
        localStorage.setItem("password", password.value);
        localStorage.setItem("check", check);
        console.log(localStorage.getItem('check'));
        return true;
    }
    alert(errInfo);
    return false;
}



