#ifndef VENDING_MACHINE_H_
#define VENDING_MACHINE_H_

#include <uFuture.h>
#include <utility>

#include "printer.h"
#include "WATCard.h"

_Task NameServer;

_Task VendingMachine {

  public:
    enum Flavours {
      Black_Cherry, 
      Cream_Soda, 
      Root_Beer, 
      Lime, 
      FlavoursCount
    };                 // flavours of soda (YOU DEFINE)

    _Event Funds {};                       // insufficient funds
    _Event Stock {};                       // out of stock for particular flavour
    VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
                    unsigned int maxStockPerFlavour );
    ~VendingMachine();
    void buy( Flavours flavour, WATCard &card );
    unsigned int *inventory();
    void restocked();
    _Nomutex unsigned int cost();
    _Nomutex unsigned int getId();

  private:

    enum PurchaseResult {
      Success = 0,
      Stock_Missing = 1,
      Funds_Missing = 2
    };

    enum State {
      Starting       = 'S', 
      StartReloading = 'r', 
      EndReloading   = 'R', 
      Buying         = 'B', 
      Finished       = 'F'
    };

    void main();

    std::pair<Flavours, WATCard *> mPurchase;
    uCondition mLock;
    PurchaseResult mPurchaseResult;

    Printer & mPrinter;
    NameServer & mNameServer;
    unsigned int mId;
    unsigned int mCost;

    unsigned int mMaxStockPerFlavor;

    unsigned int mInventory[FlavoursCount];

};

#endif
