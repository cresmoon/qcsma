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
#include "selfmsg_m.h"

namespace csma {

class WMedium :public cSimpleModule
{
protected:
    int NumNodes;
    simtime_t msgTime;
    simtime_t arrivalTime;
    simsignal_t msglenSignal;
    simsignal_t busySignal;
    simsignal_t serviceTimeSignal;
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual int IsDuplicatedMsg();
};

Define_Module(WMedium);

void WMedium::initialize()
{
    msgTime =0;
    arrivalTime =0;
    NumNodes = 4;
    msglenSignal = registerSignal("msglen"); //length of current msg come to WMedium
    busySignal = registerSignal("busy");
    serviceTimeSignal = registerSignal("msgTime"); //time of message get in the WMedium
    emit(busySignal, 0);
}

void WMedium::handleMessage(cMessage *msg)
{
        EV<<"Received msg from client :"<< msg->getName() << endl;
        int g = (int)(msg->getArrivalGate())->getIndex();
        if(strcmp(msg->getName(),"RTD")==0)
        {
             for(int i =0; i<=NumNodes ; i++)
                {
                    if(i!=g&&i!=NumNodes) //forware RTS to the all other nodes
                        {
                            SelfMsg *rcvMsg = check_and_cast< SelfMsg *>(msg);

                            SelfMsg *RTD = new SelfMsg("RTD");
                            RTD->setSrc(g);
                            RTD->setLength(rcvMsg->getLength());
                            RTD->setBackoff_timer(rcvMsg->getBackoff_timer());

                            //EV<<"Source of RTS message is:"<< g << endl;
                            send(RTD,"gate$o",i);

                            emit(busySignal, 1);
                            emit(serviceTimeSignal, simTime() - RTD->getTimestamp());
                        }
                }
            emit(busySignal, 0);
        }
        else
            if(strcmp(msg->getName(),"CTD")==0) //forward msg
            {
                SelfMsg * rcvMsg = check_and_cast< SelfMsg *>(msg);
                int dest = rcvMsg->getDest();
                send(rcvMsg,"gate$o",dest);
            }
            else
            if (strcmp(msg->getName(),"data")==0)
            {
                send(msg,"gate$o",NumNodes);
                emit(busySignal, 1);
            }
}
int WMedium::IsDuplicatedMsg()
{
    cMessage *msg1 = new cMessage ("RTS");
    cMessage *msg2 = new cMessage ("RTS");
    if(msg1->getArrivalTime()==msg2->getArrivalTime() && msg1->getSenderGate()->getIndex() != msg2->getSenderGate()->getIndex())
        return 1;
    else
        return 0;
}
} /* namespace qcsma */
