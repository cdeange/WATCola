#include "WATCard.h"

WATCard::WATCard( const WATCard & ) {}

WATCard& WATCard::operator=( const WATCard & watcard) {
  throw 1; 
}

WATCard::WATCard() {
  mBalance = 0;
}

void WATCard::deposit( unsigned int amount ) {
  mBalance += amount;
}

// TODO do we need to check things here? 
// Assumes whatever calls it determines whether there is enough
void WATCard::withdraw( unsigned int amount ) {
  mBalance -= amount;
}

unsigned int WATCard::getBalance() {
  return mBalance;
}