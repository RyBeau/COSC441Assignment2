//TODO implement what is outlined in the ned and .h file
#include "CsmaMac.h"

Define_Module(CsmaMac);

simsignal_t bufferLossSigId = cComponent::registerSignal("bufferLossSig");
simsignal_t bufferEnteredSigId = cComponent::registerSignal("bufferEnteredSig");
simsignal_t numberAttemptsSigId = cComponent::registerSignal("numberAttemptsSigId");
simsignal_t accessFailedSigId = cComponent::registerSignal("accessFailedSig");
simsignal_t accessSuccessSigId = cComponent::registerSignal("accessSuccessSig");


void CsmaMac::initialize () {
    ownAddress = par("ownAddress");
    bufferSize = par("bufferSize");
    maxBackoffs = par("maxBackoffs");
    maxAttempts = par("maxAttempts");
    macOverheadSizeData = par("macOverheadSizeData");
    macOverheadSizeAck = par("macOverheadSizeAck");
    macAckDelay = par("macAckDelay");
    ackTimeout = par("ackTimeout");
    fromHigherId = findGate("fromHigher");
    toHigherId = findGate("toHigher");
    fromTransceiverId = findGate("fromTransceiver");
    toTransceiverId = findGate("toTransceiver");

}
