#include "card.h"
Card::Card(std::string myValue, std::string mySuit){
	value = myValue;
	suit = mySuit;
}

Card::Card() {}

std::string Card::showCard(){
    std::string blah =  value + " of " + suit + "\n";
    return blah;
	
}

std::string Card::getValue(){
	std::string blah = value;
	std::transform(blah.begin(), blah.end(), blah.begin(), ::tolower);
	return blah;
}