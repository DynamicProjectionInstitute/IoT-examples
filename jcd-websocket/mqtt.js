var mqtt;
var reconnectTimeout = 2000;

 function MQTTconnect() {
     path = '/mqtt';
     mqtt = new Paho.MQTT.Client(host,port,path,"web_" + parseInt(Math.random() * 100, 10));
     var options = {
         timeout: 3,
         useSSL: useTLS,
         cleanSession: cleansession,
         onSuccess: onConnect,
         onFailure: function (message) {
             console.log("Connection failed: " + message.errorMessage + "Retrying");
             $("#status").append("Connection failed!<br>\n");
             setTimeout(MQTTconnect, reconnectTimeout);
         }
     };

     mqtt.onConnectionLost = onConnectionLost;
     mqtt.onMessageArrived = onMessageArrived;
     if (username != null) {
         options.userName = username;
         options.password = password;
     }
     console.log("Host="+ host + ", port=" + port + ", path=" + path + " TLS = " + useTLS + " username=" + username + " password=" + password);
     $("#status").append("Host="+ host + ", port=" + port + ", path=" + path + " TLS = " + useTLS + " username=" + username + " password=" + password+"<br>\n");
     mqtt.connect(options);
 }

 function onConnect() {
     console.log('Connected to ' + host + ':' + port + path);
     mqtt.subscribe(topic, {qos: 0});
     console.log("Sub: "+topic);
     $("#status").append("Connected....<br>\n");
 }

 function onConnectionLost(response) {
     setTimeout(MQTTconnect, reconnectTimeout);
     console.log("connection lost: " + responseObject.errorMessage + ". Reconnecting");
     $("#status").append("Connection lost...reconnecting<br>\n");
 };

 function onMessageArrived(message) {
     var topic = message.destinationName;
     var payload = message.payloadString;
 };


function messageSend(topic,message,ret=true) {
var d = new Date();
  $("#status").append("Sending: "+topic+" "+message+" ["+ d+"]<br>\n");
  msg=new Paho.MQTT.Message(message);
  msg.destinationName = topic; 
  msg.retained=ret;
  msg.qos=0;
  mqtt.send(msg);
  console.log(msg.destinationName+" "+message);
}
