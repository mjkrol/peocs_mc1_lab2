simple Source
{
    parameters:
        volatile double interarrival_time;
    gates:
        output out;
}


//////////////////////////////////////////////////
simple Server
{
    parameters:
        volatile double mi;
        volatile double service_time;
        volatile int buffer_max;
    gates:
        input in[];
        output out;
}


//////////////////////////////////////////////////
simple Sink
{
    gates:
        input in;
}


//////////////////////////////////////////////////
module GG1
{
    parameters:

    submodules:
        source1: Source {
            parameters:
                @display("i=block/source");
        }
        source2: Source {
            parameters:
                @display("i=block/source");
        }
        server: Server {
            parameters:
                @display("i=block/queue");
        }
        sink: Sink {
            parameters:
                @display("i=block/sink");
        }
    connections:
        source1.out --> {  } --> server.in++;
        source2.out --> {  } --> server.in++;
        server.out --> {  } --> sink.in;
}

//////////////////////////////////////////////////
network gg1 extends GG1
{
    parameters:
}
