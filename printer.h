#ifndef PRINTER_H_
#define PRINTER_H_

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


    struct PrintData {
      int mNumData;
      int mFirst;
      int mSecond;

      PrintData();
      PrintData( int first );
      PrintData( int first, int second );
    };

    struct PrintInfo {
      Kind mKind;
      char mState;
      PrintData mData;
      bool mHasData;

      PrintInfo();
      PrintInfo( Kind kind);
      PrintInfo( Kind kind, char state, PrintData & data );
    };

  private:

    unsigned int mStudentStartIndex;
    unsigned int mMachineStartIndex;
    unsigned int mCourierStartIndex;
    std::vector<PrintInfo> mPrintList;

    void printIfFlush( unsigned int index, PrintInfo & replaceInfo );
    void printLineAndFlushBuffer();
    void printFinishLine( unsigned int index );
    unsigned int getPrintIndex( Kind kind, unsigned int id = 0 );

};

#endif
