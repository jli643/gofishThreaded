
#include "thread.h"
#include "socketserver.h"
#include <stdlib.h>
#include <time.h>
#include <list>
#include <pthread.h>
#include <vector>
#include <sstream>
#include "hand.h"
#include "deck.h"
#include "card.h"

void initialDraw(Hand &player1, Hand &player2, Deck &gameDeck);
bool askPlayer(Hand &player, std::string input);
bool checkPlayer(std::string request, Hand &player);
bool verify(std::string input, Hand &player);
bool checkForDuplicates(Hand &player);

using namespace Communication;

class CommThread : public Thread
{
private:
    Socket theSocket;
    Socket theSocket2;
public:
    CommThread(Socket const & p, Socket const & t)
        : Thread(true),theSocket(p), theSocket2(t)
    {
        ;
    }
    long ThreadMain(void)
    {
        ByteArray bytes;
        std::cout << "Created a socket thread!" << std::endl;
        bool gameRunning = true;
        Deck gameDeck;
        Hand player1;
        Hand player2;
        bool player1Turn = true;
        int player1Score =0;
        bool player2Turn = false;
        int player2Score = 0;
            
        initialDraw(player1, player2, gameDeck);
        while(gameRunning == true)
        {
            while(player1Turn)
            {
                std::string display = "Your Turn\n************************************\n";
                    
                    for(int i = 0; i < player1.getSize(); i++)
                    {
                        display += player1.showCard(i);
                    }

                    //Show Score
                    std::ostringstream os;
                    os << "Your score is: " << player1Score << "\n";
                    display += os.str();
                    
                    //Check for duplicates until there are no more
                    bool duplicate = checkForDuplicates(player1);		
                    while(duplicate)
                    {
                        display += "**************************\nYou got rid of a duplicate!\n";
                            
                            for(int i = 0; i < player1.getSize(); i++)
                            {
                                display += player1.showCard(i);
                            }
                            
                            std::ostringstream osx;
                            osx << "Your score is now "  << ++player1Score << std::endl;
                            display += osx.str();
                            
                            duplicate = checkForDuplicates(player1);
                    }
                    
                    //Server Output
                    std::cout << display;
                    
                    //Send output to client
                    ByteArray p1 = display;
                    theSocket.Write(p1);
        

                int read = theSocket.Read(bytes);
                if (read == -1)
                {
                    std::cout << "Error in socket detected" << std::endl;
                    break;
                }
                else if (read == 0)
                {
                    std::cout << "Socket closed at remote end" << std::endl;
                    break;
                }
                else
                {
                    std::string request = bytes.ToString();
                    std::cout << "Received: " << request << std::endl;
 
                    if(!askPlayer(player1, request)) 
                    {
                        //send message back here
                        std::cout << "Invalid entry! Please name a card that you have in your hand." << std::endl;
                        std::string invalid = "Invalid";
                        ByteArray ba(invalid);
                        theSocket.Write(ba);
                    }
                    else
                    {
                        std::ostringstream osi;
                        
                        //If player 2 has the same card, remove the card from each hand
                        if (checkPlayer(request, player2))
                        {
                                player1.removeCard(request);
                                player2.removeCard(request);
                                    
                                osi << "You have both discarded a " << request << ". Your score is now: " << ++player1Score << std::endl << "Pick another card.\n";
                                    
                                std::cout<<osi.str();
                                    
                                std::string toSend = osi.str();
                                ByteArray ba=toSend;
                                theSocket.Write(ba);
                                    
                        }
                        //If they do not, the player picks up a card and their turn ends
                        else
                        {
                                player1.addToHand(gameDeck.dealTopCard());
                                    
                                std::ostringstream osi;
                                osi << "Your opponent did not have a " << request<< ". You pick up a card." << std::endl;
                                    

                                std::cout<<osi.str();
                                
                                std::string toSend = osi.str();
                                ByteArray ba=toSend;
                                theSocket.Write(ba);
                                    
                                player1Turn = false;
                                player2Turn = true;
                                    
                        }
                        //sleep(100);
                    }
                }
            }
            
            while(player2Turn)
            {
                std::string display = "Your Turn Player 2\n************************************\n";
                    
                    for(int i = 0; i < player2.getSize(); i++)
                    {
                        display += player2.showCard(i);
                    }

                    //Show Score
                    std::ostringstream os;
                    os << "Your score is: " << player2Score << "\n";
                    display += os.str();
                    
                    //Check for duplicates until there are no more
                    bool duplicate = checkForDuplicates(player2);		
                    while(duplicate)
                    {
                        display += "**************************\nYou got rid of a duplicate!\n";
                            
                            for(int i = 0; i < player2.getSize(); i++)
                            {
                                display += player2.showCard(i);
                            }
                            
                            std::ostringstream osx;
                            osx << "Your score is now "  << ++player2Score << std::endl;
                            display += osx.str();
                            
                            duplicate = checkForDuplicates(player2);
                    }
                    
                    //Server Output
                    std::cout << display;
                    
                    //Send output to client
                    ByteArray p1 = display;
                    theSocket2.Write(p1);
        

                int read = theSocket2.Read(bytes);
                if (read == -1)
                {
                    std::cout << "Error in socket detected" << std::endl;
                    break;
                }
                else if (read == 0)
                {
                    std::cout << "Socket closed at remote end" << std::endl;
                    break;
                }
                else
                {
                    std::string request = bytes.ToString();
                    std::cout << "Received: " << request << std::endl;
 
                    if(!askPlayer(player2, request)) 
                    {
                        //send message back here
                        std::cout << "Invalid entry! Please name a card that you have in your hand." << std::endl;
                        std::string invalid = "Invalid";
                        ByteArray ba(invalid);
                        theSocket.Write(ba);
                    }
                    else
                    {
                        std::ostringstream osi;
                        
                        //If player 1 has the same card, remove the card from each hand
                        if (checkPlayer(request, player1))
                        {
                                player1.removeCard(request);
                                player2.removeCard(request);
                                    
                                osi << "You have both discarded a " << request << ". Your score is now: " << ++player2Score << std::endl << "Pick another card.\n";
                                    
                                std::cout<<osi.str();
                                    
                                std::string toSend = osi.str();
                                ByteArray ba=toSend;
                                theSocket2.Write(ba);
                                    
                        }
                        //If they do not, the player picks up a card and their turn ends
                        else
                        {
                                player2.addToHand(gameDeck.dealTopCard());
                                    
                                std::ostringstream osi;
                                osi << "Your opponent did not have a " << request<< ". You pick up a card." << std::endl;
                                std::cout<<osi.str();
                                
                                std::string toSend = osi.str();
                                ByteArray ba=toSend;
                                theSocket2.Write(ba);
                                    
                                player1Turn = true;
                                player2Turn = false;
                                
                                
                                    
                        }
                        //sleep(100);
                    }
                }
            }
            
            if (player1Score >= 5 || player2Score >= 5) 
            {
                std::ostringstream ost;
                gameRunning = false;
                ost << "The game is over! Final scores are:\n" << "Player 1: " << player1Score << "Player 2: " << player2Score << std::endl;
                std::cout<<ost.str();
                std::string toSend = ost.str();
                ByteArray ba = toSend;
                theSocket.Write(ba);
                sleep(2);
                theSocket2.Write(ba);
                std::cin.ignore();
            }
            
        }
            std::cout << "Thread is gracefully ending" << std::endl;
    }


