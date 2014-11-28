#include "bank.h"

using namespace std;

/******** Bank constructor  *******
  Purpose: Constructs a bank object
*/
Bank::Bank( unsigned int numStudents ) {
  mBalances = new unsigned int[numStudents] ;
  for ( unsigned int i = 0; i < numStudents; ++i ) {
    mBalances[i] = 0;
  }
  mLocks = new uCondition[numStudents];
}


/******** Bank destructor  *******
  Purpose: Destructs a bank object
*/
Bank::~Bank() {
  delete[] mBalances;
  delete[] mLocks;
}


/******** Bank deposit  *******
  Purpose: Deposits amount into id's bank account

  Returns: n/a

  Errors: n/a

  Globals: member variables
*/
void Bank::deposit( unsigned int id, unsigned int amount ) {
  mBalances[id] += amount;
  mLocks[id].signal();
}


/******** Bank withdraw  *******
  Purpose: Withdraws amount into id's bank account. Blocks if not enough money.

  Returns: n/a

  Errors: n/a

  Globals: member variables
*/
void Bank::withdraw( unsigned int id, unsigned int amount ) {
  while ( mBalances[id] < amount ) {
    mLocks[id].wait();
  }

  mBalances[id] -= amount;
}
