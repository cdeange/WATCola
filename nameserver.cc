#include "nameserver.h"
#include "printer.h"
#include "vendingmachine.h"

using namespace std;

/******** Nameserver constructor  *******
  Purpose: Constructs a nameserver object

  Returns: n/a

  Errors: n/a

  Globals: n/a
*/
NameServer::NameServer( Printer & printer, unsigned int numVendingMachines, unsigned int numStudents ) 
    : mPrinter( printer ), mNumVendingMachines( numVendingMachines ), mNumStudents( numStudents ) {

  mMachines = new VendingMachine*[mNumVendingMachines];
  mStudents = new int[mNumStudents];

  // Assigns each student to their initial vending machine
  for ( unsigned int i = 0; i < mNumStudents; ++i ) {
    mStudents[i] = i % mNumVendingMachines;
  }

  mVMIndex = 0;
}

/******** Nameserver destructor  *******
  Purpose: Cleans up name server during deletion

  Returns: n/a

  Errors: n/a

  Globals: n/a
*/
NameServer::~NameServer() {
  delete[] mStudents;
  delete[] mMachines;
}


/******** Nameserver VMregister  *******
  Purpose: Called by vending machines to register themselves to the system

  Returns: n/a

  Errors: n/a

  Globals: n/a
*/
void NameServer::VMregister( VendingMachine *vendingMachine ) {
  mPrinter.print( Printer::NameServer,
                  ( char ) NameServer::Register,
                  vendingMachine->getId() );
  mMachines[mVMIndex] = vendingMachine;
  mVMIndex += 1;
}


/******** Nameserver getMachine  *******
  Purpose: Gets a particular machine for a particular student

  Returns: A newly assigned vending machine

  Errors: n/a

  Globals: n/a
*/
VendingMachine* NameServer::getMachine( unsigned int id ) {

  // Student should get the next VendingMachine
  mStudents[id] = ( mStudents[id] + 1 ) % mNumVendingMachines;

  mPrinter.print( Printer::NameServer,
                  ( char ) NameServer::New,
                  id,
                  mMachines[mStudents[id]]->getId() );

  return mMachines[mStudents[id]];
}

/******** Nameserver getMachineList  *******
  Purpose: n/a

  Returns: The list of vending machines

  Errors: n/a

  Globals: n/a
*/
VendingMachine** NameServer::getMachineList() {
  return mMachines;
}


/******** Nameserver main  *******
  Purpose: adminstrates all task calling it

  Returns: n/a

  Errors: n/a

  Globals: n/a
*/
void NameServer::main() {

  mPrinter.print( Printer::NameServer, NameServer::Starting );

  while ( true ) {
    _Accept( ~NameServer ) { 
      break;
    } or _When( mVMIndex != mNumVendingMachines ) _Accept( VMregister ) {
    } or _When( mVMIndex == mNumVendingMachines ) _Accept( getMachine, getMachineList ) {
    }
  }

  mPrinter.print( Printer::NameServer, NameServer::Finished );
}