    ~CommThread(void)
    {
        theSocket.Write(ByteArray("done"));
        terminationEvent.Wait();
        theSocket.Close();
    }
};

int main(void)
{
    std::cout << "I am a socket server.  Type 'quit' to exit" << std::endl;
    SocketServer theServer(2000);
    std::vector<CommThread *> threads;
    int count =0;
    Socket newSocket = NULL;

    for(;;)
    {
        try
        {
            FlexWait waiter(2,&theServer,&cinWatcher);
            Blockable * result = waiter.Wait();
            if (result == &cinWatcher)
            {
                std::string s;
                std::cin >> s;
                if (s=="quit")
                {
                    // No need to call SocketServer::Shutdown.  It isn't active.
                    break;
                }
                else
                    continue;
            }
            if(count==0)
            {
                newSocket = theServer.Accept();
                std::cout << "Received a socket connection from Client 1!" << std::endl;
                count++;
                // Accept should not now block.
            }
            else
            {
                Socket newSocket2 = theServer.Accept();
                std::cout << "Received a socket connection from Client 2!" << std::endl;
                threads.push_back(new CommThread(newSocket,newSocket2));
                count =0;

            // Accept should not now block.
            }
            
        }
        catch(TerminationException e)
        {
            std::cout << "The socket server is no longer listening. Exiting now." << std::endl;
            break;
        }
        catch(std::string s)
        {
            std::cout << "thrown " << s << std::endl;
            break;
        }
        catch(...)
        {
            std::cout << "caught  unknown exception" << std::endl;
            break;
        }
    }
    for (int i=0;i<threads.size();i++)
        delete threads[i];

    std::cout << "Sleep now" << std::endl;
    sleep(1);
    std::cout << "End of main" << std::endl;

}
void initialDraw(Hand &player1, Hand &player2, Deck &gameDeck)
{
	for(int i = 0; i < 7; i++)
        {
		player1.addToHand(gameDeck.dealTopCard());
		player2.addToHand(gameDeck.dealTopCard());
	}
}

bool checkForDuplicates(Hand &player)
{
	for(int i = 0; i < player.getSize(); i++)
        {
		std::string temp = player.showCardValue(i);
		
		for(int j = i+1; j < player.getSize(); j++)
                {
			std::string temp2 = player.showCardValue(j);
			
			if(temp == temp2)
                        {
				player.removeCard(temp);
				player.removeCard(temp);
				return true;
			}
		}
	}
	
	return false;
}

bool askPlayer(Hand &player, std::string input)
{
	bool inputValid = false;
        inputValid = verify(input, player);	
	
	return inputValid;
}

bool checkPlayer(std::string request, Hand &player)
{
	//If the other player has this card
		for(int x = 0; x < player.getSize(); x++)
                {
			if(player.showCardValue(x) == request)
                        {
				return true;
			}
		}
	
	return false;
}

bool verify(std::string input, Hand &player)
{
	//Convert the input to lowercase
	for(int i = 0; i < input.size(); i++)
        {
		std::transform(input.begin(), input.end(), input.begin(), ::tolower);
	}
	
	//If the input is valid
	if(input == "ace"|| input == "2"|| input == "3"|| input == "4"|| input == "5" || input == "6" || input == "7" || input == "8"|| input =="9" || input == "10" ||input =="jack"|| input == "queen"|| input =="king"){
		//If the player actually has this card
		for(int x = 0; x < player.getSize(); x++)
                {
			if(player.showCardValue(x) == input)
                        {
				return true;
			}
		}
	}
	
	return false;
}