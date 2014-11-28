#include "nameserver.h"
#include "printer.h"
#include "student.h"
#include "vendingmachine.h"
#include "MPRNG.h"
#include "WATCardOffice.h"
#include <iostream>

using namespace std;

/******** Student constructor  ********/
Student::Student( Printer & printer, 
                  NameServer & nameServer, 
                  WATCardOffice & office, 
                  unsigned int id, 
                  unsigned int maxPurchases ) 
: mPrinter( printer ), mNameServer( nameServer ), mOffice( office ), mId( id ) {

  mPurchases = RAND( 1, maxPurchases );
  mFavouriteFlavour = RAND( VendingMachine::FlavoursCount - 1 );
}

/******** Student main  *******
  Purpose: adminsters student and their purchases 

  Returns: n/a

  Errors: Lost watcard, insufficient funds, or out of stock 

  Globals: n/a
*/
void Student::main() {

  mPrinter.print( Printer::Student, 
                  mId, 
                  Student::Starting,
                  mFavouriteFlavour,
                  mPurchases );

  // Creates an initial Watcard
  WATCard::FWATCard watcard = mOffice.create( mId, STARTING_BALANCE );

  WATCard* card = NULL;

  for ( unsigned int i = 0; i < mPurchases; ++i ) {
    // Gets the first machine to get drinks from
    VendingMachine *machine = mNameServer.getMachine( mId );
    mPrinter.print( Printer::Student,
                    mId,
                    ( char ) Student::Selecting,
                    machine->getId() );

    while ( true ) {

      yield( RAND( 1, 10 ) );

      while ( true ) {
        try {
          // Tries to get their card 
          card = watcard();
          // Card not lost, attempt to buy beverage
          break;

        } catch ( WATCardOffice::Lost &ex ) {
          // Courier loses the card
          mPrinter.print( Printer::Student,
                          mId,
                          ( char ) Student::Lost );
          watcard.cancel();
          watcard = mOffice.create( mId, STARTING_BALANCE );
        } 
      }

      try {
        // Purchases the drink from the vending machine
        machine->buy( ( VendingMachine::Flavours ) mFavouriteFlavour, *card );
        mPrinter.print( Printer::Student,
                        mId,
                        ( char ) Student::Buying,
                        card->getBalance() );
        // Successful purchase  
        break;

      } catch ( VendingMachine::Funds &ex ) {
        // Not enough funds on Watcard, must have more!
        watcard = mOffice.transfer( mId, STARTING_BALANCE + machine->cost(), card );

      } catch ( VendingMachine::Stock &ex ) {
        // Not enough in stock, move onto the next machine
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
