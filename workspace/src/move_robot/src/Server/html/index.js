
//var ws = new WebSocket('ws://'+self.location.host+'/','web-protocol');
var ws = new WebSocket('ws://127.0.0.1:9002/','web-protocol');

//QUI RICEVE LA RISPOSTA DA CPP
ws.onmessage = function(event) {
   var risposta = event.data;
   if (risposta=="chiamata") return attesaRobot();
   if (document.getElementById("consegna").style.display=="block") {
        document.getElementById("transito").innerHTML = '<h3> Il pacco è in transito verso la stanza: '+risposta+'</h3>';
   }
   else {
        document.getElementById("titolo1").innerHTML="Hai effettuato l'accesso nella stanza: "+risposta;
        document.getElementById("titolo2").innerHTML="Inizia subito a spedire tramite il robot! ";
        document.getElementById("form-id").style.display="none";
        document.getElementById("main").padding="200px";
        document.getElementById("consegna").style.display="block"; }
}

//QUI INVIA LA STANZA AL CPP
function sendPacket(packet) {
    ws.send(packet);
}

function Accedi() {
    var stanza;
    var cnt=0;
    var giusta;
    stanza = document.getElementsByName("stanza");
    for (var i=0; i<stanza.length;i++) {
        if (stanza[i].checked) {
            cnt+=1;
            if (cnt==1) {
                giusta = stanza[i];
            }
            console.log(stanza[i].value);
        }
    }
    if (cnt==0) {
        alert("Scegliere almeno una stanza in cui fare l'accesso");
        return false;
    }
    if (cnt>1) {
        alert("Scegliere solo una stanza in cui fare l'accesso");
        return false;
    }
    else {
        sendPacket(giusta.value);
    }
}

function Spedisci() {
    validaStanza2();
    var stanza;
    var cnt=0;
    var giusta;
    stanza = document.getElementsByName("stanza2");
    for (var i=0; i<stanza.length;i++) {
        if (stanza[i].checked) {
            cnt+=1;
            if (cnt==1) {
                giusta = stanza[i];
            }
            console.log(stanza[i].value);
        }
    }
    if (cnt==0) {
        alert("Scegliere almeno una stanza in cui spedire il pacco");
        return false;
    }
    if (cnt>1) {
        alert("Scegliere solo una stanza in cui in cui spedire il pacco");
        return false;
    }
    else {
        sendPacket(giusta.value);
    }
}

function validaStanza2() {
    var stanza; var cnt;
    stanza = document.getElementsByName("stanza");
    for (var i=0; i<stanza.length;i++) {
        if (stanza[i].checked) {
            cnt+=1;
        }
    }
    if (cnt==0) {
        alert("Scegliere almeno una stanza in cui fare l'accesso");
        return false;
    }
    if (cnt>1) {
        alert("Scegliere solo una stanza in cui fare l'accesso");
        return false;
    }
    return true;
}

function richiamaRobot() {
    sendPacket("chiamata");
}

function attesaRobot() {
    //setTimeout(() => {
        document.getElementById("attesa").innerHTML = '<h3> Attendi che il robot arrivi o che sia disponibile... <br>Immetti il pacco sul ROBOT e seleziona la destinazione del pacco da spedire </h3>';
      //},5000);
   
}