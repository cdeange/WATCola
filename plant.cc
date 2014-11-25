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
  mPrinter.print( Printer::BottlingPlant,
                  BottlingPlant::Starting );

  mTruck = new Truck( mPrinter, 
                      mNameServer, 
                      (*this),
                      mNumVendingMachines,
                      mMaxStockPerFlavour );
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
    throw Shutdown();
  }
    
  mPrinter.print( Printer::BottlingPlant,
                  BottlingPlant::Pickup );
  unsigned int total = 0;

  for( int i = 0; i < VendingMachine::FlavoursCount; i++ ) {
    cargo[i] = RAND(mMaxShippedPerFlavour);
    total += cargo[i];
  }

  mPrinter.print( Printer::BottlingPlant,
                  ( char ) BottlingPlant::Generating, 
                  total );
}

void BottlingPlant::main() {
  
  while( !mShutdown ) {
    yield(mTimeBetweenShipments);
    _Accept( ~BottlingPlant ) {
      mShutdown = true;
      _Accept( getShipment ) {
        return;
      }
    } or _Accept( getShipment ) {}
  }

}