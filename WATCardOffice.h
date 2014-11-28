#ifndef WATCARD_OFFICE_H_
#define WATCARD_OFFICE_H_

#include <queue> 
#include <vector>

#include "bank.h"
#include "printer.h"
#include "WATCard.h"

using namespace std;

#define LOSE_CARD_CHANCE 6

_Task WATCardOffice {

    enum WorkType {
      TRANSFER,
      CREATE,
      FINISH
    };

    struct Args {
      Args( WorkType type,
            unsigned int sid,
            unsigned int amount,
            WATCard * card,
            Bank * bank) {
        mType = type;
        mSid = sid;
        mAmount = amount;
        mCard = card;
        mBank = bank;
      }

      WorkType mType;
      unsigned int mSid;
      unsigned int mAmount;
      WATCard * mCard;
      Bank * mBank;
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
        vector<WATCard* > mDeleteCards;

        void main();

      public:
        Courier ( unsigned int id, WATCardOffice & office, Printer & printer );
        ~Courier();
    };                 // communicates with bank

    Printer & mPrinter;
    Bank & mBank;
    unsigned int mNumCouriers;
    vector<Courier*> mCouriers;
    // bool mDone;

    // Courier** mCouriers;
    std::queue<Job *> mJobs;
    std::vector<Job *> mDeleteJobs;

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
