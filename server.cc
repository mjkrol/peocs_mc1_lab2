#include <omnetpp.h>
#include <iostream>
#include <fstream>

using namespace omnetpp;

class Server : public cSimpleModule
{
private:
   int N;
   cQueue queue;	 //the queue; first job in the queue is being serviced
   cMessage *departure; //message-reminder of the end of service (job departure)

   // queue len statistics
   cMessage *sampleQueueLength;
   cLongHistogram queueStats;

   // mean time in queue statistics
   cStdDev serviceTimeStats;

   // packet loss statistics
   uint64_t packetsTotal;
   uint64_t packetsLost;

protected:
   virtual void initialize();
   virtual void handleMessage(cMessage *msgin);
   virtual void finish();

};

Define_Module(Server);

void Server::initialize()
{
   N = par("buffer_max");
   departure = new cMessage("Departure");
   sampleQueueLength = new cMessage("SampleQueueLength");
   scheduleAt(simTime() + 1, sampleQueueLength);
   queueStats.setCellSize(1);
   queueStats.setRange(0, N + 1);
   packetsTotal = 0;
   packetsLost = 0;
}


void Server::handleMessage(cMessage *msgin)
{
   simtime_t now = simTime();

   if (msgin == departure) //job departure
   {
      cMessage *msg = (cMessage *)queue.pop(); //remove job from the head of the queue
      send(msg,"out");

      serviceTimeStats.collect((now - msgin->getSendingTime()).dbl());

      if (!queue.isEmpty()) //schedule next departure event
      {
         scheduleAt(simTime() + par("service_time"), departure);
      }
   }
   else if (msgin == sampleQueueLength)
   {
      queueStats.collect(queue.getLength());
      scheduleAt(now + 1, sampleQueueLength);
   }
   else if (queue.getLength() < N)
   {
      if (queue.isEmpty())
      {
         scheduleAt(now + par("service_time"), departure);
      }
      queue.insert(msgin); //put job at the end of the queue
      packetsTotal++;
   } else {
      // Queue full, drop the job
      delete msgin;
      packetsLost++;
      packetsTotal++;
   }
}

void Server::finish()
{
   EV << "mi = " << 1.0/(double)par("mi") << ", N = " << N << endl;
   EV << "Mean service time: " << serviceTimeStats.getMean() << endl;
}
