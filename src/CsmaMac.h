/*
 * CsmaMac.h
 *
 */

#ifndef CSMAMAC_H_
#define CSMAMAC_H_

#include <omnetpp.h>
// further includes ...


using namespace omnetpp;


// design a state machine and suitable states, name them here ..
enum MacState {
  ...
};



// string names for your states (for debugging purposes)
const char * const stateStrings[] = {
  ...
};


class CsmaMac : public cSimpleModule {

public:

  // your public methods and data members
  ...
  
protected:

  // your protected methods and data members
  ...
  

private:

  // your private methods and data members
  ...
  
};


#endif /* CSMAMAC_H_ */
