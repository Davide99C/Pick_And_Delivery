#pragma once
#include <string> 

int webServerCreate(int n);
int reader_table();
float* getStanza(std::string buf);
void stanzaLog(std::string buf);
int connection();
void resetLog();