#ifndef PRINTER_H
#define PRINTER_H

#include <vector>


_Monitor / _Cormonitor Printer {
  

  public:
    enum Kind { 
      Parent, 
      WATCardOffice, 
      NameServer, 
      Truck, 
      BottlingPlant, 
      Student, Vending, 
      Courier };

    Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
    void print( Kind kind, char state );
    void print( Kind kind, char state, int value1 );
    void print( Kind kind, char state, int value1, int value2 );
    void print( Kind kind, unsigned int lid, char state );
    void print( Kind kind, unsigned int lid, char state, int value1 );
    void print( Kind kind, unsigned int lid, char state, int value1, int value2 );

    const unsigned int mStudentStartIndex;
    const unsigned int mMachineStartIndex;
    vector<PrintInfo> mPrintList;

  private:
    struct TwoInfo {
      // shit with two things
    };
    struct OneInfo {
      // shit with one thing
    };
    
    union PrintData {
      
    };

    struct PrintInfo {
      Kind mPrintKind;
      PrintData
    };
};

#endif
