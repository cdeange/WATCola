#include "nameserver.h"
#include "printer.h"
#include "student.h"
#include "vendingmachine.h"
#include "MPRNG.h"
#include "WATCardOffice.h"

using namespace std;

Student::Student( Printer & printer, 
                  NameServer & nameServer, 
                  WATCardOffice & office, 
                  unsigned int id, 
                  unsigned int maxPurchases ) 
: mPrinter( printer ), mNameServer( nameServer ), mOffice( office ), mId( id ) {

  mPurchases = RAND( 1, maxPurchases );
  mFavouriteFlavour = RAND( VendingMachine::FlavoursCount - 1 );
  mPrinter.print( Printer::Student, 
                  id, 
                  Student::Starting,
                  mFavouriteFlavour,
                  mPurchases );
}

void Student::main() {

  mWatcard = mOffice.create( mId, STARTING_BALANCE );

  for ( unsigned int i = 0; i < mPurchases; ++i ) {
    yield( RAND( 1, 10 ) );

    VendingMachine *machine = mNameServer.getMachine( mId );
    mPrinter.print( Printer::Student,
                    mId,
                    ( char ) Student::Selecting,
                    machine->getId() );

    while ( true ) {
      try {
        WATCard* card = mWatcard();
        machine->buy( ( VendingMachine::Flavours ) mFavouriteFlavour, *card );
        mPrinter.print( Printer::Student,
                        mId,
                        ( char ) Student::Buying,
                        card->getBalance() );
        // Successful purchase        
        break;

      } catch ( VendingMachine::Funds &ex ) {
        mWatcard = mOffice.transfer( mId, STARTING_BALANCE + machine->cost(), mWatcard() );

      } catch ( VendingMachine::Stock &ex ) {
        machine = mNameServer.getMachine( mId );
        mPrinter.print( Printer::Student,
                        mId,
                        ( char ) Student::Selecting,
                        machine->getId() );

      } catch ( WATCardOffice::Lost &ex ) {
        mPrinter.print( Printer::Student,
                        mId,
                        ( char ) Student::Lost );

        mWatcard = mOffice.create( mId, STARTING_BALANCE );
      }
    }
  }

  mPrinter.print( Printer::Student, mId, ( char ) Student::Finished );

  delete mWatcard;
}
