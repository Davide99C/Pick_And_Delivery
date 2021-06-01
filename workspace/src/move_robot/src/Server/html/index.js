
//var ws = new WebSocket('ws://'+self.location.host+'/','web-protocol');
var ws = new WebSocket('ws://127.0.0.1:9002/','web-protocol');

//QUI RICEVE LA RISPOSTA DA CPP
ws.onmessage = function(event) {
   var risposta = event.data;
   document.getElementById("titolo1").innerHTML="Hai effettuato l'accesso nella stanza: "+risposta;
   document.getElementById("titolo2").innerHTML="Inizia subito a spedire tramite il robot! ";
   document.getElementById("form-id").style.display="none";
}

//QUI INVIA LA STANZA AL CPP
function sendPacket(packet) {
    ws.send(packet);
}

function EventHandlers() {
    var check;
    check = document.getElementsByName("stanza");
    for (var i=0; i<check.length;i++) {
        if (check[i].checked) {
            console.log(check[i].value);
            sendPacket(check[i].value);
        }
    }
}