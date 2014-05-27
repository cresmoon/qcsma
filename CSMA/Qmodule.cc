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
    backoff = 0.1;
    count =0;
    msgleng = 0;
    RTS = NULL; // RTS message
    SenseMsg = NULL; //Self message used to schedule backoff generation
    TimeSize = 128;
    MaxLeng = 30;
    NumNodes = 4; // Number of nodes in the ned simulation
}

Qmodule::~Qmodule()
{
    cancelAndDelete(SenseMsg);
    cancelAndDelete(RTS);
}

void Qmodule::initialize()
{
    SenseMsg = new cMessage("SenseMsg");
    backoff = BackoffGeneration();
    msgleng = MsgLengGeneration();

    EV<<"Backoff generate:"<< backoff << endl;
    EV<<"Send SenseMsg at Time:"<< simTime()+ backoff << endl;
    scheduleAt(simTime()+ backoff, SenseMsg);

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
        EV<<"Receive SenseMsg :"<<msg->getName()<< "at Time: " << msg->getTimestamp()<< endl;

        SelfMsg *request= new SelfMsg("RTS");

        backoff = BackoffGeneration();
        msgleng = MsgLengGeneration();

        EV<<"Send RTS with message lengh:"<<msgleng << endl;
        EV<<" Generate new backoff value:"<< backoff << endl;

        send(request,"gate$o",1);
        request->setBackoff_timer(backoff);
        request->setLength(msgleng);

        //wait(leng);

        EV<<"Schedule send next Self message at Time:"<< simTime()+ backoff + msgleng << endl;
        scheduleAt(simTime()+ backoff+ msgleng, SenseMsg); //schedule for the next self messabe
    }
    else
    {
        if(strcmp(msg->getName(),"RTS")==0) //send CTS
        {
            cancelEvent(SenseMsg);

            SelfMsg * rcvMsg = check_and_cast< SelfMsg *>(msg);
            int src = rcvMsg->getSrc();
            int leng = rcvMsg->getLength();
            EV<<"Received RTS with message lengh:"<<leng << endl;

            SelfMsg *CTS= new SelfMsg("CTS");
            CTS->setDest(src);
            send(CTS,"gate$o",1);

            //Regenerate backoff for the next self message
            backoff = BackoffGeneration();

            EV<<"Reschedule send next Self message at Time:"<< simTime()+ backoff + leng << endl;
            scheduleAt(simTime()+ backoff + leng, SenseMsg);
        }
        else
        if(strcmp(msg->getName(),"data")==0) //insert to Q
        {
            arrival( msg );
            queue.insert( msg );
            msg->setTimestamp();    // get arrival time of msg
            emit(qlenSignal, queue.length());  //get current qlen of queue
        }
        else // Pop data from Q to WMedium
            if(strcmp(msg->getName(),"CTS")==0)
            {
                count = count + 1;
                if(count == NumNodes-1)
                {
                    count =0;
                    cMessage *inServiceMsg;
                    if (queue.empty())   //neu queue rong thi tra ve inServiceMsg = null and xuat ra tin hieu queue dang idle
                    {
                        inServiceMsg = NULL;         //gan cho inServiceMsg bang rong
                        emit(busySignal, 0);     //queue is not busy
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

double Qmodule::BackoffGeneration()
{
    double bf = (double)(intuniform(1,TimeSize)*0.01);
    return bf;
}

double Qmodule::MsgLengGeneration()
{
    double leng = (intuniform(1,MaxLeng))*0.1;
    return leng;
}
} /* namespace qcsma */

