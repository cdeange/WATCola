#include "bank.h"

Bank::Bank( unsigned int numStudents ) {
  for( unsigned int i = 0; i < numStudents; i++ ) {
    mBalances.push_back(0);
  }
}

void Bank::deposit( unsigned int id, unsigned int amount ) {
  mBalances[id] += amount;
}

void Bank::withdraw( unsigned int id, unsigned int amount ) {
  while( mBalances[id] < amount ) {
    _Accept(deposit);
  }
  
  mBalances[id] -= amount;
}