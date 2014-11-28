#include "plant.h"
#include "truck.h"
#include "MPRNG.h"
#include <iostream>

using namespace std;

/* Bottling plant constructor */
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
  // Giving initial stock 
  for ( unsigned int i = 0; i < VendingMachine::FlavoursCount; ++i ) {
    mStock[i] = 0;
  }
}

/* Bottling plant destructor */
BottlingPlant::~BottlingPlant() {
  delete mTruck;
}

/******** BottlingPlant getShipment  *******
  Purpose: Called by truck to give a shipment of cargo

  Returns: cargo list of drinks

  Errors: Shutdown 

  Globals: member variables
*/
void BottlingPlant::getShipment( unsigned int cargo[] ) {
  // Bottling plant is closing down 
  if( mShutdown ) {
    uRendezvousAcceptor();
    _Throw Shutdown();
  }

  // Restocking the cargo
  for ( unsigned int i = 0; i < VendingMachine::FlavoursCount; ++i ) {
    cargo[i] = mStock[i];
    mStock[i] = 0;
  }

  mPrinter.print( Printer::BottlingPlant,
                  BottlingPlant::Pickup );
}

/******** BottlingPlant createProductionRun  *******
  Purpose: Does a production run to restocks the vending machines

  Returns: n/a

  Errors: n/a 

  Globals: member variables
*/
void BottlingPlant::createProductionRun() {
  // Simulates doing a production run for each flavour in the vending machine
  unsigned int total = 0;
  for( int i = 0; i < VendingMachine::FlavoursCount; i++ ) {
    mStock[i] = RAND(mMaxShippedPerFlavour);
    total += mStock[i];
  }

  mPrinter.print( Printer::BottlingPlant,
                  ( char ) BottlingPlant::Generating, 
                  total );
}

/******** BottlingPlant main  *******
  Purpose: Administrates the production creation and shipments

  Returns: n/a

  Errors: n/a 

  Globals: member variables
*/
void BottlingPlant::main() {

  mPrinter.print( Printer::BottlingPlant, BottlingPlant::Starting );

  // One truck per plant
  mTruck = new Truck( mPrinter, 
                      mNameServer, 
                      (*this),
                      mNumVendingMachines,
                      mMaxStockPerFlavour );
  
  createProductionRun();
  
  while( true ) {
    _Accept( ~BottlingPlant ) {
      mShutdown = true;
      // Get shipment one more time for the truck
      _Accept( getShipment ) {
        break;
      }
    } or _Accept( getShipment ) {
      yield(mTimeBetweenShipments);
      createProductionRun();
    }
  }

  mPrinter.print( Printer::BottlingPlant, BottlingPlant::Finished );

}