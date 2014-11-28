#ifndef WATCARD_H_
#define WATCARD_H_

#include <uFuture.h>

/******** WATCard class  *******/

class WATCard {
    WATCard( const WATCard & );             // Prevents copying
    WATCard &operator=( const WATCard & );  // Prevents copying

    unsigned int mBalance;                  // Balance
  public:
    WATCard();                              // Constructor
                                            // Default destructor
    typedef Future_ISM<WATCard *> FWATCard; // Future watcard pointer
    void deposit( unsigned int amount );   
    void withdraw( unsigned int amount );
    unsigned int getBalance();
};

#endif
