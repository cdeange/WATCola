#ifndef STUDENT_H_
#define STUDENT_H_

#include "nameserver.h"
#include "printer.h"
#include "WATCard.h"
#include "WATCardOffice.h"

#define STARTING_BALANCE 5

_Task Student {

    enum State {
      Starting  = 'S', 
      Selecting = 'V', 
      Buying    = 'B', 
      Lost      = 'L', 
      Finished  = 'F'
    };

    Printer & mPrinter;
    NameServer & mNameServer;
    WATCardOffice & mOffice;
    unsigned int mId;

    unsigned int mPurchases;
    unsigned int mFavouriteFlavour;

    WATCard * getWATCard( unsigned int balance, WATCard * card );

    void main();

  public:
    Student( Printer & printer, NameServer & nameServer, WATCardOffice & office, unsigned int id, 
      unsigned int maxPurchases );

};

#endif
