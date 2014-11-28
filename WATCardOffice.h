#ifndef WATCARD_OFFICE_H_
#define WATCARD_OFFICE_H_

#include <queue>
#include <memory>
#include <vector>

#include "bank.h"
#include "printer.h"
#include "WATCard.h"

#define LOSE_CARD_CHANCE 6

_Task WATCardOffice {

    enum WorkType {
      TRANSFER,
      CREATE,
      FINISH
    };

    struct Args {
      Args( WorkType type, unsigned int sid, unsigned int amount, WATCard * card )
          : mType( type ), mSid( sid ), mAmount( amount ), mCard( card ) {
      }

      WorkType mType;
      unsigned int mSid;
      unsigned int mAmount;
      WATCard * mCard;
    };

    struct Job {
      Args mArgs;                 // marshalled arguments and return future
      WATCard::FWATCard mResult;  // return future
      Job( Args args ) : mArgs( args ) {} 
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
    };

    Printer & mPrinter;
    Bank & mBank;
    unsigned int mNumCouriers;
    std::vector<Courier*> mCouriers;
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

    _Event Lost {};
    WATCardOffice( Printer & printer, Bank & bank, unsigned int numCouriers );
    ~WATCardOffice();
    WATCard::FWATCard create( unsigned int sid, unsigned int amount );
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
    Job *requestWork();

};

#endif
