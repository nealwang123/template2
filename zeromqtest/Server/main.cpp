//
//  Weather update server in C++
//  Binds PUB socket to tcp://*:5556
//  Publishes random weather updates
//
//  Olivier Chamoux <olivier.chamoux@fr.thalesgroup.com>
//
#include <zmq.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#if (defined (WIN32))
#include <zhelpers.hpp>
#endif
#include<iostream>
using namespace std;
#define within(num) (int) ((float) num * random () / (RAND_MAX + 1.0))


int main () {

    //  Prepare our context and publisher
    zmq::context_t context (1);
    zmq::socket_t publisher (context, ZMQ_PUB);
    publisher.connect("tcp://localhost:8888");
    /*on Windows, you can really only use 'inproc', TCP, or PGM. */
//    publisher.bind("ipc://weather.ipc");				// Not usable on Windows.

    //  Initialize random number generator
    srandom ((unsigned) time (NULL));
    int count =0;
    while (1) {

        char* zipcode;
        double temperature, relhumidity;
        int stop;
        //  Get values that will fool the boss
//        zipcode     = within (10);
        zipcode     = "usbControl";
        temperature =0;
        relhumidity = 0;
        stop=0;

        //  Send message to all subscribers
        zmq::message_t message(34);
        snprintf ((char *) message.data(), 34 ,"%s %-10.2f %10.2f %d", zipcode, temperature, relhumidity,stop);
//        publisher.send(message);
        s_sendmore(publisher,string("BBB"));
        s_sendmore(publisher,string("ID:B "));
        s_send(publisher,string("111111111111111111111111111111111111111111"));
        s_sendmore(publisher,string("AAA"));
        s_sendmore(publisher,string("ID:A "));
        s_send(publisher,string("222222222222222222222222222222222222222222"));
        cout<<"pub:"<<message.data()<<endl;
//        s_console((char *) message.data());
        s_sleep(5000);
        count++;
    }
    return 0;
}
