#include <omnetpp.h>

using namespace omnetpp;

class Server : public cSimpleModule
{
private:
   cQueue queue;	 //the queue; first job in the queue is being serviced
   cMessage *departure; //message-reminder of the end of service (job departure)
   simtime_t departure_time;  //time of the next departure

protected:
   virtual void initialize();
   virtual void handleMessage(cMessage *msgin);

};

Define_Module(Server);

void Server::initialize()
{
   departure = new cMessage("Departure");
}


void Server::handleMessage(cMessage *msgin)
{
   if (msgin==departure) //job departure
   {
      cMessage *msg = (cMessage *)queue.pop(); //remove job from the head of the queue
      send(msg,"out");
      if (!queue.isEmpty()) //schedule next departure event
      {
         departure_time=simTime()+par("service_time");
         scheduleAt(departure_time,departure);
      }
   }
   else //job arrival
   {
      if (queue.isEmpty())
      {
         departure_time=simTime()+par("service_time");
         scheduleAt(departure_time,departure);
      }
      queue.insert(msgin); //put job at the end of the queue
   }
}
