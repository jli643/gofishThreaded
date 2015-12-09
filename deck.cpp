#include "deck.h"
Deck::Deck(){	
	
	//Populate the deck
	std::string suitOptions[4] = {"Spades", "Diamonds", "Hearts", "Clubs"};
	std::string numberOptions[13] = {"Ace","2","3","4","5","6","7","8","9","10","Jack","Queen","King"};	
	
	for(int i = 0; i < 4; i++){	
		for(int x = 0; x < 13; x++){
			Card newCard(numberOptions[x], suitOptions[i]);
			addCard(newCard);
		}
	}
	
	//Shuffle the deck
	shuffle();
	
	//Transfer it into a queue
	transfer();
}

//Adds a card to the deck
void Deck::addCard(Card newCard){
	static int x = 0;
	myDeckHold[x++] = newCard;
}

//A Fisher-Yates Shuffle Method
void Deck::shuffle(){
	srand (time(NULL));
	for(int i = 51; i > 0; i--){
		int j = (rand() % (i + 1));

		Card temp = myDeckHold[j];
		myDeckHold[j] = myDeckHold[i];
		myDeckHold[i] = temp;	
	}			
}

//Takes the deck and transfers it to a queue
void Deck::transfer(){
	for(int i = 0; i < 52; i++){
		myDeck.push(myDeckHold[i]);
	}
}

//Returns the card currently on top of the deck, removes it from the deck
Card Deck::dealTopCard(){
	Card temp = myDeck.front();
	myDeck.pop();
	return temp;
}