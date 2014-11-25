#include "MPRNG.h"
#include "bank.h"
#include "parent.h"
#include "printer.h"

using namespace std;

Parent::Parent( Printer & prt,
                Bank & bank,
                unsigned int numStudents,
                unsigned int parentalDelay ) 
  : mPrinter(prt), 
    mBank(bank), 
    mNumStudents(numStudents), 
    mDelay(parentalDelay) {}

Parent::~Parent() {
  mPrinter.print( Printer::Parent, Finished );
}

void Parent::main() {
  while( true ) {
    // Assignment says use _Else, figure out why :O
    _Accept( ~Parent ) { return; }
    unsigned int money = RAND(1, 3);
    unsigned int studentId = RAND(mNumStudents - 1);
    yield(mDelay);
    mPrinter.print( Printer::Parent, (char) Deposit, studentId, money );
    mBank.deposit( studentId, money );
  }
}
