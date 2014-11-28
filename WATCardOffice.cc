#include "bank.h"
#include "printer.h"
#include "MPRNG.h"
#include "WATCard.h"
#include "WATCardOffice.h"
#include <iostream>

using namespace std;

WATCardOffice::WATCardOffice( Printer & printer, Bank & bank, unsigned int numCouriers )
    : mPrinter( printer ), mBank( bank ), mNumCouriers( numCouriers ), mDone ( false ) {
  mJob = NULL;
  mAcceptingJob = true;
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
}

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
  mAcceptingJob = false;
  Job * job = new Job( sid, amount, new WATCard(), mBank );
  // mJobs.push( job );
  mJob = job;
  mPrinter.print( Printer::WATCardOffice, ( char ) WATCardOffice::CreateWork, ( int ) sid, ( int ) amount );
  return job->mResult;
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ) {
  mAcceptingJob = false;
  Job * job = new Job( sid, amount, card, mBank );
  // mJobs.push( job );
  mJob = job;
  mPrinter.print( Printer::WATCardOffice, ( char ) WATCardOffice::TransferWork, ( int ) sid, ( int ) amount );
  return job->mResult;
}

WATCardOffice::Job* WATCardOffice::requestWork() {

  if ( mDone ) return NULL;
  // Job* job = mJobs.front();
  // mJobs.pop();
  Job * job = mJob;
  mJob = NULL;
  mAcceptingJob = true;

  mPrinter.print( Printer::WATCardOffice, WATCardOffice::RequestWork );
  return job;
}

void WATCardOffice::main() {
  mPrinter.print( Printer::WATCardOffice, WATCardOffice::Starting );

  mCouriers = new Courier*[mNumCouriers];
  for ( unsigned int i = 0; i < mNumCouriers; ++i ) {
    mCouriers[i] = new Courier( i, *this, mPrinter );
  }

  while ( true ) {
    _Accept( ~WATCardOffice ) {
      break;
    // } or _When ( !mJobs.empty() ) _Accept( requestWork ) {
    // } or _Accept( create, transfer ) {
    } or _When (  mAcceptingJob ) _Accept ( create, transfer ) {
    } or _When ( !mAcceptingJob ) _Accept ( requestWork ) {
    }
  }

  mPrinter.print( Printer::WATCardOffice, WATCardOffice::Finished );
}

WATCardOffice::Courier::Courier ( unsigned int id, WATCardOffice & office, Printer & printer ) 
    : mId ( id ), mOffice( office ), mPrinter( printer ) {
}

void WATCardOffice::Courier::main() {

  mPrinter.print( Printer::Courier, mId, ( char ) Courier::Starting );

  while ( true ) {

    Job* job = mOffice.requestWork();
    if ( job == NULL ) break;

    mPrinter.print( Printer::Courier, mId, ( char ) Courier::StartTransfer, job->mSid, job->mAmount );

    if ( RAND( LOSE_CARD_CHANCE - 1 ) == 0 ) {
      // if ( job->mWatcard ) delete job->mWatcard;
      job->mResult.exception( new Lost );

    } else {
      // if ( job->mWatcard == NULL ) {
      //   job->mWatcard = new WATCard;
      // }
      job->mBank.withdraw( job->mSid, job->mAmount );
      job->mWatcard->deposit( job->mAmount );
      job->mResult.delivery( job->mWatcard );
    }

    mPrinter.print( Printer::Courier, mId, ( char ) Courier::EndTransfer, job->mSid, job->mAmount );

    delete job;

  }

  mPrinter.print( Printer::Courier, mId, ( char ) Courier::Finished );

}
