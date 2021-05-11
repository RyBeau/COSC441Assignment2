/*
 * CsmaMac.h
 *  Authors: Juliet Samandari & Ryan Beaumont
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
  void  initialize();
  
protected:

  // your protected methods and data members
  int       ownAddress;
  int       bufferSize;
  int       maxBackoffs;
  int       maxAttempts;
  int64_t   macOverheadSizeData;
  int64_t   macOverheadSizeAck;
  double    macAckDelay;
  double    ackTimeout;
  int       fromHigherId;
  int       toHigherId;
  int       fromTransceiverId;
  int       toTransceiverId;

private:

  // your private methods and data members
  ...
  
};


#endif /* CSMAMAC_H_ */
