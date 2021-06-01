#include <iostream>
#include <fstream>
//#include "proc_shell.h"
#include "websocket.h"
#include <stdlib.h>
#include <unistd.h>

/*
void cleanShutdown() {
  std::cerr << "clean shutdown" << std::endl;
  WebServer_stop();
  //ProcShell::stop();
}


int main(int argc, char** argv) {
  std::ifstream is(argv[1]);
  
  std::cerr << "startnig process monitor: " << std::endl;
  //ProcShell::start();
  std::cerr << "done " << std::endl;

  int16_t port=9002;
  if (argc>2) {
    port=atoi(argv[2]);
  }
    
  WebServer_start(port, ".");
  atexit(cleanShutdown);

  while (1) {
    pause();
    char buf[1024];
    std::cerr << ">";
    std::cin.getline(buf,1024);
    ProcShell::parseCommand(buf);
    
  }
}*/

int main(int argc, char const *argv[]){


  if(webServerCreate() == -1){
    fprintf(stderr, "Errore nella socket\n");
    return EXIT_FAILURE;
  };


  return EXIT_SUCCESS;
}
