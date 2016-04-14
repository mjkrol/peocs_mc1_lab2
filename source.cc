#include <omnetpp.h>

using namespace omnetpp;

class Source : public cSimpleModule
{
   cMessage *send_event; //message-reminder: send next job

protected:
   virtual void initialize();
   virtual void handleMessage(cMessage *msgin);
};

Define_Module(Source);

void Source::initialize()
{
   for(int i=0;i<(int)par("initial_queue");i++) //this loop builds the initial queue
   {
      cMessage *job = new cMessage(" Job");
      send(job, "out" );
   }

   send_event = new cMessage("Send!");
   scheduleAt(par("interarrival_time"), send_event);
}

void Source::handleMessage(cMessage *msgin) //send next job
{
   cMessage *job = new cMessage(" Job");
   send(job, "out" );
   scheduleAt(simTime()+par("interarrival_time"), send_event); //schedule next send event
}

