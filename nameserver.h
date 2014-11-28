#ifndef NAMESERVER_H_
#define NAMESERVER_H_

#include "printer.h"
#include "vendingmachine.h"

_Task NameServer {

    Printer & mPrinter;
    unsigned int mNumVendingMachines;
    unsigned int mNumStudents;
    unsigned int mVMIndex;

    int* mStudents;
    VendingMachine** mMachines;

    void main();

  public:
    NameServer( Printer & printer, unsigned int numVendingMachines, unsigned int numStudents );
    ~NameServer();
    void VMregister( VendingMachine *vendingmachine );
    VendingMachine *getMachine( unsigned int id );
    VendingMachine **getMachineList();

    enum State {
      Starting = 'S', 
      Register = 'R', 
      New      = 'N', 
      Finished = 'F'
    };
};

#endif
