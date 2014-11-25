#include "nameserver.h"
#include "printer.h"
#include "vendingmachine.h"

using namespace std;

NameServer::NameServer( Printer & printer, unsigned int numVendingMachines, unsigned int numStudents ) 
    : mPrinter( printer ), mNumVendingMachines( numVendingMachines ), mNumStudents( numStudents ) {

  mMachines = new VendingMachine*[mNumVendingMachines];
  mStudents = new int[mNumStudents];
  for ( unsigned int i = 0; i < mNumStudents; ++i ) {
    mStudents[i] = -1;
  }

  mVMIndex = 0;
  mStudentIndex = 0;
  mRegisterIndex = 0;
}

NameServer::~NameServer() {
  delete[] mStudents;
  delete[] mMachines;
}

void NameServer::VMregister( VendingMachine *vendingMachine ) {
  mMachines[mVMIndex] = vendingMachine;
  mVMIndex += 1;
}

VendingMachine* NameServer::getMachine( unsigned int id ) {

  if ( mStudents[id] == -1 ) {
    // Student has not been registered a VendingMachine yet
    while ( mRegisterIndex == mVMIndex && mVMIndex != mNumVendingMachines ) {
      _Accept( VMregister );
    }
    mStudents[id] = mRegisterIndex;
    mRegisterIndex = ( mRegisterIndex + 1 ) % mNumStudents;

  } else {
    // Student should get the next VendingMachine
    while ( mStudents[id] == ( int ) mVMIndex && mVMIndex != mNumVendingMachines ) {
      _Accept( VMregister );
    }

    mStudents[id] = ( mStudents[id] + 1 ) % mNumVendingMachines;
  }

  return mMachines[mStudents[id]];
}

VendingMachine** NameServer::getMachineList() {
  return mMachines;
}

void NameServer::main() {

}
