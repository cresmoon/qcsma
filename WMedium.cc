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
    simsignal_t msglenSignal;
    simsignal_t busySignal;
    simsignal_t serviceTimeSignal;
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

Define_Module(WMedium);

void WMedium::initialize()
{
    msglenSignal = registerSignal("msglen"); //length of current msg come to WMedium
    busySignal = registerSignal("busy");
    serviceTimeSignal = registerSignal("msgTime"); //time of message get in the WMedium
    emit(busySignal, 0);
}
void WMedium::handleMessage(cMessage *msg)
{
        EV<<"Received msg from client :"<< msg->getName() << endl;
        int g = (int)(msg->getArrivalGate())->getIndex();
        if(strcmp(msg->getName(),"RTS")==0)
        {
            //EV<<"RTS message come from gate:"<< g << endl;
            for(int i =0; i<=4 ; i++)
            {
                if(i!=g&&i!=4) //forware RTS to the all other nodes
                {
                    SelfMsg *rcvMsg = check_and_cast< SelfMsg *>(msg);

                    SelfMsg *RTS = new SelfMsg("RTS");
                    RTS->setSrc(g);
                    RTS->setLength(rcvMsg->getLength());
                    RTS->setBackoff_timer(rcvMsg->getBackoff_timer());

                    //EV<<"Source of RTS message is:"<< g << endl;
                    send(RTS,"gate$o",i);

                    emit(busySignal, 1);
                    emit(serviceTimeSignal, simTime() - RTS->getTimestamp());
                }
            }
            emit(busySignal, 0);
        }
        else
            if(strcmp(msg->getName(),"CTS")==0) //forward msg
            {
                SelfMsg * rcvMsg = check_and_cast< SelfMsg *>(msg);
                int dest = rcvMsg->getDest();
                send(rcvMsg,"gate$o",dest);
            }
            else
            if (strcmp(msg->getName(),"data")==0)
            {
                send(msg,"gate$o",4);
                emit(busySignal, 1);
            }
}
} /* namespace qcsma */
