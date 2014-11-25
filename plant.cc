#include "plant.h"
#include "truck.h"
#include "MPRNG.h"

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

  shutdown = false;
}

BottlingPlant::~BottlingPlant() {}

void BottlingPlant::getShipment( unsigned int cargo[] ) {
  bool allzero = true;
  _Accept( ~BottlingPlant ) {
    uRendezvousAcceptor();
    throw Shutdown();
  } _Else {
    for( int i = 0; i < VendingMachine::FlavoursCount; i++ ) {
      cargo[i] = RAND(mMaxShippedPerFlavour);
      if( cargo[i] != 0 && allzero ) allzero = false;
    }  
  }
}

void BottlingPlant::main() {
  Truck truck( mPrinter, 
               mNameServer, 
               (*this),
               mNumVendingMachines,
               mMaxStockPerFlavour );
  
  while( true ) {
    yield(mTimeBetweenShipments);
    _Accept( getShipment ) {}
    
    if ( shutdown ) break;
  }

}