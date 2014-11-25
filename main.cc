#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;          // direct access to std

#include <cstdlib>           // exit

bool convert( int &val, char *buffer ) {    // convert C string to integer
    std::stringstream ss( buffer );         // connect stream and buffer
    ss >> dec >> val;                       // convert integer from buffer
    return ! ss.fail() &&                   // conversion successful ?
  // characters after conversion all blank ?
  string( buffer ).find_first_not_of( " ", ss.tellg() ) == string::npos;
} // convert

void usage( char *argv[] ) {
    cerr << "Usage: " << argv[0] << endl;
    exit( EXIT_FAILURE );                   // TERMINATE
} // usage

void uMain::main() {
  ConfigParams config;
  switch ( argc ) {
    case 3: {
      int seed;
      if ( !convert( seed, argv[2] ) || seed < 0 ) usage( argv );
      srand(seed);
    }
    case 2:
      processConfigFile( argv[1], config );
    case 1:  
      break;
    default:            // wrong number of options
      usage( argv );
  } // switch

  Printer printer( config.numStudents,
                   config.numVendingMachines,
                   config.numCouriers );

} // uMain::main

// Local Variables: //
// compile-command: "u++ uIO.cc" //
// End: //