#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <vector>
#include <pthread.h>
#include "Blockable.h"
#include "socket.h"
#include "socketserver.h"
#include "card.h"
#include "deck.h"
#include "hand.h"
#include "thread.h"

using namespace Communication;

void initialDraw(Hand &player1, Hand &player2, Deck &gameDeck);
std::string askPlayer(Hand &player);
bool checkPlayer(std::string request, Hand &player);
bool verify(std::string input, Hand &player);
bool checkForDuplicates(Hand &player);


int competitors=0;
std::vector<int> players;

class CommThread : public Thread
{
private:
    Socket theSocket;
public:
    CommThread(Socket const &p) : Thread(true),theSocket(p)
    {
        ;
    }
    long ThreadMain(void)
    {
        ByteArray bytes;
        std::string winner= "";
        for(;;)
        {
            int read = theSocket.Read(bytes);
            if(read ==-1)
            {
                std::cout<<"Error in socket detected" <<std::endl;
                break;
            }
            else if(read ==0)
            {
                std::cout<<"socket closed at remote end" <<std::endl;
                break;
            }
            else
            {
                std::string theString = bytes.ToString();
                std::cout<<"Received: " <<theString <<std::endl;
                
                if(competitors ==2)
                {
                    std::cout<<"There are now 2 players. Game is commencing..." <<std::endl;
                }
            }
        }
    }
};
int main(void)
{
    std::cout << "I am a socket server. Type 'quit' to exit" << std::endl;
    SocketServer server(2000);
    std::vector<CommThread*> threads;
    
    for(;;)
    {
        try
        {
            FlexWait waiter(2,&server,&cinWatcher);
            Blockable *result = waiter.Wait();
            if(result == &cinWatcher)
            {
                std::string s;
                std::cin>>s;
                if(s=="quit")
                {
                    break;
                }
                else
                    continue;
            }
        Socket newSocket = server.Accept();
        std::cout <<"A new player has joined the game" <<std::endl;
        competitors = threads.size() +1;
        threads.push_back(new CommThread(newSocket));
        if(competitors ==2)
        {
            Deck gameDeck;
            Hand player1;
            Hand player2;
            bool gameRunning = true;
            bool player1Turn = true;
            int player1Score =0;
            bool player2Turn = false;
            int player2Score = 0;
            
            initialDraw(player1, player2, gameDeck);
            
            while (gameRunning) {
		//Player 1's turn
		while (player1Turn) {
			std::cout << "Player 1's Turn\n************************************" << std::endl;

			//Show Hand
                        //std::string blah = "Hello!\n";
                        //ByteArray player1handtemp(blah);
                        //theSocket.Write(player1temp);
			player1.showHand();

			//Show Score
			std::cout << "Your score is: " << player1Score << std::endl;

			//Check for duplicates until there are no more
			bool duplicate = checkForDuplicates(player1);		
			while(duplicate){
				std::cout << "You got rid of a duplicate! Your score is now " << ++player1Score << std::endl << "**************************\n";
				player1.showHand();
				duplicate = checkForDuplicates(player1);
			}

			//Make a request string
			std::string request;

			//Player 1 makes a request
			request = askPlayer(player1);

			//If player 2 has the same card, remove the card from each hand
			if (checkPlayer(request, player2)) {
				player1.removeCard(request);
				player2.removeCard(request);
				std::cout << "You have both discarded a " << request << ". Your score is now: " << ++player1Score << std::endl;
			}

			//If they do not, the player picks up a card and their turn ends
			else {
				player1.addToHand(gameDeck.dealTopCard());
				std::cout << "Your opponent did not have a " << request<< ". You pick up a card." << std::endl;
				player1Turn = false;
				player2Turn = true;
			}
		}

		/*while (player2Turn) {
			std::cout << "Player 2's Turn\n************************************" << endl;
			
			player2.showHand();

			std::cout << "Your score is: " << player2Score << endl;

			bool duplicate = checkForDuplicates(player2);
			while(duplicate) {
				std::cout << "You got rid of a duplicate! Your score is now " << ++player2Score << endl << "**************************\n";
				player2.showHand();
				duplicate = checkForDuplicates(player2);
			}

			std::string request;

			request = askPlayer(player2);

			if (checkPlayer(request, player1)) {
				player1.removeCard(request);
				player2.removeCard(request);
				std::cout << "You have both discarded a " << request << ". Your score is now: " << ++player2Score << endl;
			}

			else {
				player2.addToHand(gameDeck.dealTopCard());
				std::cout << "Your opponent did not have a " << request << ". You pick up a card." << endl;
				player2Turn = false;
				player1Turn = true;
			}
		}*/

		if (player1Score >= 5 || player2Score >= 5) {
			gameRunning = false;
			std::cout << "The game is over! Final scores are:\n" << "Player 1: " << player1Score << "Player 2: " << player2Score << std::endl;
			std::cin.ignore();
		}
	}
        
        }
        
        
        
        }
        catch(TerminationException e)
        {
            std::cout<<"The socket is no longer listening. Exiting now" << std::endl;
            break;
        }
        catch(std::string s)
        {
            std::cout<<"thrown" << s << std::endl;
            break;
        }
        catch(...)
        {
            std::cout<<"caught unknown exception" << std::endl;
            break;
        }
    
    }
    for(int i=0;i<threads.size();i++)
    {
        delete threads[i];
    }
    std::cout << "Sleep now baby" <<std::endl;
    server.Shutdown();
    sleep(1);
        
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

std::string askPlayer(Hand &player)
{
	
	std::string input;
	bool inputValid = false;
	
	while(!inputValid)
        {
		std::cout << "You ask your oppponent for a card value (Ex. Ace, 3, King):" << std::endl;
		std::cout << "Do you have a... ";
		std::cin >> input;
		inputValid = verify(input, player);	
		if (!inputValid) 
                {
			std::cout << "Invalid entry! Please name a card that you have in your hand." << std::endl;
		}

	}
	
	return input;
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