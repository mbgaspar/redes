const hostname = "172.16.158.155";// "172.16.158.155" 
const port = 8081;
const clientId = "webapp" + new Date().getUTCMilliseconds();
var salaSelec;
mqttClient = new Paho.MQTT.Client(hostname, port, clientId);
mqttClient.onMessageArrived =  MessageArrived;
mqttClient.onConnectionLost = ConnectionLost;

function Send(subscription, teste) {
	mqttClient.send(subscription, teste);
	//console.log(teste, subscription);
}

//Initiates a connection to the MQTT broker/
function Connect(username, password, onSuccess, onFailure){
	mqttClient.connect({
		onSuccess: onSuccess,
		onFailure: onFailure,
		keepAliveInterval: 10,
		//userName: username,
		useSSL: true,
		//password: password
	});
}

//Callback for lost connection
function ConnectionLost(res) {
  if (res.errorCode != 0) {
	console.log("Connection lost:" + res.errorMessage);
	mostraLogin({ errorCode: 8 });
  }
}
var lampValue = 0;
//Callback for incoming message processing 
function MessageArrived(message) {
	console.log(message.destinationName +" : " + message.payloadString);
    var topic = message.destinationName.split("/");
    //console.log("Serah: " + topic[1]);
    //document.getElementById(topic[1]).innerHTML = message.payloadString;
	lampValue = message.payloadString;
	var image = document.getElementById('apagada');
	if (message.payloadString === 'HI'){
		image.src = "https://i.ibb.co/tbpsyxz/luz-acesa.png";
	}
	else{
		image.src = "https://i.ibb.co/SRdJMNZ/luz-apagada.png";
	}
}

function changeLamp() {
    if (lampValue === 'HI') {
		Send(salaSelec, 'LO');
    }
    else {
		Send(salaSelec, 'HI');
    }
}

/**
 * Habilita a interatividade com o botão de acender a lampada, escreve o nome da sala no local apropriado
 * e faz a inscrição no tópico da sala.
 * @param {evento DOM de onClick} ev 
 */
function SelecionaSala(ev) {
	const sala= ev.target.id;
	
	if(sala) {
		salaSelec = sala;
		document.getElementById('apagada').disabled = false;
		document.getElementById('sala').innerText = `Sala: ${sala}`;
		mqttClient.subscribe(`${sala}`)
					
	}
}