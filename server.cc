#include <assert.h>
#include <omnetpp.h>
#include <iostream>
#include <fstream>

using namespace omnetpp;

class Server : public cSimpleModule
{
public:
   Server();
   ~Server();

private:
   int N;
   cQueue queue;
   cQueue queuePriority;
   cMessage *served;
   cMessage *departure;

   // queue len statistics
   cMessage *sampleQueueLength;
   cLongHistogram queueStats;

   // mean time in queue statistics
   cStdDev serviceTimeStats;

protected:
   virtual void initialize();
   virtual void handleMessage(cMessage *msgin);
   virtual void finish();
private:
   void PrintStats(std::ostream &);
   void StartServing(cMessage *message);
};

Define_Module(Server);

Server::Server()
{
   served = NULL;
   departure = NULL;
   sampleQueueLength = NULL;
}

Server::~Server()
{
   delete departure;
}

void Server::initialize()
{
   N = par("N");
   departure = new cMessage("Departure");
   sampleQueueLength = new cMessage("SampleQueueLength");
   scheduleAt(simTime() + 1, sampleQueueLength);
   queueStats.setCellSize(1);
   queueStats.setRange(0, N + 1);
}

void Server::handleMessage(cMessage *msgin)
{
   simtime_t now = simTime();

   if (msgin == departure) {
      assert(served != NULL);

      serviceTimeStats.collect((now - served->getSendingTime()).dbl());
      if ((double)par("probability_0") < 0.5) {
         send(served, "out", 0);
      } else {
         send(served, "out", 1);
      }
      served = NULL;

      if (!queuePriority.isEmpty()) {
         StartServing((cMessage *)queuePriority.pop());
      } else if (!queue.isEmpty()) {
         StartServing((cMessage *)queue.pop());
      }
   } else if (msgin == sampleQueueLength) {
      queueStats.collect(queue.getLength() + queuePriority.getLength());
      scheduleAt(now + 1, sampleQueueLength);
   } else if (queue.getLength() + queuePriority.getLength() < N) {
      if (served == NULL) {
         StartServing(msgin);
      } else {
         if (msgin->getKind() == 1 && (int)par("priority") == 1) {
            queuePriority.insert(msgin);
         } else {
            queue.insert(msgin);
         }
      }
   } else {
      delete msgin;
   }
}

void Server::StartServing(cMessage *msg)
{
   assert(msg != NULL);
   assert(served == NULL);

   served = msg;
   if (served->getKind() == 1) {
      scheduleAt(simTime() + par("service_time_A"), departure);
   } else {
      scheduleAt(simTime() + par("service_time_B"), departure);
   }
}

void Server::finish()
{
   PrintStats(EV);
   std::ofstream f;
   f.open("gg1-lab3.txt", std::ios::app);
   PrintStats(f);
   f.close();
}

void Server::PrintStats(std::ostream &out)
{
   out << "--------------------" << endl;
   out << "name = " << getName() << endl;
   out << "miA = " << 1.0/(double)par("service_time_A") << endl;
   out << "miB = " << 1.0/(double)par("service_time_B") << endl;
   out << "Queue mean:   " << queueStats.getMean() << endl;
   for (int i = 0; i <= N; i++) {
      EV << "P(" << i << ") = " << queueStats.getCellValue(i) / queueStats.getCount() << endl;
   }
   out << endl;
}
