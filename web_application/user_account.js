if (!getCookie('username')) {
    alert('Please log in first.');
    window.location.href = 'index.html';
}
let username = getCookie('username');
let nameElement = document.getElementById('username');
nameElement.innerHTML = username;

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
        alert('Back-end error.');
    }
};

function renderChart(respondObj) {
    let myChart = echarts.init(document.getElementById('echart_canvas'));
    let info = respondObj.info;
    let nowDate = new Date();
    let option = {
        title: {
            text: 'Parking in Last 7 Days'
        },
        tooltip: {},
        legend: {
            data: ['hrs']
        },
        xAxis: {data:[]},
        yAxis: {},
        series: [{name: 'hrs', type: 'line', data: []}]
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
}

function registerVehicle() {
    let addVehicleForm = document.getElementById('add_vehicle_form');
    let vehicleId = document.getElementById('vehicle_id').value;
    let vehicleType = document.getElementById('vehicle_type').value;
    if (vehicleId && vehicleType) {
        let messageBody = {
            username: username,
            vehicle_id: vehicleId,
            vehicle_type: vehicleType,
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
    for (let i in vehicleArr) {
        let newLi = document.createElement('li');
        newLi.innerHTML = vehicleArr[i].vehicle_id + vehicleArr[i].vehicle_type
        vehicleListEle.appendChild(newLi);
    }
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
