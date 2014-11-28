#ifndef STUDENT_H_
#define STUDENT_H_

#include "nameserver.h"
#include "printer.h"
#include "WATCard.h"
#include "WATCardOffice.h"

_Task Student {

    // Printing state
    enum State {
      Starting  = 'S', 
      Selecting = 'V', 
      Buying    = 'B', 
      Lost      = 'L', 
      Finished  = 'F'
    };

    static const unsigned int STARTING_BALANCE = 5;

    Printer & mPrinter;
    NameServer & mNameServer;
    WATCardOffice & mOffice;            // Centralized office
    unsigned int mId;                   // Student id

    unsigned int mPurchases;            // Number of purchases
    unsigned int mFavouriteFlavour;     // Flavour to purchase

    void main();

  public:
    Student( Printer & printer, NameServer & nameServer, WATCardOffice & office, unsigned int id, 
      unsigned int maxPurchases );

};

#endif
