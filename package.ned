simple Source
{
    parameters:
        volatile double interarrival_time;
        volatile int initial_queue;

    gates:
        output out;
}


//////////////////////////////////////////////////
simple Server
{
    parameters:
        volatile double service_time;
    gates:
        input in;
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
        source: Source {
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
        source.out --> {  } --> server.in;
        server.out --> {  } --> sink.in;
}

//////////////////////////////////////////////////
network gg1 extends GG1
{
    parameters:
}
