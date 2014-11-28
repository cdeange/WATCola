#include "bank.h"
#include "printer.h"
#include "MPRNG.h"
#include "WATCard.h"
#include "WATCardOffice.h"
#include <iostream>

using namespace std;

WATCardOffice::WATCardOffice( Printer & printer, Bank & bank, unsigned int numCouriers )
    : mPrinter( printer ), mBank( bank ), mNumCouriers( numCouriers ) {

  for( unsigned int i = 0; i < mNumCouriers; i++ ) {
    mCouriers.push_back( new Courier( i, (*this), printer, mBank ) );
  }
}

WATCardOffice::~WATCardOffice() {

  for( unsigned int i = 0; i < mNumCouriers; i++ ) {
    Args arg( WATCardOffice::FINISH, 0, 0, NULL );
    Job * job = new Job( arg );
    mJobs.push( job );
    mDeleteJobs.push_back( job );
  }

  while ( !mJobs.empty() ) {
    _Accept( requestWork );
  }

  for( unsigned int i = 0; i < mNumCouriers; i++ ) {
    delete mCouriers[i];
    mCouriers[i] = NULL;
  }

  for( unsigned int i = 0; i < mDeleteJobs.size(); i++ ) {
    if( mDeleteJobs[i] != NULL ) 
      delete mDeleteJobs[i];
    mDeleteJobs[i] = NULL;
  }

}

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
  Args args( WATCardOffice::CREATE, sid, amount, NULL );
  Job * job = new Job( args );
  mJobs.push( job );
  mDeleteJobs.push_back(job);
  mPrinter.print( Printer::WATCardOffice, ( char ) WATCardOffice::CreateWork, ( int ) sid, ( int ) amount );
  return job->mResult;
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ) {
  Args args( WATCardOffice::TRANSFER, sid, amount, card );
  Job * job = new Job( args );
  mJobs.push( job );
  mDeleteJobs.push_back(job);
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

  while ( true ) {
    _Accept( ~WATCardOffice ) {
      break;
    } or _When ( !mJobs.empty() ) _Accept( requestWork ) {
    } or _Accept( create, transfer ) {
    }
  }

  mPrinter.print( Printer::WATCardOffice, WATCardOffice::Finished );
}

WATCardOffice::Courier::Courier ( unsigned int id, WATCardOffice & office, Printer & printer, Bank & bank ) 
    : mId ( id ), mOffice( office ), mPrinter( printer ), mBank( bank ) {
}

WATCardOffice::Courier::~Courier() {
  for( unsigned int i = 0; i < mDeleteCards.size(); i++ ) {
    if( mDeleteCards[i] != NULL ) {
      delete mDeleteCards[i];
    }
    mDeleteCards[i] = NULL;
  }
}
void WATCardOffice::Courier::main() {

  mPrinter.print( Printer::Courier, mId, ( char ) Courier::Starting );

  while ( true ) {
    Job * job = mOffice.requestWork();

    Args args = job->mArgs;
    if( args.mType == WATCardOffice::FINISH ) break;

    if( args.mType == WATCardOffice::CREATE ) {
      args.mCard = new WATCard();
      mDeleteCards.push_back( args.mCard );
    }

    mPrinter.print( Printer::Courier, mId, ( char ) Courier::StartTransfer, args.mSid, args.mAmount );
    mBank.withdraw( args.mSid, args.mAmount );
    args.mCard->deposit( args.mAmount );
    
    if( RAND(1, 6) == 1 ) {
      // delete args.mCard;
      job->mResult.exception( new Lost );
    } else {
      job->mResult.delivery( args.mCard );
    }
    mPrinter.print( Printer::Courier, mId, ( char ) Courier::EndTransfer, args.mSid, args.mAmount );

    // delete job;

  }

  mPrinter.print( Printer::Courier, mId, ( char ) Courier::Finished );
}
