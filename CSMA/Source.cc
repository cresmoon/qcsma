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
        long numSent;
        cLongHistogram numSentStats;
        cOutVector numSentVector;
public:
    Source();
    virtual ~Source();

 protected:
   virtual void initialize();
   virtual void handleMessage(cMessage *msg);
   virtual void updateDisplay();
   virtual void finish();
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
    numSent = 0;
    WATCH(numSent);
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

            send(newmsg,"gate$o");
            numSent ++;
            numSentVector.record(numSent);
            numSentStats.collect(numSent);

            scheduleAt(simTime()+ par("sendInterval").doubleValue(), datamsg);
            if (ev.isGUI())
                updateDisplay();
}

void Source::updateDisplay()
{
    char buf[40];
    sprintf(buf, "sent: %ld", numSent);
    getDisplayString().setTagArg("t",0,buf);
}

void Source::finish()
{
    // This function is called by OMNeT++ at the end of the simulation.
    EV << "Sent:     " << numSent << endl;
    EV << " Num Sent , min:    " << numSentStats.getMin() << endl;
    EV << " Num Sent , max:    " << numSentStats.getMax() << endl;
    EV << " Num Sent , mean:   " << numSentStats.getMean() << endl;
    EV << " Num Sent , stddev: " <<  numSentStats.getStddev() << endl;

    recordScalar("#sent", numSent);

    numSentStats.recordAs(" Num Sent");
}
};// end namspace
