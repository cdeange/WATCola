#include "nameserver.h"
#include "printer.h"
#include "student.h"
#include "vendingmachine.h"
#include "MPRNG.h"
#include "WATCardOffice.h"

#include <iostream>

using namespace std;

Student::Student( Printer & printer, 
                  NameServer & nameServer, 
                  WATCardOffice & office, 
                  unsigned int id, 
                  unsigned int maxPurchases ) 
: mPrinter( printer ), mNameServer( nameServer ), mOffice( office ), mId( id ) {

  mPurchases = RAND( 1, maxPurchases );
  mFavouriteFlavour = RAND( VendingMachine::FlavoursCount - 1 );
}

WATCard * Student::getWATCard( unsigned int balance, WATCard * card ) {

  WATCard * newCard = NULL;
  WATCard::FWATCard watcard;

  while ( true ) {

    try {

      if ( card == NULL ) {
        watcard = mOffice.create( mId, balance );
      } else {
        watcard = mOffice.transfer( mId, balance, card );
      }

      newCard = watcard();
      // Card was not lost!
      return newCard;

    } catch ( WATCardOffice::Lost &ex ) {
      delete newCard;
      mPrinter.print( Printer::Student,
                      mId,
                      ( char ) Student::Lost );
    }
  }
}

void Student::main() {

  mPrinter.print( Printer::Student, 
                  mId, 
                  Student::Starting,
                  mFavouriteFlavour,
                  mPurchases );

  bool refillNeeded = false;
  unsigned int refillBalance = STARTING_BALANCE;
  WATCard* card = NULL;

  for ( unsigned int i = 0; i < mPurchases; ++i ) {

    VendingMachine *machine = mNameServer.getMachine( mId );
    mPrinter.print( Printer::Student,
                    mId,
                    ( char ) Student::Selecting,
                    machine->getId() );

    while ( true ) {

      yield( RAND( 1, 10 ) );

      if ( card == NULL ) {
        card = getWATCard( refillBalance, NULL );
      } else if ( refillNeeded ) {
        card = getWATCard( refillBalance, card );
        refillNeeded = false;
      }

      try {

        machine->buy( ( VendingMachine::Flavours ) mFavouriteFlavour, *card );
        mPrinter.print( Printer::Student,
                        mId,
                        ( char ) Student::Buying,
                        card->getBalance() );
        // Successful purchase        
        break;

      } catch ( VendingMachine::Funds &ex ) {
        // cout << mId << "'s FUNDS card is " << card << endl;
        refillBalance = STARTING_BALANCE + machine->cost();
        refillNeeded = true;

      } catch ( VendingMachine::Stock &ex ) {
        machine = mNameServer.getMachine( mId );
        mPrinter.print( Printer::Student,
                        mId,
                        ( char ) Student::Selecting,
                        machine->getId() );
      }

    }
  }

  mPrinter.print( Printer::Student, mId, ( char ) Student::Finished );

  delete card;

}
