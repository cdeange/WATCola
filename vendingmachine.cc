#include "vendingmachine.h"
#include "nameserver.h"
#include <iostream>

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

  mPrinter.print( Printer::Vending, mId, ( char ) VendingMachine::Starting, sodaCost );
}

VendingMachine::~VendingMachine() {
  mPrinter.print( Printer::Vending, mId, ( char ) VendingMachine::Finished );
}

void VendingMachine::buy( Flavours flavour, WATCard &card ) {

  unsigned int balance = card.getBalance();

  if ( balance < mCost ) {
    uRendezvousAcceptor();
    _Throw Funds();

  } else if ( mInventory[flavour] <= 0 ) {
    uRendezvousAcceptor();
    _Throw Stock();
  }

  card.withdraw( mCost );
  mInventory[flavour]--;

  mPrinter.print( Printer::Vending, VendingMachine::Buying, flavour, mInventory[flavour] );
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

  mNameServer.VMregister( this );
  
  while( true ) {
    _Accept( inventory ) {
      mPrinter.print( Printer::Vending, VendingMachine::StartReloading );
      _Accept( restocked ) {
        mPrinter.print( Printer::Vending, VendingMachine::EndReloading );
      }

    } or _Accept( buy ) {

    } or _Accept( ~VendingMachine ) {
      return;
    }
  }
}