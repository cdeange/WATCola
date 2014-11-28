#include "bank.h"
#include "printer.h"
#include "MPRNG.h"
#include "WATCard.h"
#include "WATCardOffice.h"
#include <iostream>

using namespace std;

WATCardOffice::WATCardOffice( Printer & printer, Bank & bank, unsigned int numCouriers )
    : mPrinter( printer ), mBank( bank ), mNumCouriers( numCouriers ) {
}

WATCardOffice::~WATCardOffice() {
}

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
  Job * job = new Job( sid, amount, NULL );
  mJobs.push( job );
  mPrinter.print( Printer::WATCardOffice, ( char ) WATCardOffice::CreateWork, ( int ) sid, ( int ) amount );
  return job->mResult;
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ) {
  Job * job = new Job( sid, amount, card );
  mJobs.push( job );
  mPrinter.print( Printer::WATCardOffice, ( char ) WATCardOffice::TransferWork, ( int ) sid, ( int ) amount );
  return job->mResult;
}

WATCardOffice::Job* WATCardOffice::requestWork() {

  Job* job = mJobs.front();
  mJobs.pop();

  mPrinter.print( Printer::WATCardOffice, WATCardOffice::RequestWork );
  return job;
}

void WATCardOffice::main() {
  mPrinter.print( Printer::WATCardOffice, WATCardOffice::Starting );

  mCouriers = new Courier*[mNumCouriers];
  for ( unsigned int i = 0; i < mNumCouriers; ++i ) {
    mCouriers[i] = new Courier( i, *this, mPrinter, mBank );
  }

  while ( true ) {
    _Accept( ~WATCardOffice ) {
      for ( unsigned int i = 0; i < mNumCouriers; ++i ) {
        mJobs.push( NULL );
      }
      while ( !mJobs.empty() ) {
        _Accept( requestWork );
      }
      break;

    } or _When ( !mJobs.empty() ) _Accept( requestWork ) {
    } or _Accept( create, transfer ) {
    }
  }

  for ( unsigned int i = 0; i < mNumCouriers; ++i ) {
    delete mCouriers[i];
  }
  delete[] mCouriers;

  mPrinter.print( Printer::WATCardOffice, WATCardOffice::Finished );
}

WATCardOffice::Courier::Courier ( unsigned int id, WATCardOffice & office, Printer & printer, Bank & bank ) 
    : mId ( id ), mOffice( office ), mPrinter( printer ), mBank( bank ) {
}

void WATCardOffice::Courier::main() {

  mPrinter.print( Printer::Courier, mId, ( char ) Courier::Starting );

  while ( true ) {

    Job* job = mOffice.requestWork();
    if ( job == NULL ) break;

    Args args = job->mArgs;
    mPrinter.print( Printer::Courier, mId, ( char ) Courier::StartTransfer, args.mSid, args.mAmount );

    if ( RAND( LOSE_CARD_CHANCE - 1 ) == 0 ) {
      delete args.mWatcard;
      job->mResult.exception( new Lost );

    } else {
      if ( args.mWatcard == NULL ) {
        args.mWatcard = new WATCard;
      }
      mBank.withdraw( args.mSid, args.mAmount );
      args.mWatcard->deposit( args.mAmount );
      job->mResult.delivery( args.mWatcard );
    }

    mPrinter.print( Printer::Courier, mId, ( char ) Courier::EndTransfer, args.mSid, args.mAmount );

    delete job;

  }

  mPrinter.print( Printer::Courier, mId, ( char ) Courier::Finished );

}
