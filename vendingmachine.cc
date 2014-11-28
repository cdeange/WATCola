#include "vendingmachine.h"
#include "nameserver.h"

#include <iostream>
#include <utility>

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

}

VendingMachine::~VendingMachine() {
}

void VendingMachine::buy( Flavours flavour, WATCard &card ) {

  mPurchase = make_pair( flavour, &card );
  mLock.wait();

  if ( mPurchaseResult == VendingMachine::Funds_Missing ) {
    uRendezvousAcceptor();
    _Throw Funds();

  } else if ( mPurchaseResult == VendingMachine::Stock_Missing ) {
    uRendezvousAcceptor();
    _Throw Stock();
  }

  mPrinter.print( Printer::Vending, VendingMachine::Buying, flavour, mInventory[flavour] );
}

unsigned int * VendingMachine::inventory() {
  return mInventory;
}

void VendingMachine::restocked() {
}

unsigned int VendingMachine::cost() {
  return mCost;
}

unsigned int VendingMachine::getId() {
  return mId;
}

void VendingMachine::main() {
  mPrinter.print( Printer::Vending, mId, ( char ) VendingMachine::Starting, mCost );

  while( true ) {
    _Accept( inventory ) {
      mPrinter.print( Printer::Vending, mId, (char)VendingMachine::StartReloading );
      _Accept( restocked ) {
        mPrinter.print( Printer::Vending, mId, (char)VendingMachine::EndReloading );
      }

    } or _Accept( buy ) {

      Flavours flavour = mPurchase.first;
      WATCard * card = mPurchase.second;

      if ( card->getBalance() < mCost ) {
        mPurchaseResult = VendingMachine::Funds_Missing;

      } else if ( mInventory[flavour] <= 0 ) {
        mPurchaseResult = VendingMachine::Stock_Missing;

      } else {
        mPurchaseResult = Success;
        card->withdraw( mCost );
        mInventory[flavour]--;
      }

      mLock.signalBlock();

    } or _Accept( ~VendingMachine ) {
      return;
    }
  }

  mPrinter.print( Printer::Vending, mId, ( char ) VendingMachine::Finished );
}