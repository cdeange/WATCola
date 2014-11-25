#ifndef VENDING_MACHINE_H_
#define VENDING_MACHINE_H_

#include <uFuture.h>

#include "printer.h"
#include "WATCard.h"

_Task NameServer;

_Task VendingMachine {
    enum State {
      Starting       = 'S', 
      StartReloading = 'r', 
      EndReloading   = 'R', 
      Buying         = 'B', 
      Finished       = 'F'
    };
    void main();

    unsigned int * mInventory;
    unsigned int mCost;
    unsigned int mId;

  public:
    enum Flavours {
      Cola = 0, 
      // ADD MORE HERE
      FlavoursCount = 1
    };                 // flavours of soda (YOU DEFINE)

    _Event Funds {};                       // insufficient funds
    _Event Stock {};                       // out of stock for particular flavour
    VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
                    unsigned int maxStockPerFlavour );
    void buy( Flavours flavour, WATCard &card );
    unsigned int *inventory();
    void restocked();
    _Nomutex unsigned int cost();
    _Nomutex unsigned int getId();

};

#endif
