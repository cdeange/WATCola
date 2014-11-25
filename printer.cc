#include <iostream>
#include <string>

#include "printer.h"

using namespace std;

Printer::PrintInfo::PrintInfo() {  
  this->mHasData = false;
}

// Only used in initialization of the printer
Printer::PrintInfo::PrintInfo( Kind kind ) {
  this->mKind = kind;
  this->mHasData = false;
}

Printer::PrintInfo::PrintInfo( Kind kind, 
                               char state, 
                               PrintData & data ) {
  this->mKind = kind;
  this->mState = state;
  this->mData = data;
  this->mHasData = true;
}

Printer::PrintData::PrintData() {
  this->mNumData = 0;
}

Printer::PrintData::PrintData( int first ) {
  this->mNumData = 1;
  this->mFirst = first;
}

Printer::PrintData::PrintData( int first, int second ) {
  this->mNumData = 2;
  this->mFirst = first;
  this->mSecond = second;
}

Printer::Printer( unsigned int numStudents,
                  unsigned int numVendingMachines,
                  unsigned int numCouriers ) {

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
  for( unsigned int i = 0; i < numStudents; i++ ) {
    cout << "Stud" << i << "\t";
    mPrintList.push_back( PrintInfo( Student ) );
  }

  // Machines
  mMachineStartIndex = mPrintList.size() - 1;
  for( unsigned int i = 0; i < numVendingMachines; i++ ) {
    cout << "Mach" << i << "\t";
    mPrintList.push_back( PrintInfo( Vending ) );
  }

  // Couriers
  mCourierStartIndex = mPrintList.size() - 1;
  for( unsigned int i = 0; i < numCouriers; i++ ) {
    cout << "Cour" << i;
    if( i != numCouriers - 1 ) cout << "\t";
    mPrintList.push_back( PrintInfo( Courier ) );
  }

  cout << endl;

  // Stars
  for( unsigned int i = 0; i < mPrintList.size(); i++ ) {
    cout << "*******";
    if( i != mPrintList.size() - 1 ) 
      cout << "\t";
  }
  cout << endl;

}

Printer::~Printer() {
  cout << "***********************" << endl;
}

void Printer::print( Kind kind, char state ) {
  PrintData newData;
  PrintInfo newInfo( kind, state, newData );

  int printIndex = getPrintIndex( kind );
  if( state == 'F' ) {
    printFinishLine( printIndex );
  } else {
    printIfFlush( printIndex, newInfo );
  }
}

void Printer::print( Kind kind, char state, int value1 ) {
  PrintData newData( value1 );
  PrintInfo newInfo( kind, state, newData );

  printIfFlush( getPrintIndex( kind ), newInfo );
}

void Printer::print( Kind kind, char state, int value1, int value2 ) {
  PrintData newData( value1, value2 );
  PrintInfo newInfo( kind, state, newData );

  printIfFlush( getPrintIndex( kind ), newInfo );
}

void Printer::print( Kind kind, unsigned int lid, char state ) {
  PrintData newData;
  PrintInfo newInfo( kind, state, newData );

  printIfFlush( getPrintIndex( kind, lid ), newInfo );  
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1 ) {
  PrintData newData( value1 );
  PrintInfo newInfo( kind, state, newData );

  printIfFlush( getPrintIndex( kind, lid ), newInfo );
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1, int value2 ) {
  PrintData newData( value1, value2 );
  PrintInfo newInfo( kind, state, newData );

  printIfFlush( getPrintIndex( kind, lid ), newInfo );
}

unsigned int Printer::getPrintIndex( Kind kind, unsigned int id ) {

  switch( kind ) {
    case Parent:
      return 0;
    case WATCardOffice:
      return 1;
    case NameServer:
      return 2;
    case Truck:
      return 3;
    case BottlingPlant:
      return 4;
    case Student:
      return mStudentStartIndex + id;
    case Vending:
      return mMachineStartIndex + id;
    case Courier:
      return mCourierStartIndex + id;
    default:
      cerr << "ERROR: wrong usage of print" << endl;
  }

  return 0;
}

void Printer::printIfFlush( unsigned int index, PrintInfo & replaceInfo ) {
  if( mPrintList[index].mHasData ) {
    printLineAndFlushBuffer();
  }

  mPrintList[index] = replaceInfo;
  mPrintList[index].mHasData = true;
}

void Printer::printLineAndFlushBuffer() {

  // That thing where you don't print anything else after the last element
  unsigned int lastPrintIndex = 0;
  for( unsigned int i = 0; i < mPrintList.size(); i++ ) {
    if( mPrintList[i].mHasData ) lastPrintIndex = i;
  }

  for( unsigned int i = 0; i <= lastPrintIndex; i++ ) {
    if( mPrintList[i].mHasData ) {
      cout << mPrintList[i].mState;
      switch( mPrintList[i].mData.mNumData ) {
        case 0:
          break;
        case 1:
          cout << mPrintList[i].mData.mFirst;
          break;
        case 2:
          cout << mPrintList[i].mData.mFirst;
          cout << mPrintList[i].mData.mSecond;
          break;
        default:
          cerr << "ERROR: number of data points is wrong" << endl;
      }
    }
    
    mPrintList[i].mHasData = false;

    if( i != lastPrintIndex ) cout << "\t";
  }

  cout << endl;

}

void Printer::printFinishLine( unsigned int index ) {
  printLineAndFlushBuffer();
  for( unsigned int i = 0; i < mPrintList.size(); i++ ) {
    if( i == index ) {
      cout << "F";
    } else {
      cout << "...";
    }

    if( i != mPrintList.size() - 1 ) cout << "\t";
  }

  cout << endl;

}


