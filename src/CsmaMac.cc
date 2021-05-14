//TODO implement what is outlined in the ned and .h file
#include "CsmaMac.h"
#include <queue>

Define_Module(CsmaMac);

simsignal_t bufferLossSigId     = cComponent::registerSignal("bufferLossSig");
simsignal_t bufferEnteredSigId  = cComponent::registerSignal("bufferEnteredSig");
simsignal_t numberAttemptsSigId = cComponent::registerSignal("numberAttemptsSigId");
simsignal_t accessFailedSigId   = cComponent::registerSignal("accessFailedSig");
simsignal_t accessSuccessSigId  = cComponent::registerSignal("accessSuccessSig");


void CsmaMac::initialize () {
    ownAddress          = par("ownAddress");
    bufferSize          = par("bufferSize");
    maxBackoffs         = par("maxBackoffs");
    maxAttempts         = par("maxAttempts");
    macOverheadSizeData = par("macOverheadSizeData");
    macOverheadSizeAck  = par("macOverheadSizeAck");
    macAckDelay         = par("macAckDelay");
    ackTimeout          = par("ackTimeout");
    fromHigherId        = findGate("fromHigher");
    toHigherId          = findGate("toHigher");
    fromTransceiverId   = findGate("fromTransceiver");
    toTransceiverId     = findGate("toTransceiver");
    backOffComplete = new cMessage ("BackOffComplete");
    queue<int> buffer;
}

/**
 * Handles incoming messages on the gates.
 */

void CsmaMac::dropAppMessage(AppMessage* appMsg){

}

void CsmaMac::receiveAppMessage(AppMessage* appMsg){


}

void CsmaMac::checkBuffer(){

    if (buffer.empty()) {
        MacState = STATE_IDLE;
    } else {
        MacState = STATE_CS;
        handleMessage(buffer.pop());
    }

}

void CsmaMac::handleMessage(cMessage* msg){
    dbg_string("----------------------------------------------");
    dbg_enter("handleMessage");
    int arrivalGate = msg->getArrivalGateId();

    if (dynamic_cast<CSResponse*>(msg) && arrivalGate == fromTransceiverId && currentState = STATE_CS){
        dbg_string("Received CSReponse Message");
        handleCSResponse((CSResponse) msg);
        dbg_leave("handleMessage");
        return;
    }

    if(msg == backOffComplete && currentState == STATE_BACKOFF){ //Need to check message on buffer
        if (currentAttempts < maxAttempts){
            performCarrierSense();
        } else {
            dbg_string("Max Attempts Reached");
            popHOLPacket();
            currentState = STATE_IDLE;
            currentAttempts = 0;
            currentBackoffs = 0;
        }
        dbg_leave("handleMessage");
        return;
    }
    delete msg;
    error("CsmaMac::handleMessage: unexpected message");
}

/**
 * Sends the CSRequest message to the transceiver.
 */
void CsmaMac::performCarrierSense(){
    dbg_enter("performCarrierSense");
    CSRequest* request = new CSRequest;
    dbg_string("Sending CSRequest");
    send(request, toTransceiverId);
    currentState = STATE_CS;
    dbg_leave("performCarrierSense");
}

/**
 * Handles the received CSResponse message from the transceiver.
 */
void CsmaMac::handleCSReponse(CSResponse* response){
    dbg_enter("handleCSResponse");
    if (!response->busyChannel){
        transmitHOLPacket();
    } else {
        if (currentBackoffs < maxBackoffs){
            currentBackoffs++;
            dbg_string("Beginning CSResponse Backoff");
            beginBackoff(par("csBackoffDistribution").doubleValue());
        } else {
            dbg_string("Max backoffs reached");
            currentBackoffs = 0;
            currentAttempts++;
            dbg_string("Beginning Attempts Backoff");
            beginBackoff(par("attBackoffDistribution").doubleValue());
        }
    }
    delete CSResponse;
    dbg_leave("handleCSReponse");
}

/**
 * Transmits the HOL packet on the channel
 */
void CsmaMac::transmitHOLPacket(){
    dbg_enter("transmitPacket");
    //TODO Transmit Packet Currently on Buffer.
    dbg_leave("transmitPacket");
}

/**
 * Schedules the backoff message to be received after a given backoff time.
 */
void CsmaMac::beginBackoff(double backOffTime){
    dbg_enter("beginBackoff");
    currentBackOffs++;
    scheduleAt(simTime() + backOffTime, backOffComplete);
    currentState = STATE_BACKOFF;
    dbg_leave("beginBackoff");
}

void CsmaMac::PopHOLPacket(){
    //TODO Pop from buffer
}

// ===================================================================================
// ===================================================================================
//
// Debug helpers
//
// ===================================================================================
// ===================================================================================

void CsmaMac::dbg_prefix()
{
    EV << "t = " << simTime()
       << " - CsmaMac-" << ownAddress
       << "s = " << stateStrings[state]
       << endl;
}

// ------------------------------------------------------------


void CsmaMac::dbg_enter (std::string methname)
{
    dbg_prefix();
    EV << "entering " << methname
       << endl;
}

// ------------------------------------------------------------

void CsmaMac::dbg_leave (std::string methname)
{
    dbg_prefix();
    EV << "leaving " << methname
       << endl;
}

// ------------------------------------------------------------

void CsmaMac::dbg_string(std::string str)
{
    dbg_prefix();
    EV << str
       << endl;
}


/**
 * Deletes any allocated messages.
 */
CsmaMac::~CsmaMac(){
    delete backOffComplete;
}
