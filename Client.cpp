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
    while(input != "done")
    {
        ByteArray read;
        socket.Read(read);
        std::string passedString = read.ToString();
        
        if (passedString == "Invalid")
        {
            std::cout <<"you need to pick a card in your hand";
        }
        else if (passedString == "Done")//done
        {
            break;
        }
        else
        {
            std::cout << "Type 'done' to quit: " <<std::endl;
            std::cout << read.ToString() << std::endl;
        }

        
        std::cout << "Pick a card in opponents hand. Card must be in your hand already.\n";

      FlexWait waiter(2,&cinWatcher,&socket);
      Blockable * result = waiter.Wait();
      if (result == &socket)
      {
          //std::cout << "We have likely received a disconnection command" << std::endl;
          
          ByteArray dummy;
          if (socket.Read(dummy) <= 0)
              std::cout << "The socket has been closed suddenly" << std::endl;
          else
          {
              input = dummy.ToString();
              std::cout << "Received: " << input << " from master" << std::endl;
              
              std::cin >> input;
              ByteArray ba(input);
                int written = socket.Write(ba);

                if (written != ba.v.size())
                {
                    std::cout << "The socket appears to have been closed suddenly" << std::endl;
                    break;
                }
                else
                {
                    std::cout << "Sent: " << ba.ToString() << std::endl;
                }
              
          }
          
      }
      else if(result == &cinWatcher)
      {
          std::cin >> input;
          ByteArray ba(input);
          int written = socket.Write(ba);
          
          if (written != ba.v.size())
          {
              std::cout << "The socket appears to have been closed suddenly" << std::endl;
              break;
          }
          else
          {
              std::cout << "Sent: " << ba.ToString() << std::endl;
          }
      }
    }
    std::cout << "Sleep now baby" <<std::endl;
    socket.Close();
    sleep(1);
    
    return 0;
}
