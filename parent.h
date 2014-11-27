#ifndef PARENT_H_
#define PARENT_H_

#include "bank.h"
#include "printer.h"

_Task Parent {
  public:
    Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay );
    enum State {
      Starting = 'S', 
      Deposit  = 'D', 
      Finished = 'F'
    };

  private:
    void main();

    Printer & mPrinter;
    Bank & mBank;
    unsigned int mNumStudents;
    unsigned int mDelay;
};

#endif