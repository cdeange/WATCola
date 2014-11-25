#include "nameserver.h"
#include "printer.h"
#include "student.h"
#include "vendingmachine.h"
#include "MPRNG.h"
#include "WATCardOffice.h"

using namespace std;

Student::Student( Printer & printer, NameServer & nameServer, WATCardOffice & office, unsigned int id, 
  unsigned int maxPurchases ) : mPrinter( printer ), mNameServer( nameServer ), mOffice( office ), mId( id ) {

  mPurchases = RAND( maxPurchases );
  mFavouriteFlavour = RAND( VendingMachine::FlavoursCount );
}

void Student::main() {

  mWatcard = mOffice.create( mId, STARTING_BALANCE );

  for ( unsigned int i = 0; i < mPurchases; ++i ) {
    yield( RAND( 1, 10 ) );

    VendingMachine *machine = mNameServer.getMachine( mId );

    while ( true ) {
      try {
        machine->buy( ( VendingMachine::Flavours ) mFavouriteFlavour, *( mWatcard() ) );

        // Successful purchase        
        break;

      } catch ( VendingMachine::Funds &ex ) {
        mWatcard = mOffice.transfer( mId, STARTING_BALANCE + machine->cost(), mWatcard() );

      } catch ( VendingMachine::Stock &ex ) {
        machine = mNameServer.getMachine( mId );

      } catch ( WATCardOffice::Lost &ex ) {
        mWatcard = mOffice.create( mId, STARTING_BALANCE );
      }
    }

  }

}
