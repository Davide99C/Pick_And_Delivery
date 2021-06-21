#include <unistd.h>
#include <iostream>
#include <fstream>
#include <map>
#include <list>
#include <string>
#include <signal.h>
#include <sstream>
#include <libwebsockets.h>
#include <set>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include "header.h"


////////////////////////////////////////////////////////////////////////////////////////
#define MYFIFO "my_fifo" 

int count=0; // Variabile globale per la verifica dello scaricamento del Robot 

using namespace std;

typedef struct WSContent{
  int port;
  const char *interface;

}WSContent;

static int callback_http(
    struct lws *wsi,
    enum lws_callback_reasons reason,
    void *user,
    void *in,
    size_t len
) {
  switch (reason) {

    case LWS_CALLBACK_ESTABLISHED:
      printf("connection established\n");
      break;

    case LWS_CALLBACK_RECEIVE: {

      break;
    }
    default:
      break;
  }

  return 0;
}

static int callback_dumb_increment( //struct libwebsocket_context * this_context,
                                  struct lws *wsi,
                                  enum lws_callback_reasons reason,
                                  void *user, void *in, size_t len) {

  switch (reason) {
  case LWS_CALLBACK_ESTABLISHED:
    printf("connection established\n");

    break;

  case LWS_CALLBACK_RECEIVE: {

      //GESTISCO LA RISPOSTA DA PARTE DELL'UTENTE
      char *buf = (char*) malloc((sizeof(char)*len)+1);
      strncpy(buf,(char*)in,len);
      float *coordinate;
      int size;

      //VERIFICO SE LA STRINGA È RELATIVA AL BOTTONE "CHIAMA ROBOT" O MENO
      if (len>13) {
        // stringa = chiamata:stanzarobot. --> split della stringa:
        std::string s = (string)buf;
        std::string token = s.substr(0, s.find(":")); // token is stanza             <-- più efficace rispetto a strtok
        size = token.length();
        strcpy(buf, token.c_str());
        // char* token = strtok(buf,":"); // token is "chiamata"                     <--
        // char* token2 = strtok(NULL,":"); // token2 is stanza                      <-- meno efficace rispetto a substr
        // size = strlen(token2);
        // strcpy(buf, token2);
        buf[size] = '\0';
        cout << "chiamata" << " --> " << buf << endl;
        lws_write(wsi, (unsigned char*) buf, size, LWS_WRITE_TEXT);
      }
      else {
        buf[len] = '\0';
        size = strlen(buf);
        lws_write(wsi, (unsigned char*) buf, size, LWS_WRITE_TEXT);
      }

      //COMUNICAZIONE TRAMITE NAMED PIPE PER LO SCAMBIO DEL MESSAGGIO (INVIO COORDINATE AL ROBOT)
      int i;
      int fd = open(MYFIFO,O_WRONLY);

      if (!fd) {
        cerr << "Errore apertura in scrittura della fifo" << endl;
        return EXIT_FAILURE;
      }
      //prendo le coordinate da inviare al robot
      coordinate=getStanza((string)buf); 

      cout << "INVIO COORDINATE AL ROBOT: " << coordinate[0] << " "
      << coordinate[1] << " " << coordinate[2] << " " << endl;
      for (i=0;i<3;i++) {
        if(write(fd,&coordinate[i],sizeof(float))==-1) {
          cerr << "Errore di scrittura nella fifo" << endl;
          return EXIT_FAILURE;
        }
      }
      count++; 
      // Controllo scaricamento del Robot (utile solo al fine di modificare l'interfaccia utente, non incide sul movimento del Robot)
      if (count==3) {
        string s = "scarico";
        char *scarico = (char*) malloc((sizeof(char)*8));
        strcpy(scarico, s.c_str());
        size = strlen(scarico);
        scarico[size] = '\0';
        size = strlen(scarico);
        lws_write(wsi, (unsigned char*) scarico, size, LWS_WRITE_TEXT);
        count = 0;
      }
    
      close(fd);
        

    }
    break;


  default:
    break;
  }

  return 0;
}



int webServerCreate(){
  int port = 9002;
  int opts = 0;
  const char *interface = NULL;
  struct lws_context *context;
  struct lws_context_creation_info info;

  //CREZIONE FIFO
  int res;
  unlink(MYFIFO);
  res = mkfifo(MYFIFO,0666);
  if (res==-1) {
    cerr << "Errore creazione fifo" << endl;
    return EXIT_FAILURE;

  }

  memset(&info, 0, sizeof info);

  struct lws_protocols protocols[] = {
    // first protocol must always be HTTP handler 
    {
      .name="http-only",   // name
      .callback=callback_http, // callback
      .per_session_data_size=0,              // per_session_data_size
      .rx_buffer_size=0
    },
    {
      .name = "web-protocol", // protocol name - molto importante!
      .callback = callback_dumb_increment,   // callback
      .per_session_data_size=0,                          // we don't use any per session data
      .rx_buffer_size=0
    },
    {
      .name=NULL,
        .callback=NULL,
        .per_session_data_size=0,   // End of list 
        .rx_buffer_size=0
    }
  };

  info.port = port;
  info.iface = interface;
  info.protocols = protocols;
  //info.extensions = lws_get_internal_extensions();
  //if (!use_ssl) {
  info.ssl_cert_filepath = NULL;
  info.ssl_private_key_filepath = NULL;
  //} else {
  //  info.ssl_cert_filepath = LOCAL_RESOURCE_PATH"/libwebsockets-test-server.pem";
  //  info.ssl_private_key_filepath = LOCAL_RESOURCE_PATH"/libwebsockets-test-server.key.pem";
  //}
  info.gid = -1;
  info.uid = -1;
  info.options = opts;



  // create libwebsocket context representing this server
  context = lws_create_context(&info);

  if (context == NULL) {
    fprintf(stderr, "libwebsocket init failed\n");
    return -1;
  }

  printf("starting server...\n");

  // infinite loop, to end this server send SIGTERM. (CTRL+C)
  while (1) {
    lws_service(context, 50);
    // libwebsocket_service will process all waiting events with
    // their callback functions and then wait 50 ms.
    // (this is a single threaded web server and this will keep our
    // server from generating load while there are not
    // requests to process)
  }

  lws_context_destroy(context);

  //DISTRUZIONE FIFO
  // res = unlink(MYFIFO);
  // if (res==-1) {
  //   cerr << "Errore unlink fifo" << endl;
  //   return EXIT_FAILURE;
  // }

  return 0;

}
