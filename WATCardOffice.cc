#include "bank.h"
#include "printer.h"
#include "MPRNG.h"
#include "WATCard.h"
#include "WATCardOffice.h"

using namespace std;

WATCardOffice::WATCardOffice( Printer & printer, Bank & bank, unsigned int numCouriers )
    : mPrinter( printer ), mBank( bank ), mNumCouriers( numCouriers ), mDone ( false ), mJob( NULL ) {

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
  mJob = new Job( sid, amount, NULL, mBank );
  mPrinter.print( Printer::WATCardOffice, ( char ) WATCardOffice::CreateWork, ( int ) sid, ( int ) amount );
  return mJob->mResult;
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ) {
  mJob = new Job( sid, amount, card, mBank );
  mPrinter.print( Printer::WATCardOffice, ( char ) WATCardOffice::TransferWork, ( int ) sid, ( int ) amount );
  return mJob->mResult;
}

WATCardOffice::Job* WATCardOffice::requestWork() {

  if ( mDone ) return NULL;
  Job* job = mJob;
  mJob = NULL;

  mPrinter.print( Printer::WATCardOffice, WATCardOffice::RequestWork );
  return job;
}
 
void WATCardOffice::main() {

  while ( true ) {
    _Accept( ~WATCardOffice ) {
      break;
    } or _When ( mJob != NULL ) _Accept( requestWork ) {
    } or _When ( mJob == NULL ) _Accept( create, transfer ) {
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

    mPrinter.print( Printer::Courier, mId, ( char ) Courier::StartTransfer, job->mSid, job->mAmount );

    job->mBank.withdraw( job->mSid, job->mAmount );
    if ( job->mWatcard == NULL ) job->mWatcard = new WATCard;
    job->mWatcard->deposit( job->mAmount );

    if ( RAND( LOSE_CARD_CHANCE - 1 ) == 0 ) {
      job->mResult.exception( new Lost );
      delete job->mWatcard;

    } else {
      job->mResult.delivery( job->mWatcard );
    }

    mPrinter.print( Printer::Courier, mId, ( char ) Courier::EndTransfer, job->mSid, job->mAmount );

    delete job;

  }

  mPrinter.print( Printer::Courier, mId, ( char ) Courier::Finished );

}
