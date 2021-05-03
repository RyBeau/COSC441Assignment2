//TODO implement what is outlined in the ned and .h file
#include "CsmaMac.h"

Define_Module(CsmaMac);

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
