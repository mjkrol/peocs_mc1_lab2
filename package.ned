simple Source
{
    parameters:
        volatile double interarrival_time;
        volatile int kind;
    gates:
        output out;
}


//////////////////////////////////////////////////
simple Server
{
    parameters:
        volatile double service_time_A;
        volatile double service_time_B;
        volatile int N;
        volatile double probability_0;
        volatile int priority;
    gates:
        input in[];
        output out[];
}


//////////////////////////////////////////////////
simple Sink
{
    gates:
        input in[];
}


//////////////////////////////////////////////////
module GG1
{
    parameters:

    submodules:
        sourceA: Source {
            parameters:
                @display("i=block/source");
        }
        sourceB: Source {
            parameters:
                @display("i=block/source");
        }
        fifo1: Server {
            parameters:
                @display("i=block/queue");
        }
        fifo2: Server {
            parameters:
                @display("i=block/queue");
        }
        fifo3: Server {
            parameters:
                @display("i=block/queue");
        }
        sink: Sink {
            parameters:
                @display("i=block/sink");
        }
    connections:
        sourceA.out --> {  } --> fifo1.in++;
        sourceB.out --> {  } --> fifo1.in++;
        fifo1.out++ --> {  } --> fifo2.in++;
        fifo1.out++ --> {  } --> fifo3.in++;
        fifo2.out++ --> {  } --> sink.in++;
        fifo3.out++ --> {  } --> sink.in++;
}

//////////////////////////////////////////////////
network gg1 extends GG1
{
    parameters:
}
