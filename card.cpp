#include "card.h"
Card::Card(std::string myValue, std::string mySuit){
	value = myValue;
	suit = mySuit;
}

Card::Card() {}

void Card::showCard(){
	std::cout << value << " of " << suit << std::endl;
}

std::string Card::getValue(){
	std::string blah = value;
	std::transform(blah.begin(), blah.end(), blah.begin(), ::tolower);
	return blah;
}