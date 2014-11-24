#include "printer.h"
#include <string>

Printer::Printer( unsigned int numStudents,
                  unsigned int numVendingMachines,
                  unsigend int numCouriers ) {
  string extralist[5] = {
    "Parent",
    "WATOff",
    "Names",
    "Truck",
    "Plant"
  };

  // Assumes something else comes after
  // aka num____ != 0
  for( int i = 0; i < extralist; i++ ) {
    cout << extralist[i] << "\t";
  }

  // Students
  for( int i = 0; i < numStudents; i++ ) {
    cout << "Stud" << i << "\t";
  }

  //




}