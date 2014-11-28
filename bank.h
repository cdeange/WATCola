#ifndef BANK_H_
#define BANK_H_

#include <vector>


/******** Bank class  *******
  Purpose: Student bank accounts
*/
_Monitor Bank {
  public:
    Bank( unsigned int numStudents );
    ~Bank();
    void deposit( unsigned int id, unsigned int amount );
    void withdraw( unsigned int id, unsigned int amount );

  private:
    unsigned int* mBalances;  // Balance
    uCondition* mLocks;       // Lock for waiting in withdraw
};

#endif
