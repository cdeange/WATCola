#ifndef PRINTER_H
#define PRINTER_H

#include <vector>

_Monitor Printer {
  
  public:
    enum Kind { 
      Parent, 
      WATCardOffice, 
      NameServer, 
      Truck, 
      BottlingPlant, 
      Student, 
      Vending, 
      Courier
    };

    Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
    ~Printer();
    void print( Kind kind, char state );
    void print( Kind kind, char state, int value1 );
    void print( Kind kind, char state, int value1, int value2 );
    void print( Kind kind, unsigned int lid, char state );
    void print( Kind kind, unsigned int lid, char state, int value1 );
    void print( Kind kind, unsigned int lid, char state, int value1, int value2 );

    const unsigned int mStudentStartIndex;
    const unsigned int mMachineStartIndex;
    const unsigned int mCourierStartIndex;
    vector<PrintInfo> mPrintList;

  private:
    void printIfFlush( unsigned int index, PrintInfo & replaceInfo );
    void printLineAndFlushBuffer();
    void printFinishLine( unsigned int index );
    unsigned int getPrintIndex( Kind kind, unsigned int id = 0 );

    union PrintData {
      int mNumData;
      int mFirst;
      int mSecond;

      PrintData();
    };

    struct PrintInfo {
      Kind mKind
      char mState;
      PrintData mData;
      bool mHasData;

      PrintInfo();
      PrintInfo( Kind & );
      PrintInfo( Kind &, char, PrintData & );

    };


};

#endif
