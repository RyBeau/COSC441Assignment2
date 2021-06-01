/*
 * CsmaMac.h
 *  Authors: Juliet Samandari & Ryan Beaumont
 *  Date: May 2021
 */

#ifndef CSMAMAC_H_
#define CSMAMAC_H_

#include <omnetpp.h>
#include "CsmaMac.h"
#include "AppMessage_m.h"
#include "AppResponse_m.h"
#include "CSRequest_m.h"
#include "CSResponse_m.h"
#include "MacPacket_m.h"
#include "MacPacketType_m.h"
#include "TransmissionRequest_m.h"
#include "TransmissionConfirmation_m.h"
#include "TransmissionIndication_m.h"
#include <queue>


using namespace omnetpp;

// design a state machine and suitable states, name them here ..
enum MacState {
    STATE_BACKOFF, STATE_IDLE, STATE_CS, STATE_TCONF, STATE_ACK
};



// string names for your states (for debugging purposes)
const char * const stateStrings[5] = {
  "Wait for backoff to conclude", "Wait for incoming packet", "Wait for carrier sensing to conclude",
  "Wait to receive TransmissionConfirmation", "Wait to receive Acknowledgement"
};


class CsmaMac : public cSimpleModule {

public:

  // your public methods and data members
  void  initialize();
  virtual void handleMessage(cMessage* msg);
  ~CsmaMac();
  
protected:

  // your protected methods and data members
  simsignal_t bufferLossSigId     = cComponent::registerSignal("bufferLossSig");
  simsignal_t bufferEnteredSigId  = cComponent::registerSignal("bufferEnteredSig");
  simsignal_t numberAttemptsSigId = cComponent::registerSignal("numberAttemptsSig");
  simsignal_t accessFailedSigId   = cComponent::registerSignal("accessFailedSig");
  simsignal_t accessSuccessSigId  = cComponent::registerSignal("accessSuccessSig");

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
  int       currentState = STATE_IDLE;
  int currentBackoffs = 0;
  int currentAttempts = 0;
  cMessage* backOffComplete;
  cMessage* ackTimeoutMessage;
  cMessage* ackSendMessage;
  std::queue<AppMessage*> ackQueue;
  std::queue<AppMessage*> buffer;
  void handleTransmissionConfirmation(TransmissionConfirmation* confirmation);
  void handleTransmissionIndication(TransmissionIndication* indication);
  void handleAckTimeout();
  void dbg_prefix();
  void dbg_enter (std::string methname);
  void dbg_leave (std::string methname);
  void dbg_string(std::string str);
  void transmitAckForReceived(AppMessage* appMsg);
  void transmitHOLPacket();
  virtual void checkBuffer();
  void dropPacketChannelFail();
  void receiveAppMessage(AppMessage* appMsg);


private:

  // your private methods and data members
  void dropAppMessage(AppMessage* appMsg);
  void performCarrierSense();
  void handleCSResponse(CSResponse* response);
  void beginBackoff(double backOffTime);
  MacPacket* encapsulateAppMessage(AppMessage* message);
  TransmissionRequest* encapsulateMacPacket(MacPacket* macPacket);
  void handleAck(MacPacket* macPacket);
  void dropPacketSuccess();
  void handleReceivedMessage(MacPacket* macPacket);
};


#endif /* CSMAMAC_H_ */
