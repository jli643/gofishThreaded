#ifndef HAND_H
#define HAND_H

#include "card.h"
#include <vector>
#include <algorithm>

class Hand{
	private:
		std::vector<Card> myHand;
	public:
		void removeCard(std::string valueToRemove);
		void addToHand(Card addedCard);
		void showHand();
		std::string showCardValue(int i);
		int getSize();
		Hand() {}
};

#endif