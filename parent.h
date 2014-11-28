#ifndef PARENT_H_
#define PARENT_H_

#include "bank.h"
#include "printer.h"


_Task Parent {
  public:
    Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay );

  private:
    // States for printing
    enum State {
      Starting = 'S', 
      Deposit  = 'D', 
      Finished = 'F'
    };

    void main();

    Printer & mPrinter;
    Bank & mBank;
    unsigned int mNumStudents;    // Number of students in total 
    unsigned int mDelay;          // Delay before giving out money
};

#endif