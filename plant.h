#ifndef BOTTLING_PLANT_H_
#define BOTTLING_PLANT_H_

#include "nameserver.h"
#include "printer.h"

_Task Truck;

/******** BottlingPlant getShipment  *******
  Purpose: To create cargos of drinks
*/
_Task BottlingPlant {
  public:
    _Event Shutdown {};                    // shutdown plant
    BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                 unsigned int timeBetweenShipments );
    ~BottlingPlant();
    void getShipment( unsigned int cargo[] );

  private:

    void createProductionRun();
    void main();
    
    // Printing states
    enum State {
      Starting   = 'S', 
      Generating = 'G', 
      Pickup     = 'P', 
      Finished   = 'F'
    };
    
    Printer & mPrinter;                         // Printer
    NameServer & mNameServer;                   // Name Server
    unsigned int mNumVendingMachines;           // Total number of vending machines
    unsigned int mMaxShippedPerFlavour;         // Maximum number of shipped flavours per round
    unsigned int mMaxStockPerFlavour;           // Max stock per flavour at the vending machines
    unsigned int mTimeBetweenShipments;         // Time to yield for simulation between shipments
    bool mShutdown;                             // Notify 
    int mStock[VendingMachine::FlavoursCount];  // Flavours stock

    Truck * mTruck;                             // Plant's truck

};

#endif
