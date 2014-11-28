#include "MPRNG.h"
#include "bank.h"
#include "parent.h"
#include "printer.h"

using namespace std;

/******** Parent constructor  ********/
Parent::Parent( Printer & prt,
                Bank & bank,
                unsigned int numStudents,
                unsigned int parentalDelay ) 
  : mPrinter(prt), 
    mBank(bank), 
    mNumStudents(numStudents), 
    mDelay(parentalDelay) {
}


/******** Parent main  *******
  Purpose: Gives money to random student

  Returns: n/a

  Errors: n/a

  Globals: member variables
*/
void Parent::main() {

  mPrinter.print( Printer::Parent, Parent::Starting );

  while( true ) {
    _Accept( ~Parent ) { 
      break;

    } _Else {
      yield(mDelay);
      unsigned int money = RAND(1, 3);                 // Pick random amount of money
      unsigned int studentId = RAND(mNumStudents - 1); // Pick random student
      mPrinter.print( Printer::Parent, (char) Parent::Deposit, studentId, money );
      mBank.deposit( studentId, money );               // Give money to student
    }
  }

  mPrinter.print( Printer::Parent, Parent::Finished );

}
