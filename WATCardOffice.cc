#include "bank.h"
#include "printer.h"
#include "MPRNG.h"
#include "WATCard.h"
#include "WATCardOffice.h"

using namespace std;

WATCardOffice::WATCardOffice( Printer & printer, Bank & bank, unsigned int numCouriers )
    : mPrinter( printer ), mBank( bank ), mNumCouriers( numCouriers ), mDone ( false ) {

  mCouriers = new Courier*[mNumCouriers];
  for ( unsigned int i = 0; i < mNumCouriers; ++i ) {
    mCouriers[i] = new Courier( i, *this, printer );
  }

  mPrinter.print( Printer::WATCardOffice, WATCardOffice::Starting );
}

WATCardOffice::~WATCardOffice() {
  mDone = true;
  for ( unsigned int i = 0; i < mNumCouriers; ++i ) {
    _Accept( requestWork );
  }

  for ( unsigned int i = 0; i < mNumCouriers; ++i ) {
    delete mCouriers[i];
  }
  delete[] mCouriers;

  mPrinter.print( Printer::WATCardOffice, WATCardOffice::Finished );
}

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
  Args args = { sid, amount, NULL };
  Job* job = new Job( args, mBank );
  mJobs.push( job );
  mPrinter.print( Printer::WATCardOffice, ( char ) WATCardOffice::CreateWork, ( int ) sid, ( int ) amount );
  return job->mResult;
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ) {
  Args args = { sid, amount, card };
  Job* job = new Job( args, mBank );
  mJobs.push( job );
  mPrinter.print( Printer::WATCardOffice, ( char ) WATCardOffice::TransferWork, ( int ) sid, ( int ) amount );
  return job->mResult;
}

WATCardOffice::Job* WATCardOffice::requestWork() {

  if ( mDone ) return NULL;
  Job* job = mJobs.front();
  mJobs.pop();

  mPrinter.print( Printer::WATCardOffice, WATCardOffice::RequestWork );
  return job;
}

void WATCardOffice::main() {

  while ( true ) {
    _Accept( ~WATCardOffice ) {
      break;
    } or _When ( !mJobs.empty() ) _Accept( requestWork ) {
    } or _Accept( create, transfer ) {
    }
  }

}

WATCardOffice::Courier::Courier ( unsigned int id, WATCardOffice & office, Printer & printer ) 
    : mId ( id ), mOffice( office ), mPrinter( printer ) {
  mPrinter.print( Printer::Courier, id, ( char ) Courier::Starting );
}

void WATCardOffice::Courier::main() {

  while ( true ) {

    Job* job = mOffice.requestWork();
    if ( job == NULL ) break;

    Args args = job->mArgs;
    mPrinter.print( Printer::Courier, mId, ( char ) Courier::StartTransfer, args.mSid, args.mAmount );

    job->mBank.withdraw( args.mSid, args.mAmount );
    if ( args.mWatcard == NULL ) args.mWatcard = new WATCard;
    args.mWatcard->deposit( args.mAmount );

    if ( RAND( LOSE_CARD_CHANCE - 1 ) == 0 ) {
      job->mResult.exception( new Lost );
      delete args.mWatcard;

    } else {
      job->mResult.delivery( args.mWatcard );
    }

    delete job;

    mPrinter.print( Printer::Courier, mId, ( char ) Courier::EndTransfer, args.mSid, args.mAmount );

  }

  mPrinter.print( Printer::Courier, mId, ( char ) Courier::Finished );

}
