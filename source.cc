#include <omnetpp.h>
#include <stdio.h>

using namespace omnetpp;

class Source : public cSimpleModule
{
   cMessage *send_event; //message-reminder: send next job

protected:
   virtual void initialize();
   virtual void handleMessage(cMessage *msgin);

private:
   void SendJob();
};

Define_Module(Source);

void Source::initialize()
{
   for (int i = 0; i < (int)par("initial_queue"); i++)
   {
      SendJob();
   }

   send_event = new cMessage("Send!");
   scheduleAt(par("interarrival_time"), send_event);
}

void Source::handleMessage(cMessage *msgin) //send next job
{
   SendJob();
   simtime_t now = simTime();
   simtime_t next = now + par("interarrival_time");
   scheduleAt(next, send_event);
}

void Source::SendJob()
{
   send(new cMessage(" Job"), "out");
}
