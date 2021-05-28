/*
 * AlohaMac.h
 *  Authors: Juliet Samandari & Ryan Beaumont
 *  Date: May 2021
 */

#include "AlohaMac.h"

Define_Module(AlohaMac);

using namespace omnetpp;


/**
 * Handles incoming messages on the gates.
 */
void AlohaMac::handleMessage(cMessage* msg){
    dbg_string("----------------------------------------------");
    dbg_enter("handleMessage");
    int arrivalGate = msg->getArrivalGateId();

    if (dynamic_cast<AppMessage*>(msg) && arrivalGate == fromHigherId){
           receiveAppMessage((AppMessage*) msg);
           return;
    }

    if(dynamic_cast<TransmissionConfirmation*>(msg) && arrivalGate == fromTransceiverId && currentState == STATE_TCONF){
        dbg_string("Transmission Confirmation Received");
        handleTransmissionConfirmation((TransmissionConfirmation*) msg);
        dbg_leave("handleMessage");
        return;
    }

    if(dynamic_cast<TransmissionConfirmation*>(msg) && arrivalGate == fromTransceiverId){
        dbg_string("Transmission Confirmation for Ack Received");
        delete msg;
        dbg_leave("handleMessage");
        return;
    }

    if(dynamic_cast<TransmissionIndication*>(msg) && arrivalGate == fromTransceiverId){
        dbg_string("Transmission Indication Received");
        handleTransmissionIndication((TransmissionIndication*) msg);
        dbg_leave("handleMessage");
        return;
    }

    if(msg == backOffComplete && currentState == STATE_BACKOFF){ //Need to check message on buffer
        dbg_string("Backoff Complete");
        checkBuffer();
        dbg_leave("handleMessage");
        return;
    }


    if(msg == ackTimeoutMessage && currentState == STATE_ACK){
        dbg_string("Ack Timeout Message Received");
        handleAckTimeout();
        dbg_leave("handleMessage");
        return;
    }
    if (msg == ackSendMessage && msg->isSelfMessage()) {
        dbg_string("Ack Completed Message Received");
        transmitAckForReceived((AppMessage*) ackQueue.front());
        delete ackQueue.front();
        ackQueue.pop();
        dbg_leave("handleMessage");
        return;
    }

    delete msg;
    error("CsmaMac::handleMessage: unexpected message");
}


/**
 * Checks buffer for messages to transmit.
 */
void AlohaMac::checkBuffer(){
    dbg_enter("checkBuffer");
    if (buffer.empty()) {
        currentState = STATE_IDLE;
    } else {
        if (currentAttempts < maxAttempts){
            transmitHOLPacket();
        } else {
            dbg_string("Max Attempts Reached");
            dropPacketChannelFail();
            emit(numberAttemptsSigId, currentAttempts);
            currentAttempts = 0;
            checkBuffer();
        }
    }

    dbg_leave("checkBuffer");
}

///**
// * Deletes any allocated messages.
// */
//AlohaMac::~AlohaMac(){
//    cancelAndDelete(backOffComplete);
//    cancelAndDelete(ackTimeoutMessage);
//    cancelAndDelete(ackSendMessage);
//    while (buffer.size() > 0) {
//        delete buffer.front();
//        buffer.pop();
//    }
//    while (ackQueue.size() > 0) {
//        delete ackQueue.front();
//        ackQueue.pop();
//    }
//}



