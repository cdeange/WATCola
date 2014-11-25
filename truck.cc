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
  mPrinter.print( Printer::Truck, Truck::Start );
}

Truck::~Truck() {
  mPrinter.print( Printer::Truck, Truck::Finished );
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

      // We have no cargo left :( keep machineId and start again later
      if( !hasCargo() ) {
        mStartVending = machineId;
        break;
      }

      // We have cargo to give out to this machine! :D
      unsigned int * machineInventory = machineList[machineId]->inventory();

      for( int flavor = 0; flavor < VendingMachine::FlavoursCount; flavor++ ) {
        unsigned int addedInventory = machineInventory[flavor] + mCargo[flavor];
        int given;
        
        if( addedInventory >= mMaxStockPerFlavour ) {
          given = ( mMaxStockPerFlavour - machineInventory[flavor] );
        } else {
          given = mCargo[flavor];
          mPrinter.print( Printer::Truck, 
                          Truck::Unsuccessful, 
                          machineId, 
                          mMaxStockPerFlavour - given );
        }

        total = total - given;
        machineInventory[flavor] += given;
        mCargo[flavor] = mCargo[flavor] - given;
      }
      machineList[machineId]->restocked();
      mPrinter.print(Printer::Truck, Truck::Delivery, machineId, total);
    }
  }
}