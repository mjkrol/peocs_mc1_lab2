//////////////////////////////////////////////////
simple Server
{
    parameters:
        volatile double service_time;
        volatile int buffer_max;
        volatile int initial_queue;
    gates:
        input in;
        output out;
}


//////////////////////////////////////////////////
module GG1
{
    parameters:

    submodules:
        server1: Server {
            parameters:
                @display("i=block/queue");
        }
        server2: Server {
            parameters:
                @display("i=block/queue");
        }
    connections:
        server1.out --> {  } --> server2.in;
        server2.out --> {  } --> server1.in;
}

//////////////////////////////////////////////////
network gg1 extends GG1
{
    parameters:
}
