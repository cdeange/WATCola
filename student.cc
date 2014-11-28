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

void Student::main() {

  mPrinter.print( Printer::Student, 
                  mId, 
                  Student::Starting,
                  mFavouriteFlavour,
                  mPurchases );

  WATCard::FWATCard watcard = mOffice.create( mId, STARTING_BALANCE );

  WATCard* card = NULL;

  for ( unsigned int i = 0; i < mPurchases; ++i ) {

    VendingMachine *machine = mNameServer.getMachine( mId );
    mPrinter.print( Printer::Student,
                    mId,
                    ( char ) Student::Selecting,
                    machine->getId() );

    while ( true ) {

      yield( RAND( 1, 10 ) );

      while ( true ) {
        try {
          card = watcard();
          // Card not lost, attempt to buy beverage
          break;

        } catch ( WATCardOffice::Lost &ex ) {
          mPrinter.print( Printer::Student,
                          mId,
                          ( char ) Student::Lost );
          watcard.cancel();
          watcard = mOffice.create( mId, STARTING_BALANCE );
        } 
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
        watcard = mOffice.transfer( mId, STARTING_BALANCE + machine->cost(), card );

      } catch ( VendingMachine::Stock &ex ) {
        machine = mNameServer.getMachine( mId );
        mPrinter.print( Printer::Student,
                        mId,
                        ( char ) Student::Selecting,
                        machine->getId() );

      }
    }
  }

  delete card;

  mPrinter.print( Printer::Student, mId, ( char ) Student::Finished );
}
