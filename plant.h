#ifndef BOTTLING_PLANT_H_
#define BOTTLING_PLANT_H_

#include "nameserver.h"
#include "printer.h"

_Task BottlingPlant {
    void main();

  public:
    _Event Shutdown {};                    // shutdown plant
    BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                 unsigned int timeBetweenShipments );
    void getShipment( unsigned int cargo[] );

    enum State {
      Starting   = 'S', 
      Generating = 'G', 
      Pickup     = 'P', 
      Finished   = 'F'
    };

};

#endif
