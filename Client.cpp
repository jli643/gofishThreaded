#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include "Blockable.h"
//#include "shared.h"
#include "socket.h"

using namespace Communication;
int main(void) 
{
    const int port = 2000;
    std::string ip = "127.0.0.1";
    Communication::Socket socket(ip,port);
    
    socket.Open();
    
    std::string input = "";
    
    while(input != "n")
    {
      std::cout<< "Are you ready to start Gofish?"<<std::endl;
      std::cout << "Type 'y' to start, 'n' to quit: " <<std::endl;
      std::cin >> input;
      
    }
    std::cout << "Sleep now baby" <<std::endl;
    socket.Close();
    sleep(1);
    
    return 0;
}
