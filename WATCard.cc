#include "WATCard.h"

WATCard::WATCard() {
  mBalance = 0;
}

void WATCard::deposit( unsigned int amount ) {
  mBalance += amount;
}

void WATCard::withdraw( unsigned int amount ) {
  mBalance -= amount;
}

unsigned int WATCard::getBalance() {
  return mBalance;
}