#ifndef WATCARD_OFFICE_H_
#define WATCARD_OFFICE_H_

#include <queue>
#include <memory>
#include <vector>

#include "bank.h"
#include "printer.h"
#include "WATCard.h"


/******** WATCardOffice class  *******
  Purpose: Administers tasks between money flowing from students to bank using couriers
*/

_Task WATCardOffice {

    // Work types for the courier
    enum WorkType {
      TRANSFER,
      CREATE,
      FINISH
    };

    // Arguments for the courier
    struct Args {
      Args( WorkType type, unsigned int sid, unsigned int amount, WATCard * card )
          : mType( type ), mSid( sid ), mAmount( amount ), mCard( card ) {
      }

      WorkType mType;         // Work type
      unsigned int mSid;      // Student Id requesting the funds
      unsigned int mAmount;   // Amount of funds from student 
      WATCard * mCard;        // Newly or original card
    };

    struct Job {
      Args mArgs;                 // marshalled arguments and return future
      WATCard::FWATCard mResult;  // return future
      Job( Args args ) : mArgs( args ) {} 
    };


    /******** Courier transfer  *******
      Purpose: Created by WATCardOffice to run errands to the bank for students
    */
    _Task Courier {
        // States for printing
        enum State {
          Starting      = 'S', 
          StartTransfer = 't', 
          EndTransfer   = 'T', 
          Finished      = 'F'
        };

        static const unsigned int LOSE_CARD_CHANCE = 6;

        unsigned int mId;         // Courier Id
        WATCardOffice & mOffice;  // Watcard office 
        Printer & mPrinter;       // Printer
        Bank & mBank;             // Bank for monies

        void main();

      public:
        Courier ( unsigned int id, WATCardOffice & office, Printer & printer, Bank & bank );
    };

    Printer & mPrinter;               
    Bank & mBank;                     
    unsigned int mNumCouriers;
    std::vector<Courier*> mCouriers;      // Courier pool
    std::queue<Job *> mJobs;              // List of queued up jobs

    void main();

    // Printing state for WATCardOffice
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
