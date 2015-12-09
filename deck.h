#ifndef DECK_H
#define DECK_H

#include "card.h"
#include<stdlib.h>
#include<time.h>
#include<queue>

class Deck {
	private:
		std::queue<Card> myDeck;
		Card myDeckHold[52];
	public:
		void addCard(Card newCard);
		void shuffle();
		void transfer();
		Card dealTopCard();
		Deck();
};

#endif