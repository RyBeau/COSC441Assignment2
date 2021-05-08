
#include "CsmaMac.h"
#include "CSResponse_m.h"
#include "CSRequest_m.h"

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
}

/**
 * Handles incoming messages on the gates.
 */
void CsmaMac::handleMessage(cMessage* msg){
    dbg_string("----------------------------------------------");
    dbg_enter("handleMessage");
    int arrivalGate = msg->getArrivalGateId();

    if (dynamic_cast<CSResponse*>(msg) && arrivalGate == fromTransceiverId){
        handleCSResponse((CSResponse) msg);
    }

    if(msg == backOffComplete){
        if (currentAttempts < maxAttempts){
            performCarrierSense();
        } else {
            //TODO Drop packet?
        }
    }
    delete msg;
    dbg_leave("handleMessage");
}

/**
 * Sends the CSRequest message to the transceiver.
 */
void CsmaMac::performCarrierSense(){
    CSRequest* request = new CSRequest;
    send(request, toTransceiverId);
}

/**
 * Handles the received CSResponse message from the transceiver.
 */
void CsmaMac::handleCSReponse(CSRsponse* response){
    dbg_enter("handleCSResponse");
    if (!response->busyChannel){
        transmitPacket();
    } else {
        if (currentBackoffs < maxBackoffs){
            currentBackoffs++;
            dbg
            beginBackoff(par("csBackoffDistribution").doubleValue());
        } else {
            currentBackoffs = 0;
            currentAttempts++;
            beginBackoff(par("attBackoffDistribution").doubleValue());
        }
    }
    delete CSResponse;
    dbg_leave("handleCSReponse");
}

/**
 * Transmits the HOL packet on the channel
 */
void CsmaMac::transmitPacket(){
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
    dbg_leave("beginBackoff");
}

/**
 * Deletes any allocated messages.
 */
CsmaMac::~CsmaMac(){
    delete backOffComplete;
}
