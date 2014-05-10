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
#include <stdio.h>

namespace csma {

class Source : public cSimpleModule
{
private:
        SelfMsg *datamsg; //msg request to send
public:
    Source();
    virtual ~Source();

 protected:
   virtual void initialize();
   virtual void handleMessage(cMessage *msg);
};

Define_Module(Source);

Source::Source() {
    datamsg = NULL;
}

Source::~Source() {
    cancelAndDelete(datamsg);
}

void Source:: initialize()
{
    datamsg = new SelfMsg("data");
    scheduleAt(simTime(), datamsg);
}

void Source::handleMessage(cMessage *msg)
{
           //send data message
            SelfMsg *newmsg = new SelfMsg("data");
            int src = getIndex();
            int dest = (int)src +10;
            newmsg->setSrc(src);
            newmsg->setDest(dest);
            //newmsg->setLength(leng);
            EV<<"Send Data Message out :"<< newmsg->getName() << endl;
            send(newmsg,"gate$o");
            EV<<"Send Data Message out at time:"<< newmsg->getTimestamp() << endl;
            scheduleAt(simTime()+ par("sendInterval").doubleValue(), datamsg);
}
};// end namspace
