#include "truck.h"

Truck::Truck( Printer & prt,
              NameServer & nameServer,
              BottlingPlant & plant,
              unsigned int numVending ) 
: mPrinter( printer ), 
  mNameServer( nameServer ), 
  mPlant( plant ), 
  mNumVending( numVending ) {

}