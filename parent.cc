#include "parent.h"
#include "bank.h"
#include "printer.h"

Parent::Parent( Printer & prt,
                Bank & bank,
                unsigned int numStudents,
                unsigned int parentalDelay ) 
  : mPrinter(prt), 
    mBank(bank), 
    mNumStudents(numStudents), 
    mDelay(parentalDelay) {}

Parent::~Parent() {
  mPrinter.print( Parent, Finished );
}

void Parent::main() {
  while( true ) {
    // Assignment says use _Else, figure out why :O
    _Accept( ~Parent ) { return; }
    unsigned int money = RAND(1, 3);
    unsigned int studentId = RAND(numStudents-1);
    yield(parentalDelay);
    mPrinter.print( Parent, Deposit, studentId, money );
    mBank.deposit( studentId, money );
  }
}