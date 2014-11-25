#ifndef NAMESERVER_H_
#define NAMESERVER_H_

#include "printer.h"
#include "vendingmachine.h"

_Task NameServer {
    void main();

  public:
    NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents );
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
