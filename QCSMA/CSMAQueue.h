//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef CSMAQUEUE_H_
#define CSMAQUEUE_H_
#include "selfmsg_m.h"

#include "omnetpp.h"

namespace csma {

class CSMAQueue : public cSimpleModule
{
protected:
        cMessage *SenseMsg;
        //SelfMsg *RTS;
        cQueue queue;
        simsignal_t qlenSignal;
        simsignal_t busySignal;
        simsignal_t queueingTimeSignal;
        double backoff;
        int msgleng;
        int count;
        int TimeSize;
        int MaxLeng;
        int NumNodes;
public:
    CSMAQueue();
    virtual ~CSMAQueue();
protected:
    virtual void initialize();

    virtual void handleMessage(cMessage *msg);
    virtual double BackoffGeneration();
    virtual double MsgLengGeneration();

    // hook functions to (re)define behaviour
    virtual void arrival(cMessage *msg) {}
};

} /* namespace csma */
#endif /* CSMAQueue_H_ */
