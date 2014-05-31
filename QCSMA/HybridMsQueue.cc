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

#include "HybridMsQueue.h"

namespace csma {

Define_Module(HybridMsQueue);

HybridMsQueue::HybridMsQueue() {
    // TODO Auto-generated constructor stub
        backoff = 0.1;
        count =0;
        msgleng = 0;
        SenseMsg = NULL; //Self message used to schedule backoff generation
        TimeSize = 128;
        MaxLeng = 30;
        NumNodes = 0; // Number of nodes in the ned simulation
        W0 = 10;
        B = 2;
}

HybridMsQueue::~HybridMsQueue() {
    // TODO Auto-generated destructor stub
        cancelAndDelete(SenseMsg);
}

void HybridMsQueue::initialize()
{
    NumNodes = par("netsize").doubleValue();
    SenseMsg = new cMessage("SenseMsg");
    backoff = BackoffGeneration();
    //msgleng = MsgLengGeneration();

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

void HybridMsQueue::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage()) // send RTD message with msleng = 0
    {
        EV<<"Receive SenseMsg :"<<msg->getName()<< "at Time: " << msg->getTimestamp()<< endl;
        if(queue.length() > W0) //Q-CSMA
        {
            SelfMsg *RTD= new SelfMsg("RTD");

            backoff = BackoffGeneration();
            msgleng =0;

            EV<<"Send init RTD with message lengh:"<<msgleng << endl;
            EV<<" Generate backoff value:"<< backoff << endl;

            send(RTD,"gate$o",1);
            RTD->setBackoff_timer(backoff);
            RTD->setLength(msgleng);

            EV<<"RTD: Schedule send next Self message at Time:"<< simTime()+ backoff << endl;
            scheduleAt(simTime()+ backoff, SenseMsg); //schedule for the next self messabe
        }
        else //D-GMS
        {
            SelfMsg *RTS= new SelfMsg("RTS");

            backoff = GMSBackoffGeneration(queue.length());
            msgleng = MsgLengGeneration();

            EV<<"Send init RTS with message lengh:"<<msgleng << endl;
            EV<<" Generate backoff value:"<< backoff << endl;

            send(RTS,"gate$o",1);
            RTS->setBackoff_timer(backoff);
            RTS->setLength(msgleng);

            EV<<"RTS: Schedule send next Self message at Time:"<< simTime()+ backoff + msgleng << endl;
            scheduleAt(simTime()+ backoff+ msgleng, SenseMsg); //schedule for the next self messabe
        }
    }
    else
    {
        if(strcmp(msg->getName(),"data")==0) //insert to Q
        {
            arrival( msg );
            queue.insert( msg );
            msg->setTimestamp();    // get arrival time of msg
            emit(qlenSignal, queue.length());  //get current qlen of queue
        }
        else
        if(strcmp(msg->getName(),"RTD")==0) //send CTD for Q-CSMA
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
            if(strcmp(msg->getName(),"RTS")==0) //send CTS (RESV msg) for D-GMS
            {
                cancelEvent(SenseMsg);

                SelfMsg * rcvMsg = check_and_cast< SelfMsg *>(msg);
                int src = rcvMsg->getSrc();
                double leng = rcvMsg->getLength();
                EV<<"Received RTS with message lengh:"<<leng << endl;

                backoff = GMSBackoffGeneration(queue.length());
                EV<<" Generate backoff value:"<< backoff << endl;
                EV<<"Reschedule send next Self message at Time:"<< simTime()+ backoff + leng << endl;

                SelfMsg *CTS= new SelfMsg("CTS");
                CTS->setDest(src);
                CTS->setLength(leng);
                send(CTS,"gate$o",1);

                //Regenerate backoff for the next self message
                scheduleAt(simTime()+ backoff + leng, SenseMsg);
            }
            else // Pop data from Q to WMedium
            {
                if(queue.length()>W0) //Q-CSMA
                {
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
                                    cMessage *qMsg;
                                    if (queue.empty())   //neu queue rong thi tra ve inServiceMsg = null and xuat ra tin hieu queue dang idle
                                    {
                                        qMsg = NULL;         //gan cho inServiceMsg bang rong
                                        emit(busySignal, 0);     //queue is not busy
                                    }
                                    else
                                    {
                                        qMsg = (cMessage *) queue.pop();   // pop msg from queue and assign to inServiceMsg
                                        emit(qlenSignal, queue.length());   //get current qlength of queue
                                        emit(queueingTimeSignal, simTime() - qMsg->getTimestamp()); //get arrival time of msg
                                    }
                                    send(qMsg, "gate$o",1);
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
            else //D-GMS
            {
                if(strcmp(msg->getName(),"CTS")==0 && queue.length() !=0)
                {
                    count = count + 1;
                    SelfMsg * rcvMsg = check_and_cast< SelfMsg *>(msg);
                    if(count == NumNodes-1)
                    {
                        count =0;
                        EV<<"Received CTS msg with msgleng:"<< rcvMsg->getLength()<< endl;
                        cancelEvent(SenseMsg);
                        cMessage *qMsg;
                        if (queue.empty())
                        {
                            qMsg = NULL;
                            emit(busySignal, 0);
                        }
                        else
                        {
                            qMsg = (cMessage *) queue.pop();
                            emit(qlenSignal, queue.length());
                            emit(queueingTimeSignal, simTime() - qMsg->getTimestamp()); //get arrival time of msg
                        }
                        send(qMsg, "gate$o",1);
                        backoff = GMSBackoffGeneration(queue.length());
                        EV<<"Reschedule send next SelfMsg after send out data:"<< simTime()+ backoff + rcvMsg->getLength() << endl;
                        scheduleAt(simTime()+ backoff + rcvMsg->getLength(), SenseMsg);
                    }
                }
            }
        }
    }
}

double HybridMsQueue::BackoffGeneration()
{
    double bf = (double)(intuniform(1,TimeSize)*0.01);
    return bf;
}

double HybridMsQueue::GMSBackoffGeneration(int w)
{
    double bf1 = (double)(intuniform(1,TimeSize)*0.01);
    double bf = TimeSize*0.01 + B - log(w+1) + bf1;
    return bf;
}

double HybridMsQueue::PiGeneration(int w)
{
    double e = 2.718;
    double b = 0.5;
    double wi = (double)log(b*w); //wi(t) = log (b*q(t))
    EV<<" Wi(t):" << wi << endl;

    double temp = (double)(pow(e,wi));

    EV<<" Pow(e,wi):" << temp << endl;
    double pi = (double)(temp/(temp+1));

    EV<<" Pi:" << pi << endl;

    return pi;
}

double HybridMsQueue::MsgLengGeneration()
{
    double leng = (double)(intuniform(1,MaxLeng)*0.01);
    return leng;
}
} /* namespace csma */
