#ifndef TRUCK_H_
#define TRUCK_H_

#include "nameserver.h"
#include "plant.h"
#include "printer.h"

_Task Truck {
    void main();

  public:
    Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
           unsigned int numVendingMachines, unsigned int maxStockPerFlavour );

    enum State {
      Starting     = 'S', 
      Pickup       = 'P', 
      Begin        = 'd', 
      Unsuccessful = 'U', 
      Delivery     = 'D', 
      Finished     = 'F'
    };

};

#endif
