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
    RTD = NULL; // RTS message
    SenseMsg = NULL; //Self message used to schedule backoff generation
    TimeSize = 128;
    MaxLeng = 30;
    NumNodes = 4; // Number of nodes in the ned simulation
    maxQ = 0;
}

Qmodule::~Qmodule()
{
    cancelAndDelete(SenseMsg);
    cancelAndDelete(RTD);
}

void Qmodule::initialize()
{
    SenseMsg = new cMessage("SenseMsg");
    backoff = BackoffGeneration();
    msgleng = 0;
    //msgleng = MsgLengGeneration();

    EV<<"Backoff generate:"<< backoff << endl;
    EV<<"Send SenseMsg at Time:"<< simTime()+ backoff << endl;
    scheduleAt(simTime()+ backoff, SenseMsg);

    queue.setName("queue");

    for(int i=0; i<NumNodes; i++)
       Qlen[i] = 0;

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

        SelfMsg *request= new SelfMsg("RTD");

        backoff = BackoffGeneration();
        msgleng =0;
        //msgleng = MsgLengGeneration();

        EV<<"Send init RTD with message lengh:"<<msgleng << endl;
        EV<<" Generate new backoff value:"<< backoff << endl;

        send(request,"gate$o",1);
        request->setBackoff_timer(backoff);
        request->setLength(msgleng);

        EV<<"Schedule send next Self message at Time:"<< simTime()+ backoff + msgleng << endl;
        scheduleAt(simTime()+ backoff+ msgleng, SenseMsg); //schedule for the next self messabe
    }
    else
    {
        if(strcmp(msg->getName(),"RTD")==0) //send CTD
        {
            cancelEvent(SenseMsg);

            SelfMsg * rcvMsg = check_and_cast< SelfMsg *>(msg);
            int src = rcvMsg->getSrc();
            double leng = rcvMsg->getLength();
            EV<<"Received RTD with message lengh:"<<leng << endl;

            SelfMsg *CTD= new SelfMsg("CTD");
            CTD->setDest(src);
            CTD->setLength(leng);
            send(CTD,"gate$o",1);

            if(leng != 0)
            {
                //Regenerate backoff for the next self message
                backoff = BackoffGeneration();

                EV<<"Reschedule send next Self message at Time:"<< simTime()+ backoff + leng << endl;
                scheduleAt(simTime()+ backoff + leng, SenseMsg);
            }
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
            if(strcmp(msg->getName(),"CTD")==0)
            {
                count = count + 1;
                SelfMsg * rcvMsg = check_and_cast< SelfMsg *>(msg);
                if(count == NumNodes-1 && rcvMsg->getLength()==0) //send out RTD msg with data leng !=0
                {
                    count =0;
                    EV<<"Received CTD msg with msgleng:"<< rcvMsg->getLength()<< endl;
                    SelfMsg *newRTD= new SelfMsg("RTD");
                    msgleng = MsgLengGeneration();
                    backoff = BackoffGeneration();

                    EV<<"Send newRTD with message lengh:"<<msgleng << endl;
                    send(newRTD,"gate$o",1);

                    newRTD->setBackoff_timer(backoff);
                    newRTD->setLength(msgleng);
                }
                else //received CTD msg with data leng !=0 => send data
                if(count == NumNodes-1 && rcvMsg->getLength()!=0)
                {
                    count =0;
                    EV<<"Received CTD msg with msgleng:"<< rcvMsg->getLength()<< endl;
                    cancelEvent(SenseMsg);
                    double random = (double)(intuniform(1,9)*0.1);
                    double pi = PiGeneration(queue.length());
                    EV<<" Qlengh of nodes : " << queue.length() << "Xac xuat Pi : " << pi << endl;
                    EV<<" Random value : " << random << endl;
                    //geometric(pi, random);
                    if(pi>=random)
                    {
                        EV<<" Nodes active  with Pi: "<< pi << endl;
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
                    else
                    {
                        EV<<" Nodes inactive  with Pi: "<< pi << endl;
                    }
                    backoff = BackoffGeneration();
                    EV<<"Reschedule send next SelfMsg after send out data:"<< simTime()+ backoff + rcvMsg->getLength() << endl;
                    scheduleAt(simTime()+ backoff + rcvMsg->getLength(), SenseMsg);
                }
            }
    }
}

int Qmodule::IsMax(int qlen)
{
    if(maxQ < qlen)
        return 1;
    else
        return 0;
}
double Qmodule::BackoffGeneration()
{
    double bf = (double)(intuniform(1,TimeSize)*0.01);
    return bf;
}
double Qmodule::PiGeneration(int w)
{
    double e = 2.718;
    double b = 0.5;
    double wi = (double)(log(b*w));
    double temp = (double)(pow(e,wi));
    double pi = (double)(temp/(temp+1));
    //double pi = exponential(temp/(temp+1));
    return pi;
}

double Qmodule::MsgLengGeneration()
{
    double leng = (double)(intuniform(1,MaxLeng)*0.01);
    return leng;
}
} /* namespace qcsma */

