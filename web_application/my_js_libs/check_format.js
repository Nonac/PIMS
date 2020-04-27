function checkUsernameFormat(str) {
    if (str.match(/[^\w_]/g)) {
        return false;
    }
    else {
        return true;
    }
}

function checkPasswordFormat(str) {
    if (str.match(/\s/g)) {
        return false;
    }
    else {
        return true;
    }
}