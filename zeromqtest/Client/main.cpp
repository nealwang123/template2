//  Weather update client in C++
//  Connects SUB socket to tcp://localhost:5556
//  Collects weather updates and finds avg temp in zipcode
//
//  Olivier Chamoux <olivier.chamoux@fr.thalesgroup.com>
//
#include"zhelpers.hpp"
#include <iostream>
#include <sstream>

int main (int argc, char *argv[])
{
    zmq::context_t context (1);
    //  Socket to talk to server
    std::cout << "Collecting updates from weather server...\n" << std::endl;
    zmq::socket_t subscriber (context, ZMQ_SUB);
    subscriber.connect("tcp://localhost:9999");
    //  Subscribe to zipcode, default is NYC, 10001
    const char *filter = (argc > 1)? argv [1]: "BBB";//s
    subscriber.setsockopt(ZMQ_SUBSCRIBE, filter, strlen (filter));
    zmq::message_t update;
    char* zipcode;
    double xpos, ypos;
    int count;


    //  Process 100 updates
    while(1){
        //  Read envelope with address
        std::string address = s_recv (subscriber);
        //  Read id with address
        std::string identify = s_recv (subscriber);
        //  Read message contents
        std::string contents = s_recv (subscriber);

        std::cout << "[" << address << "] "<< "[" << identify << "] " << contents << std::endl;
//        subscriber.recv(&update);
//        std::istringstream iss(static_cast<char*>(update.data()));
//        iss >> zipcode >> xpos >> ypos >>count;

//        std::cout 	<< "Average temperature for zipcode '"<< filter
//                    <<"xpos:"<<xpos<<" "
//                    <<"ypos:"<<ypos<<" "
//                    <<"count:"<<count<<" "
//                    << std::endl;
        s_sleep(50);;
    }

    return 0;
}
