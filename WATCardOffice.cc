#include "bank.h"
#include "printer.h"
#include "MPRNG.h"
#include "WATCard.h"
#include "WATCardOffice.h"

#include <iostream>
#include <memory>

using namespace std;

/******** WATCardOffice constructor  *******
  Purpose: Constructs a WATCardOffice object

  Returns: a WATCardOffice instance

  Errors: n/a

  Globals: n/a
*/
WATCardOffice::WATCardOffice( Printer & printer, Bank & bank, unsigned int numCouriers )
    : mPrinter( printer ), mBank( bank ), mNumCouriers( numCouriers ) {

  // Creating courier pool
  for( unsigned int i = 0; i < mNumCouriers; i++ ) {
    mCouriers.push_back( new Courier( i, (*this), printer, mBank ) );
  }
}

/******** WATCardOffice destructor  *******
  Purpose: Destructs a WATCardOffice object

  Returns: n/a

  Errors: n/a

  Globals: n/a
*/
WATCardOffice::~WATCardOffice() {

  // Tells the couriers to finish their last run
  for( unsigned int i = 0; i < mNumCouriers; i++ ) {
    Args arg( WATCardOffice::FINISH, 0, 0, NULL );
    Job * job = new Job( arg );
    mJobs.push( job );
  }

  // Clears the job queue
  while ( !mJobs.empty() ) {
    _Accept( requestWork );
  }

  // Deletes all couriers
  for( unsigned int i = 0; i < mNumCouriers; i++ ) {
    delete mCouriers[i];
    mCouriers[i] = NULL;
  }

}


/******** WATCardOffice create  *******
  Purpose: Called by student to create a Watcard at the beginning of their life

  Returns: a future Watcard that will be filled by the courier

  Errors: n/a

  Globals: member variables
*/
WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
  Args args( WATCardOffice::CREATE, sid, amount, new WATCard() );
  Job * job = new Job( args );
  mJobs.push( job );
  mPrinter.print( Printer::WATCardOffice, ( char ) WATCardOffice::CreateWork, ( int ) sid, ( int ) amount );
  return job->mResult;
}

/******** WATCardOffice transfer  *******
  Purpose: Called by student to transfer funds from bank into Watcard

  Returns: a future Watcard that will be filled by the courier with more money

  Errors: n/a

  Globals: member variables
*/
WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ) {
  Args args( WATCardOffice::TRANSFER, sid, amount, card );
  Job * job = new Job( args );
  mJobs.push( job );
  mPrinter.print( Printer::WATCardOffice, ( char ) WATCardOffice::TransferWork, ( int ) sid, ( int ) amount );
  return job->mResult;
}

/******** WATCardOffice requestWork  *******
  Purpose: Called by courier to request work that is given by the students

  Returns: A job for the courier to do

  Errors: n/a

  Globals: member variables
*/
WATCardOffice::Job* WATCardOffice::requestWork() {
  Job* job = mJobs.front();
  mJobs.pop();
  mPrinter.print( Printer::WATCardOffice, WATCardOffice::RequestWork );
  return job;
}

/******** WATCardOffice main  *******
  Purpose: The administrative task main that takes care of who can call on its methods

  Returns: n/a

  Errors: n/a

  Globals: member variables
*/
void WATCardOffice::main() {
  mPrinter.print( Printer::WATCardOffice, WATCardOffice::Starting );

  while ( true ) {
    _Accept( ~WATCardOffice ) {
      break;
    } or _When ( !mJobs.empty() ) _Accept( requestWork ) {
    } or _Accept( create, transfer ) {
    }
  }

  mPrinter.print( Printer::WATCardOffice, WATCardOffice::Finished );
}

/******** Courier constructir  *******
  Purpose: Constructs a courier object

  Returns: Courier instance

  Errors: n/a

  Globals: n/a
*/
WATCardOffice::Courier::Courier ( unsigned int id, WATCardOffice & office, Printer & printer, Bank & bank ) 
    : mId ( id ), mOffice( office ), mPrinter( printer ), mBank( bank ) {
}


/******** Courier main  *******
  Purpose: Takes care of tasks for the Courier

  Returns: n/a

  Errors: n/a

  Globals: member variables
*/
void WATCardOffice::Courier::main() {

  mPrinter.print( Printer::Courier, mId, ( char ) Courier::Starting );

  while ( true ) {
    // Job that is used here and in the create / transfer functions
    unique_ptr<Job> job( mOffice.requestWork() ); // Unique pointer to manage memory

    Args args = job->mArgs;

    // Last task for courier
    if ( args.mType == WATCardOffice::FINISH ) {
      delete args.mCard;
      break;
    }

    mPrinter.print( Printer::Courier, mId, ( char ) Courier::StartTransfer, args.mSid, args.mAmount );
    mBank.withdraw( args.mSid, args.mAmount );
    args.mCard->deposit( args.mAmount );
    
    if( RAND( 1, LOSE_CARD_CHANCE ) == 1 ) {
      delete args.mCard;
      job->mResult.exception( new Lost );
    } else {
      job->mResult.delivery( args.mCard );
    }
    mPrinter.print( Printer::Courier, mId, ( char ) Courier::EndTransfer, args.mSid, args.mAmount );

  }

  mPrinter.print( Printer::Courier, mId, ( char ) Courier::Finished );
}
