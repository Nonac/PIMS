if (!getCookie('username')) {
    alert('Please log in first.');
    window.location.href = 'index.html';
}

let username = getCookie('username');
let nameElement = document.getElementById('username');
nameElement.innerHTML = username;

setCanvas();

function setCanvas() {
    let canvas = $('#echart_canvas');
    let canvasWidth = canvas.width();
    let canvasHeight = canvas.height()
    console.log(canvasWidth);
    canvas.css('width', canvasWidth).css('height', canvasHeight);

}

client.connect({onSuccess: function () {
    let addVehicleButton = document.getElementById('add_vehicle_button');
    let rechargeButton = document.getElementById('submit_pay_button');
    client.subscribe(topicName, {});
    sendQuery('web_finance', {username: username, status: 2});
    sendQuery('web_vehicle_query', {username: username, status: 2});
    addVehicleButton.onclick = registerVehicle;
    rechargeButton.onclick = recharge;
    }
});

client.onMessageArrived = function (message) {
    console.log("onMessageArrived:"+message.payloadString);
    let respondObj = JSON.parse(message.payloadString);
    if (respondObj.info.username === username && respondObj.info.status === 1) {
        let message_type = respondObj.data_type;
        switch (message_type) {
            case 'web_finance': renderUserBalance(respondObj); break;
            case 'web_recharge': renderRecharge(respondObj); break;
            case 'web_vehicle_query': renderVehicleList(respondObj); break;
            case 'web_vehicle_register': renderVehicleRegister(); break;
            case 'web_vehicle_history': renderChart(respondObj); break;
            default: return;
        }
    }
    if (respondObj.info.username === username && respondObj.info.status === 0) {
        alert('Back-end rejected your request.');
    }
};

function renderChart(respondObj) {
    let myChart = echarts.init(document.getElementById('echart_canvas'), 'light');
    let info = respondObj.info;
    let nowDate = new Date();
    let option = {
        title: {
            text: 'Parking time in Last 7 Days',
            fontSize: '12',
            top: 0,
            textStyle: {
                color: 'white'
            }
        },
        tooltip: {},
        legend: {
            data: ['hrs'],
            top: 25,
            right: 0,
            textStyle: {
                color: 'white'
            }
        },
        xAxis: {data:[]},
        yAxis: {},
        series: [{name: 'hrs', type: 'line', data: []}],
        textStyle: {
            color: 'white'
        }
    }
    for (let i in info) {
        if (parseInt(i) || parseInt(i) === 0) {
            let tempDate = new Date(nowDate.getTime() - (parseInt(i) * 24 * 60 * 60 * 1000));
            console.log(tempDate);
            option.xAxis.data[6 - parseInt(i)] = tempDate.getDate() + '/' + (tempDate.getMonth() + 1);
            option.series[0].data[6 - parseInt(i)] = info[i];
        }
    }
    myChart.setOption(option);
}

function renderRecharge(respondObj) {
    sendQuery('web_finance', {username: username, status: 2});
    closeModal('top_up_modal');
    showSuccess('You have successfully recharged.');
}

function closeModal(modalId) {
    let closeButton = '#' + modalId + ' .close';
    $(closeButton).click();
}

function recharge() {
    let card = document.getElementById('card_number').value;
    let amount = document.getElementById('pay_amount').value;
    if (card && amount) {
        let messageBody = {
            username: username,
            card_number: card,
            pay_amount: amount,
            status: 2
        };
        sendQuery('web_recharge', messageBody);
    }
    else {
        alert('Please input the full payment method');
    }
}

function renderVehicleRegister() {
    let vehicleListEle = document.getElementById('vehicle_list');
    vehicleListEle.innerHTML = '';
    sendQuery('web_vehicle_query', {username: username, status: 2});
    closeModal('add_vehicle_modal');
    showSuccess('You have registered a new vehicle.');
}

function registerVehicle() {
    let addVehicleForm = document.getElementById('add_vehicle_form');
    let vehicleId = document.getElementById('vehicle_id').value;
    let vehicleType = document.getElementById('vehicle_type').value;
    let blueTooth = document.getElementById('blue_tooth').value;
    if (vehicleId && vehicleType) {
        let messageBody = {
            username: username,
            vehicle_id: vehicleId,
            vehicle_type: vehicleType,
            bluetooth_address: blueTooth,
            status: 2
        }
        sendQuery('web_vehicle_register', messageBody);
    }
    else {
        alert('Please complete the information of your vehicle');
    }
}

function renderVehicleList(respondObj) {
    let vehicleListEle = document.getElementById('vehicle_list');
    let vehicleArr = respondObj.info.vehicle_list;
    vehicleListEle.innerHTML = '';
    for (let i in vehicleArr) {
        let newLi = document.createElement('li');
        newLi.setAttribute('class', 'list-group-item list-group-item-light');
        newLi.innerHTML = vehicleArr[i].vehicle_type + ':&nbsp' + vehicleArr[i].vehicle_id;
        vehicleListEle.appendChild(newLi);
    }
    if (vehicleListEle.children.length > 0) {
        let firstChild = vehicleListEle.children[0];
        firstChild.setAttribute('class', firstChild.getAttribute('class') + ' active');
        setClick($('#vehicle_list'));
        sendHistoryQuery(firstChild.innerHTML.split(':')[1].trim().split(';')[1]);
    }
}

function setClick(list) {
    list.children().click(function(){
        if($(this).hasClass('active')){
        }else{
            $(this).addClass('active');
            sendHistoryQuery($(this).text().split(':')[1].trim());
            console.log('success');
        }
        if($(this).siblings().hasClass('active')){
            $(this).siblings().removeClass('active');
        }
    })
}

function sendHistoryQuery(vehicleId) {
    let messageBody = {
        username: username,
        vehicleId: vehicleId,
        status: 2
    };
    sendQuery('web_vehicle_history', messageBody);
}

function renderUserBalance(respondObj) {
    let balanceElement = document.getElementById('balance');
    let balance = respondObj.info.balance + respondObj.info.currency;
    balanceElement.innerHTML = balance;
}

function sendQuery(dataType, messageBody) {
    let message = buildMessage(dataType, messageBody);
    client.send(message);
}

function showSuccess(successInfo) {
    let bodyEle = document.getElementsByTagName('body')[0];
    let newAlert = document.createElement('div');
    newAlert.setAttribute('class', 'alert alert-success alert-dismissible fade show');
    newAlert.setAttribute('id', 'success_info');
    newAlert.innerHTML = '<button type="button" class="close" data-dismiss="alert">&times;</button> ' +
        '<strong>Success!</strong><span class="success-info">' + successInfo + '</span>';
    bodyEle.insertBefore(newAlert, bodyEle.children[0]);
    setTimeout(function() {
        $('#success_info .close').click();
    }, 1000);
}