#ifndef CARD_H
#define CARD_H

#include<string>
#include<iostream>
#include <algorithm>

class Card{
	private:
		std::string value;
		std::string suit;
		
	public:
		Card(std::string value, std::string mySuit);
		Card();
		std::string getValue();
		std::string showCard();
};

#endif