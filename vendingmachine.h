#ifndef VENDING_MACHINE_H_
#define VENDING_MACHINE_H_

#include <uFuture.h>

#include "printer.h"
#include "WATCard.h"

_Task NameServer;

_Task VendingMachine {
    void main();
  public:
    enum Flavours {
      COLA = 0, 
      // ADD MORE HERE
      FLAVOURS_COUNT
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

    enum State {
      Starting       = 'S', 
      StartReloading = 'r', 
      EndReloading   = 'R', 
      Buying         = 'B', 
      Finished       = 'F'
    };
};

#endif
