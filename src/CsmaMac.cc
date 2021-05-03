//TODO implement what is outlined in the ned and .h file
#include "CsmaMac.h"

Define_Module(CsmaMac);

void CsmaMac::initialize () {
    ownAddress = par("ownAddress");                       // own node address
    bufferSize = par("bufferSize");                       // max number of application messages the MAC can hold
    maxBackoffs = par("maxBackoffs");                      // max number of backoff operations per attempt
    maxAttempts = par("maxAttempts");                      // max number of attempts
    macOverheadSizeData = par("macOverheadSizeData");  // Overhead for a MAC data packet
    macOverheadSizeAck = par("macOverheadSizeAck");   // Overhead for a MAC acknowledgement
    macAckDelay = par("macAckDelay");             // fixed waiting time for sending ACK
    ackTimeout = par("ackTimeout");              // timeout for ACK packet
}
