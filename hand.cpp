#include "hand.h"

void Hand::showHand(){
	for(int i = 0; i < myHand.size(); i++){
		std::cout << myHand.at(i).showCard() << std::endl;
	}
}

void Hand::addToHand(Card addedCard){
	myHand.push_back(addedCard);
}

void Hand::removeCard(std::string valueToRemove){
	for(int i = 0; i < myHand.size(); i++){
		//std::cout << "Value to remove: " << valueToRemove;
		
		std::string blah = myHand.at(i).getValue();
		
		if(blah == valueToRemove){
			myHand.erase((myHand.begin() + i));
			//std::cout << "Inside removeCard, getValue:" << s;
			break;
		}
	}
}

std::string Hand::showCardValue(int i){
	std::string value = myHand.at(i).getValue();
	
	//Convert to lowercase
	std::transform(value.begin(), value.end(), value.begin(), ::tolower);

	return value;
}

int Hand::getSize(){
	return myHand.size();
}

std::string Hand::showCard(int i){
    std::string blah = myHand.at(i).showCard();
    return blah;
}