#include "WATCard.h"

/******** WATCard constructor *******
  Purpose: WATCard constructor

  Returns: A WATCard object

  Errors: n/a

  Globals: n/a
*/

WATCard::WATCard() {
  mBalance = 0;
}

/******** WATCard deposit function *******
  Purpose: deposits amount into watcard

  Returns: n/a

  Errors: n/a

  Globals: mBalance
*/

void WATCard::deposit( unsigned int amount ) {
  mBalance += amount;
}

/******** WATCard withdraw function  *******
  Purpose: withdraws amount out of watcard

  Returns: n/a

  Errors: n/a

  Globals: mBalance
*/
void WATCard::withdraw( unsigned int amount ) {
  mBalance -= amount;
}


/******** WATCard getBalance function  *******
  Purpose: returns current balance on card

  Returns: current balance on card

  Errors: n/a

  Globals: mBalance
*/
unsigned int WATCard::getBalance() {
  return mBalance;
}