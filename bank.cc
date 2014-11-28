#include "bank.h"

using namespace std;

Bank::Bank( unsigned int numStudents ) {
  mBalances = new unsigned int[numStudents] ;
  for ( unsigned int i = 0; i < numStudents; ++i ) {
    mBalances[i] = 0;
  }
  mLocks = new uCondition[numStudents];
}

Bank::~Bank() {
  delete[] mBalances;
  delete[] mLocks;
}

void Bank::deposit( unsigned int id, unsigned int amount ) {
  mBalances[id] += amount;
  mLocks[id].signal();
}

void Bank::withdraw( unsigned int id, unsigned int amount ) {
  while ( mBalances[id] < amount ) {
    mLocks[id].wait();
  }

  mBalances[id] -= amount;
}
