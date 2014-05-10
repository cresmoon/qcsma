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

#include "Qmodule.h"
#include "selfmsg_m.h"

namespace csma {

Qmodule::Qmodule()
{
    backoff = 0;
    count =0;
    leng = 0;
    RTS = NULL;
}

Qmodule::~Qmodule()
{
    cancelAndDelete(RTS);
}

void Qmodule::initialize()
{
    RTS = new SelfMsg("RTS");
    backoff = par("backoff").doubleValue();
    leng = intuniform(1,5);
    RTS->setBackoff_timer(backoff);
    RTS->setLength(leng);

    scheduleAt(simTime()+ backoff, RTS);
    EV<<"Send RTS with message at Time:"<< simTime()+ backoff << endl;

    queue.setName("queue");

    qlenSignal = registerSignal("qlen"); //current length of queue
    busySignal = registerSignal("busy");
    queueingTimeSignal = registerSignal("queueingTime"); //time of message get in the queue
    emit(qlenSignal, queue.length());
    emit(busySignal, 0);
}

void Qmodule::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage())
    {
        EV<<"Receive SelfMsg :"<<msg->getName() << endl;

        SelfMsg * rcvMsg = check_and_cast< SelfMsg *>(msg);
        backoff = rcvMsg->getBackoff_timer();
        leng = rcvMsg->getLength();
        SelfMsg *request= new SelfMsg("RTS");

        EV<<"Send RTS with message lengh:"<<leng << endl;
        send(request,"gate$o",1);
        request->setBackoff_timer(backoff);
        request->setLength(leng);

        EV<<"Schedule send RTS with message at Time:"<< simTime()+ backoff + leng << endl;
        scheduleAt(simTime()+ backoff+ leng, RTS);
    }
    else
    {
        if(strcmp(msg->getName(),"RTS")==0) //send CTS
        {
            SelfMsg * rcvMsg = check_and_cast< SelfMsg *>(msg);
            int src = rcvMsg->getSrc();

            EV<<"Received RTS with message lengh:"<<rcvMsg->getLength() << endl;

            SelfMsg *CTS= new SelfMsg("CTS");
            CTS->setDest(src);
            send(CTS,"gate$o",1);

            backoff = rcvMsg->getBackoff_timer();
            leng = rcvMsg->getLength();
        }
        else
        if(strcmp(msg->getName(),"data")==0) //insert to Q
        {
            arrival( msg );
            queue.insert( msg );
            msg->setTimestamp();    // get arrival time of msg
            emit(qlenSignal, queue.length());  //get current qlen of queue
        }
        else
            if(strcmp(msg->getName(),"CTS")==0)
            {
                count = count + 1;
                if(count==3) // Q nhan dc 3 msg CTS from 3 src
                {
                    count =0;
                    cMessage *inServiceMsg;
                    if (queue.empty())   //neu queue rong thi tra ve inServiceMsg = null and xuat ra tin hieu queue dang idle
                    {
                        inServiceMsg = NULL;         //gan cho inServiceMsg bang rong
                        emit(busySignal, 0);    // //queue is not busy
                    }
                    else
                    {
                        inServiceMsg = (cMessage *) queue.pop();   // pop msg from queue and assign to inServiceMsg
                        emit(qlenSignal, queue.length());   //get current qlength of queue
                        emit(queueingTimeSignal, simTime() - inServiceMsg->getTimestamp()); //get arrival time of msg
                    }
                    endService(inServiceMsg);
                }
            }
    }
}
} /* namespace qcsma */

