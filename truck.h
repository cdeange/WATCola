#ifndef TRUCK_H_
#define TRUCK_H_

#include "nameserver.h"
#include "plant.h"
#include "printer.h"

_Task Truck {
  void main();
  bool hasCargo();

  // Printing state
  enum State {
    Starting     = 'S', 
    Pickup       = 'P', 
    Begin        = 'd', 
    Unsuccessful = 'U', 
    Delivery     = 'D', 
    Finished     = 'F'
  };

  Printer & mPrinter;
  NameServer & mNameServer;
  BottlingPlant & mPlant;             // Plant where the truck gets shipments from
  unsigned int mNumVending;           // Number of vending machines to restock
  unsigned int mMaxStockPerFlavour;   // Max number of drinks to restock

  unsigned int mStartVending;         // Machine index to start restocking in next cycle
  unsigned int mCargo[VendingMachine::FlavoursCount];

public:
  Truck( Printer &prt, 
         NameServer &nameServer, 
         BottlingPlant &plant,
         unsigned int numVendingMachines, 
         unsigned int maxStockPerFlavour );
  ~Truck();
};

#endif
