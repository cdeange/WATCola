#ifndef WATCARD_OFFICE_H_
#define WATCARD_OFFICE_H_

#include <queue>

#include "bank.h"
#include "printer.h"
#include "WATCard.h"

#define LOSE_CARD_CHANCE 6

_Task WATCardOffice {

  public:

    struct Args {
      unsigned int mSid;
      unsigned int mAmount;
      WATCard * mWatcard;
    };

    struct Job {
        Args mArgs;
        Bank & mBank;
        WATCard::FWATCard mResult;
        Job( Args args, Bank & bank ) : mArgs( args ), mBank( bank ) {}
    };

    _Task Courier {

      unsigned int mId;
      WATCardOffice & mOffice;
      Printer & mPrinter;

      void main();

      public:
      Courier ( unsigned int id, WATCardOffice & office, Printer & printer );

        enum State {
          Starting      = 'S', 
          StartTransfer = 't', 
          EndTransfer   = 'T', 
          Finished      = 'F'
        };
    };                 // communicates with bank

    Printer & mPrinter;
    Bank & mBank;
    unsigned int mNumCouriers;
    bool mDone;

    Courier** mCouriers;
    std::queue<Job*> mJobs;

    void main();

  public:

    _Event Lost {};                        // lost WATCard
    WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers );
    ~WATCardOffice();
    WATCard::FWATCard create( unsigned int sid, unsigned int amount );
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
    Job *requestWork();

    enum State {
      Starting     = 'S', 
      RequestWork  = 'W', 
      CreateWork   = 'C', 
      TransferWork = 'T', 
      Finished     = 'F'
    };

};

#endif
