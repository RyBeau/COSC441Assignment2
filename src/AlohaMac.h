/*
 * AlohaMac.h
 *  Authors: Juliet Samandari & Ryan Beaumont
 *  Date: May 2021
 */


#ifndef ALOHAMAC_H_
#define ALOHAMAC_H_

#include <omnetpp.h>
#include "CsmaMac.h"

using namespace omnetpp;

class AlohaMac : public CsmaMac {

 public:

    void handleMessage(cMessage* msg) override;
//    ~AlohaMac();

 protected:

    void checkBuffer() override;


 private:

};


#endif /* ALOHAMAC_H_ */
