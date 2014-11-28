#ifndef NAMESERVER_H_
#define NAMESERVER_H_

#include "printer.h"
#include "vendingmachine.h"

/******** Nameserver class  *******
  Purpose: Manages names and mapping between student and machines
*/
_Task NameServer {

    Printer & mPrinter;                 // Printer
    unsigned int mNumVendingMachines;   // Number of vending machines in total
    unsigned int mNumStudents;          // Number of students in total
    unsigned int mVMIndex;              // Used for ordering of student to machine mapping

    int* mStudents;                     // List of student ids to their machines
    VendingMachine** mMachines;         // List of machines

    void main();

    // States for printing
    enum State {
      Starting = 'S', 
      Register = 'R', 
      New      = 'N', 
      Finished = 'F'
    };
    
  public:
    NameServer( Printer & printer, unsigned int numVendingMachines, unsigned int numStudents );
    ~NameServer();
    void VMregister( VendingMachine *vendingmachine );
    VendingMachine *getMachine( unsigned int id );
    VendingMachine **getMachineList();


};

#endif
