#include "bank.h"

using namespace std;

Bank::Bank( unsigned int numStudents ) : mBalances( numStudents, 0 ), mLocks( numStudents ) {
}

Bank::~Bank() {
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
