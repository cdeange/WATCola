#include "truck.h"
#include "MPRNG.h"
#include <iostream>

using namespace std;

/* Truck constructor */
Truck::Truck( Printer & prt, 
              NameServer & nameServer, 
              BottlingPlant & plant,
              unsigned int numVendingMachines, 
              unsigned int maxStockPerFlavour ) 
: mPrinter( prt ), 
  mNameServer( nameServer ), 
  mPlant( plant ), 
  mNumVending( numVendingMachines ),
  mMaxStockPerFlavour( maxStockPerFlavour ) {

  mStartVending = 0;
  mPrinter.print( Printer::Truck, Truck::Starting );
}

/* Truck destructor */
Truck::~Truck() {
  mPrinter.print( Printer::Truck, Truck::Finished );
}

/******** Truck hasCargo  *******
  Purpose: Checks whether the truck has cargo or not

  Returns: true or false

  Errors: n/a 

  Globals: n/a
*/
bool Truck::hasCargo() {
  for( int i = 0; i < VendingMachine::FlavoursCount; i++ ) {
    if( mCargo[i] > 0 ) return true;
  }
  return false;
}

/******** Truck main  *******
  Purpose: Administers the behavior of the task

  Returns: n/a

  Errors: n/a 

  Globals: member variables
*/
void Truck::main() {

  VendingMachine ** machineList = mNameServer.getMachineList();

  while( true ) {
    unsigned int yieldNum = RAND(1, 10);
    yield(yieldNum);

    try {
      mPlant.getShipment( mCargo );  
    } catch ( BottlingPlant::Shutdown & shutdown ) {
      return;
    }

    // calculating the total cargo 
    int total = 0;
    for( int i = 0; i < VendingMachine::FlavoursCount; i++ ) {
      total += mCargo[i];
    }
    mPrinter.print(Printer::Truck, Truck::Pickup, total);

    for( unsigned  machine = 0; machine < mNumVending; machine++ ) {


      int machineId = ( mStartVending + machine ) % mNumVending;
      mPrinter.print(Printer::Truck, Truck::Begin, machineId, total);

      // We have cargo to give out to this machine! :D
      unsigned int * machineInventory = machineList[machineId]->inventory();
      unsigned int unstocked = 0;

      for( int flavor = 0; flavor < VendingMachine::FlavoursCount; flavor++ ) {
        int given;

        if( mMaxStockPerFlavour > machineInventory[flavor] + mCargo[flavor] ) {
          // Cannot fully restock this flavour
          unstocked += ( mMaxStockPerFlavour - mCargo[flavor] );
          given = mCargo[flavor];

        } else {
          // We have enough!
          given = ( mMaxStockPerFlavour - machineInventory[flavor] );
        }

        total -= given;
        machineInventory[flavor] += given;
        mCargo[flavor] -= given;
      }

      if ( unstocked != 0 ) {
        mPrinter.print( Printer::Truck, 
                        Truck::Unsuccessful, 
                        machineId, 
                        unstocked );
      }

      machineList[machineId]->restocked();
      // We have no cargo left :( keep machineId and start again later
      if( !hasCargo() ) {

        mStartVending = ( machineId + 1 ) % mNumVending;
        break;
      }
      mPrinter.print(Printer::Truck, Truck::Delivery, machineId, total);
    }
  }
}