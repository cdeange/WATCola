#include "truck.h"
#include "MPRNG.h"

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

}

bool Truck::hasCargo() {
  for( int i = 0; i < VendingMachine::FlavoursCount; i++ ) {
    if( mCargo[i] > 0 ) return true;
  }
  return false;
}

void Truck::main() {
  while( true ) {
    VendingMachine ** machineList = mNameServer.getMachineList();
    unsigned int yieldNum = RAND(1, 10);
    yield(yieldNum);

    try {
      mPlant.getShipment( mCargo );  
    } _CatchResume( BottlingPlant::Shutdown shutdown ) {
      return;
    }

    for( unsigned  machine = 0; machine < mNumVending; machine++ ) {
      int index = ( mStartVending + machine ) % mNumVending;

      // We have no cargo left :( keep index and start again later
      if( !hasCargo() ) {
        mStartVending = index;
        break;
      }

      // We have cargo to give out to this machine! :D
      unsigned int * machineInventory = machineList[index]->inventory();
      for( int flavor = 0; flavor < VendingMachine::FlavoursCount; flavor++ ) {
        unsigned int addedInventory = machineInventory[flavor] + mCargo[flavor];
        if( addedInventory > mMaxStockPerFlavour ) {
          mCargo[flavor] = mCargo[flavor] - ( mMaxStockPerFlavour - machineInventory[flavor] );
          machineInventory[flavor] = mMaxStockPerFlavour;
        } else {
          mCargo[flavor] = 0;
          machineInventory[flavor] += mCargo[flavor];
        }
      }

      machineList[index]->restocked();
    }
    
  }

}