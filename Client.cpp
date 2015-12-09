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
    std::cout<< "Are you ready to start Gofish?"<<std::endl;
    while(input != "n")
    {
      ByteArray readHand;
      socket.Read(readHand);
      std::cout << "Type 'n' to quit: " <<std::endl;
      std::cout << readHand.ToString() << std::endl;
      std::cout << "Pick a card in opponents hand. Card must be in your hand already.\n";

      FlexWait waiter(2,&cinWatcher,&socket);
      Blockable * result = waiter.Wait();
      if (result == &socket)
      {
          std::cout << "We have likely received a disconnection command" << std::endl;
          ByteArray dummy;
          if (socket.Read(dummy) <= 0)
              std::cout << "The socket has been closed suddenly" << std::endl;
          else
          {
              input = dummy.ToString();
              std::cout << "Received: " << input << " from master" << std::endl;
          }
          break;
      }
      else
      {
          std::cin >> input;
          ByteArray ba(input);
          int written = socket.Write(ba);
          if ( written != ba.v.size())
          {
              std::cout << "The socket appears to have been closed suddenly" << std::endl;
              break;
          }
          else
          {
              std::cout << "Sent: " << input << std::endl;
          }
          if (socket.Read(ba) <=0)
          {
              std::cout << "The socket appears to have been closed suddenly" << std::endl;
              break;
          }
      }
      
    }
    std::cout << "Sleep now baby" <<std::endl;
    socket.Close();
    sleep(1);
    
    return 0;
}
