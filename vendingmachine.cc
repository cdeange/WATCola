#include "vendingmachine.h"
#include "nameserver.h"

using namespace std;

VendingMachine::VendingMachine( 
    Printer &prt, 
    NameServer &nameServer, 
    unsigned int id, 
    unsigned int sodaCost,
    unsigned int maxStockPerFlavour ) 
: mPrinter(prt), 
  mNameServer(nameServer), 
  mId(id),
  mCost(sodaCost),
  mMaxStockPerFlavor(maxStockPerFlavour) {

  for( int i = 0; i < FlavoursCount; i++ ) {
    mInventory[i] = 0;
  }

  mNameServer.VMregister( this );
  mPrinter.print( Printer::Vending, mId, (char)Starting, sodaCost );
}

VendingMachine::~VendingMachine() {
  mPrinter.print( Printer::Vending, Finished );
}

void VendingMachine::buy( Flavours flavour, WATCard &card ) {
  unsigned int balance = card.getBalance();
  // This might break since the main might not start up as a result
  // TODO test
  if( balance < mCost ) {
    throw Funds();
  } else if ( mInventory[flavour] <= 0 ) {
    throw Stock();
  }

  card.withdraw( mCost );
  mInventory[flavour]--;

  mPrinter.print( Printer::Vending, Buying, flavour, mInventory[flavour] );
}

unsigned int * VendingMachine::inventory() {
  return mInventory;
}

void VendingMachine::restocked() {}

unsigned int VendingMachine::cost() {
  return mCost;
}

unsigned int VendingMachine::getId() {
  return mId;
}

void VendingMachine::main() {
  
  while( true ) {
    _Accept( inventory ) {
      mPrinter.print( Printer::Vending, StartReloading );
      _Accept( restocked ) {
        mPrinter.print( Printer::Vending, EndReloading );
      }

    } or _Accept( buy ) {

    } or _Accept( ~VendingMachine ) {
      return;
    }
  }
}