#ifndef TRUCK_H_
#define TRUCK_H_

#include "nameserver.h"
#include "plant.h"
#include "printer.h"

_Task Truck {
  void main();
  bool hasCargo();

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
  BottlingPlant & mPlant;
  unsigned int mNumVending;
  unsigned int mMaxStockPerFlavour;

  unsigned int mStartVending;
  unsigned int mCargo[VendingMachine::FlavoursCount];

public:
  Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
         unsigned int numVendingMachines, unsigned int maxStockPerFlavour );

};

#endif
