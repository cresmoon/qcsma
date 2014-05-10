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

class AccessPoint : public cSimpleModule
{
private:
    int gate;
protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

Define_Module(AccessPoint);

void AccessPoint::initialize()
{
    gate =0;
}
void AccessPoint::handleMessage(cMessage *msg)
{
    int src;
    //int dest;
    if(strcmp(msg->getName(),"data")==0) //send data message to dest
    {
        SelfMsg * rcvMsg = check_and_cast< SelfMsg *>(msg);
        src= rcvMsg->getSrc();
        //dest = rcvMsg->getDest();
        //EV<<"Receive msg from WMedium:" << rcvMsg->getName()<<" From src :" <<src <<" To dest :" << dest <<endl;
        if(src == 0)
            gate = 1;
        else
            if(src == 1)
                gate =2;
            else
                if(src ==2)
                    gate =3;
                else
                    if(src ==3)
                        gate =4;
        send(rcvMsg,"gate$o",gate);
    }
}
} /* namespace qcsma */
