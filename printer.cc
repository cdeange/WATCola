#include "printer.h"
#include <string>

Printer::PrintInfo::PrintInfo() {
  this->mState = 'S';  
}

Printer::PrintInfo::PrintInfo( Kind & kind ) {
  this->mState = 'S';  
}

Printer::PrintInfo::PrintInfo( Kind & kind, 
                               char state, 
                               PrintData & data ) {
  this->mKind = kind;
  this->mState = state;
  this->mData = data;
}

Printer::Printer( unsigned int numStudents,
                  unsigned int numVendingMachines,
                  unsigend int numCouriers ) {

  cout << "Parent \t";
  cout << "WATOff \t";
  cout << "Names \t";
  cout << "Truck \t";
  cout << "Plant \t";
  mPrintList.push_back( PrintInfo( Parent ) );
  mPrintList.push_back( PrintInfo( WATCardOffice ) );
  mPrintList.push_back( PrintInfo( NameServer ) );
  mPrintList.push_back( PrintInfo( Truck ) );
  mPrintList.push_back( PrintInfo( BottlingPlant ) );

  // Students
  mStudentStartIndex = mPrintList.size() - 1;
  for( int i = 0; i < numStudents; i++ ) {
    cout << "Stud" << i << "\t";
    mPrintList.push_back( Student );
  }

  // Machines
  mMachineStartIndex = mPrintList.size() - 1;
  for( int i = 0; i < numVendingMachines; i++ ) {
    cout << "Mach" << i << "\t";
    mPrintList.push_back( Vending );
  }

  // Couriers
  mCourierStartIndex = mPrintList.size() - 1;
  for( int i = 0; i < numCouriers; i++ ) {
    cout << "Cour" << i;
    if( i != numCourier - 1 ) cout << "\t";
    mPrintList.push_back( Courier );
  }

  // Stars
  for( int i = 0; i < mPrintList.size(); i++ ) {
    cout << "*******" <<;
    if( i != mPrintList.size() - 1 ) 
      cout << "\t";
  }

}

Printer::~Printer() {
  cout << "***********************" << endl;
}

void Printer::print( Kind kind, char state ) {
  PrintData newData;
  PrintInfo newInfo;
  newInfo.mKind = kind;
  int index = 0;
  switch( kind ) {
    case Parent:
      index = 0;
      break;
    case WATCardOffice:
      index = 1;
      break;
    case NameServer:
      index = 2;
      break;
    case Truck:
      index = 3;
      break;
    case BottlingPlant:
      index = 4;
      break;
    default:
      cerr << "ERROR: wrong usage of print" << endl;
  }

  mPrintList[index]
}

void print( Kind kind, char state, int value1 );
void print( Kind kind, char state, int value1, int value2 );
void print( Kind kind, unsigned int lid, char state );
void print( Kind kind, unsigned int lid, char state, int value1 );
void print( Kind kind, unsigned int lid, char state, int value1, int value2 );

void printIfFlush( int index, PrintInfo& replaceInfo ) {

}


