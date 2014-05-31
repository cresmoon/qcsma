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

#ifndef QCSMAQUEUE_H_
#define QCSMAQUEUE_H_
#include "selfmsg_m.h"
#include "omnetpp.h"

namespace csma {

class QCSMAQueue : public cSimpleModule
{
protected:
        cMessage *SenseMsg;
        cQueue queue;
        simsignal_t qlenSignal;
        simsignal_t busySignal;
        simsignal_t queueingTimeSignal;
        double backoff;
        double msgleng;
        int count;
        int TimeSize;
        int MaxLeng;
        int NumNodes;
public:
    QCSMAQueue();
    virtual ~QCSMAQueue();
protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual double BackoffGeneration();
    virtual double MsgLengGeneration();
    virtual double PiGeneration(int w);

    // hook functions to (re)define behaviour
    virtual void arrival(cMessage *msg) {}
};

} /* namespace csma */
#endif /* QCSMAQueue_H_ */
