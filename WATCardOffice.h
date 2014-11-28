#ifndef WATCARD_OFFICE_H_
#define WATCARD_OFFICE_H_

#include <queue>

#include "bank.h"
#include "printer.h"
#include "WATCard.h"

#define LOSE_CARD_CHANCE 6

_Task WATCardOffice {

    struct Args {
      unsigned int mSid;
      unsigned int mAmount;
      WATCard * mWatcard;
      Args( unsigned int sid, unsigned int amount, WATCard * watcard )
            : mSid( sid ), mAmount( amount ), mWatcard( watcard ) {}
    };

    struct Job {
      Args mArgs;
      WATCard::FWATCard mResult;
        Job( unsigned int sid, unsigned int amount, WATCard * watcard )
            : mArgs( sid, amount, watcard ) {}
    };

    _Task Courier {
        enum State {
          Starting      = 'S', 
          StartTransfer = 't', 
          EndTransfer   = 'T', 
          Finished      = 'F'
        };

        unsigned int mId;
        WATCardOffice & mOffice;
        Printer & mPrinter;
        Bank & mBank;

        void main();

      public:
        Courier ( unsigned int id, WATCardOffice & office, Printer & printer, Bank & bank );
    };                 // communicates with bank

    Printer & mPrinter;
    Bank & mBank;
    unsigned int mNumCouriers;
    bool mDone;

    Courier** mCouriers;
    std::queue<Job *> mJobs;

    void main();

    enum State {
      Starting     = 'S', 
      RequestWork  = 'W', 
      CreateWork   = 'C', 
      TransferWork = 'T', 
      Finished     = 'F'
    };

  public:

    _Event Lost {};                        // lost WATCard
    WATCardOffice( Printer & printer, Bank & bank, unsigned int numCouriers );
    ~WATCardOffice();
    WATCard::FWATCard create( unsigned int sid, unsigned int amount );
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
    Job *requestWork();

};

#endif
