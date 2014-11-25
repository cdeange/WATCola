#ifndef BOTTLING_PLANT_H_
#define BOTTLING_PLANT_H_

#include "nameserver.h"
#include "printer.h"

_Task BottlingPlant {
  public:
    _Event Shutdown {};                    // shutdown plant
    BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                 unsigned int timeBetweenShipments );
    ~BottlingPlant();
    void getShipment( unsigned int cargo[] );

  private:
    void main();
    enum State {
      Starting   = 'S', 
      Generating = 'G', 
      Pickup     = 'P', 
      Finished   = 'F'
    };
    
    Printer & mPrinter;
    NameServer & mNameServer;
    unsigned int mNumVendingMachines;
    unsigned int mMaxShippedPerFlavour;
    unsigned int mMaxStockPerFlavour;
    unsigned int mTimeBetweenShipments;
    bool shutdown;

};

#endif
