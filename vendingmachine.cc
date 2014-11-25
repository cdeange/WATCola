#include "vendingmachine.h"

using namespace std;

VendingMachine::VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
                    unsigned int maxStockPerFlavour ) {

}


void VendingMachine::buy( Flavours flavour, WATCard &card ) {}
unsigned int * VendingMachine::inventory() {
  return mInventory;
}

void VendingMachine::restocked() {}

unsigned int VendingMachine::cost() {
  return mCost;
}

unsigned int VendingMachine::getId() {
  return mId;
}

void VendingMachine::main() {}