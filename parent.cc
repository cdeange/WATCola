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
    mDelay(parentalDelay) {
  mPrinter.print( Printer::Parent, Parent::Starting );
}

Parent::~Parent() {
  mPrinter.print( Printer::Parent, Parent::Finished );
}

void Parent::main() {
  while( true ) {
    _Accept( ~Parent ) { 
      return; 
    } _Else {
      unsigned int money = RAND(1, 3);
      unsigned int studentId = RAND(mNumStudents - 1);
      yield(mDelay);
      mPrinter.print( Printer::Parent, (char) Parent::Deposit, studentId, money );
      mBank.deposit( studentId, money );  
    }
  }
}
