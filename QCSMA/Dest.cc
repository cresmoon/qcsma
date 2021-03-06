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

#include "omnetpp.h"

namespace csma {

class Dest : public cSimpleModule
{
  private:
    simsignal_t lifetimeSignal;
    long numReceived;

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void updateDisplay();
};

Define_Module( Dest );

void Dest::initialize()
{
    numReceived = 0;
    WATCH(numReceived);
    lifetimeSignal = registerSignal("lifetime");
}

void Dest::handleMessage(cMessage *msg)
{
    simtime_t lifetime = simTime() - msg->getCreationTime();
    EV << "Received : " << msg->getName() << ", lifetime: " << lifetime << "s" << endl;
    numReceived++;
    if(ev.isGUI())
        updateDisplay();
    emit(lifetimeSignal, lifetime);
    //EV << "Life Time of data msg from src to dest : " << msg->getArrivalTime()-msg->getCreationTime()<< "s" << endl;
    delete msg;
}
void Dest::updateDisplay()
{
    char buf[40];
    sprintf(buf, "Rcv: %ld", numReceived);
    getDisplayString().setTagArg("t",0,buf);
}
}
