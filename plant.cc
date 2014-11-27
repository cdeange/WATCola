#include "plant.h"
#include "truck.h"
#include "MPRNG.h"
#include <iostream>

using namespace std;

BottlingPlant::BottlingPlant( 
  Printer &prt, 
  NameServer &nameServer, 
  unsigned int numVendingMachines,
  unsigned int maxShippedPerFlavour, 
  unsigned int maxStockPerFlavour,
  unsigned int timeBetweenShipments ) 
: mPrinter( prt ),
  mNameServer( nameServer ),
  mNumVendingMachines( numVendingMachines ),
  mMaxShippedPerFlavour( maxShippedPerFlavour ),
  mMaxStockPerFlavour( maxStockPerFlavour ),
  mTimeBetweenShipments( timeBetweenShipments ) {

  mShutdown = false;
  for ( unsigned int i = 0; i < VendingMachine::FlavoursCount; ++i ) {
    mStock[i] = 0;
  }

  mPrinter.print( Printer::BottlingPlant,
                  BottlingPlant::Starting );
}

BottlingPlant::~BottlingPlant() {
  delete mTruck;
  mTruck = NULL;
  mPrinter.print( Printer::BottlingPlant,
                  BottlingPlant::Finished );
}

void BottlingPlant::getShipment( unsigned int cargo[] ) {
  if( mShutdown ) {
    uRendezvousAcceptor();
    _Throw Shutdown();
  }

  for ( unsigned int i = 0; i < VendingMachine::FlavoursCount; ++i ) {
    cargo[i] = mStock[i];
    mStock[i] = 0;
  }

  mPrinter.print( Printer::BottlingPlant,
                  BottlingPlant::Pickup );
}

void BottlingPlant::createProductionRun() {
  unsigned int total = 0;
  for( int i = 0; i < VendingMachine::FlavoursCount; i++ ) {
    mStock[i] = RAND(mMaxShippedPerFlavour);
    total += mStock[i];
  }

  mPrinter.print( Printer::BottlingPlant,
                  ( char ) BottlingPlant::Generating, 
                  total );
}

void BottlingPlant::main() {
  mTruck = new Truck( mPrinter, 
                      mNameServer, 
                      (*this),
                      mNumVendingMachines,
                      mMaxStockPerFlavour );
  
  createProductionRun();
  
  while( true ) {
    _Accept( ~BottlingPlant ) {
      mShutdown = true;
      _Accept( getShipment ) {
        return;
      }
    } or _Accept( getShipment ) {
      yield(mTimeBetweenShipments);
      createProductionRun();
    }
  }

}