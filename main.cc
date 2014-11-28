#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/types.h>
#include <cstdlib>           // exit

#include "MPRNG.h"
#include "WATCard.h"
#include "WATCardOffice.h"
#include "bank.h"
#include "config.h"
#include "nameserver.h"
#include "parent.h"
#include "plant.h"
#include "printer.h"
#include "student.h"

using namespace std;          // direct access to std

MPRNG RAND;

bool convert( int &val, char *buffer ) {    // convert C string to integer
    std::stringstream ss( buffer );         // connect stream and buffer
    ss >> dec >> val;                       // convert integer from buffer
    return ! ss.fail() &&                   // conversion successful ?
  // characters after conversion all blank ?
  string( buffer ).find_first_not_of( " ", ss.tellg() ) == string::npos;
} // convert

void usage( char *argv[] ) {
    cerr << "Usage: " << argv[0] << " [ config-file [ random-seed (> 0) ] ]" << endl;
    exit( EXIT_FAILURE );                   // TERMINATE
} // usage

void uMain::main() {
  ConfigParams config;
  int seed = getpid();
  const char* filename = "soda.config";

  switch ( argc ) {
    case 3:  if ( !convert( seed, argv[2] ) || seed < 0 ) usage( argv );
    case 2:  filename = argv[1];
    case 1:  break;
    default: usage( argv );
  }

  processConfigFile( filename, config );

  srand(seed);

  // Must be created in the following order:
  // Printer, bank, parent, WATCard office, 
  // name server, vending machines, bottling plant, and students

  Printer printer( config.numStudents, config.numVendingMachines, config.numCouriers );
  Bank bank( config.numStudents );
  Parent parent( printer, bank, config.numStudents, config.parentalDelay );
  WATCardOffice office( printer, bank, config.numCouriers );
  NameServer nameServer( printer, config.numVendingMachines, config.numStudents );

  VendingMachine* machines[config.numVendingMachines];
  for ( unsigned int i = 0; i < config.numVendingMachines; ++i ) {
    machines[i] = new VendingMachine( printer, 
                                      nameServer, 
                                      i, 
                                      config.sodaCost, 
                                      config.maxStockPerFlavour );
  }

  BottlingPlant *plant = new BottlingPlant( 
            printer, 
            nameServer, 
            config.numVendingMachines, 
            config.maxShippedPerFlavour, 
            config.maxStockPerFlavour, 
            config.timeBetweenShipments );

  Student* students[config.numStudents];
  for ( unsigned int i = 0; i < config.numStudents; ++i ) {
    students[i] = new Student( printer, nameServer, office, i, config.maxPurchases );
  }

  for ( unsigned int i = 0; i < config.numStudents; ++i ) {
    delete students[i];
  }

  delete plant;
  for ( unsigned int i = 0; i < config.numVendingMachines; ++i ) {
    delete machines[i];
  }


}
