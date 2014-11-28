#include "nameserver.h"
#include "printer.h"
#include "vendingmachine.h"

using namespace std;

NameServer::NameServer( Printer & printer, unsigned int numVendingMachines, unsigned int numStudents ) 
    : mPrinter( printer ), mNumVendingMachines( numVendingMachines ), mNumStudents( numStudents ) {

  mMachines = new VendingMachine*[mNumVendingMachines];
  mStudents = new int[mNumStudents];
  for ( unsigned int i = 0; i < mNumStudents; ++i ) {
    mStudents[i] = i % mNumVendingMachines;
  }

  mVMIndex = 0;
}

NameServer::~NameServer() {
  delete[] mStudents;
  delete[] mMachines;
}

void NameServer::VMregister( VendingMachine *vendingMachine ) {
  mPrinter.print( Printer::NameServer,
                  ( char ) NameServer::Register,
                  vendingMachine->getId() );
  mMachines[mVMIndex] = vendingMachine;
  mVMIndex += 1;
}

VendingMachine* NameServer::getMachine( unsigned int id ) {

  // Student should get the next VendingMachine
  mStudents[id] = ( mStudents[id] + 1 ) % mNumVendingMachines;

  mPrinter.print( Printer::NameServer,
                  ( char ) NameServer::New,
                  id,
                  mMachines[mStudents[id]]->getId() );

  return mMachines[mStudents[id]];
}

VendingMachine** NameServer::getMachineList() {
  return mMachines;
}

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
