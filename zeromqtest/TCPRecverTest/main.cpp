#include"zhelpers.hpp"
#include <iostream>
#include <sstream>

//int main (int argc, char *argv[])
//{
//    zmq::context_t context (1);
//    zmq::socket_t socket (context, ZMQ_REQ);
//    std::cout << "Connecting to hello world server..." << std::endl;
//    socket.connect ("tcp://192.168.5.84:10125");
//    s_sleep(1000);
//    //  Subscribe to zipcode, default is NYC, 10001

//    //  Process 100 updates
//    while(1){
//        static int count=0;
////        zmq::message_t request (5);
////        memcpy (request.data (), "Hello", 5);
////        std::cout << "Sending Hello " << count << "..." << std::endl;
////        subscriber.send (request);

//        //  Get the reply.
//        zmq::message_t reply;
//        socket.recv (&reply);
//        std::cout << "Received World " <<reply<< count << std::endl;
//    }
//    return 0;
//}

#include <zmq.hpp>
#include <string>
#include <iostream>
/*
    当使用 tcp:// transport 地址时，ZeroMQ 的 ZMQ_STREAM socket 可以用于在 non-ZMQ socket 发送和接收TCP数据。一个 ZMQ_STREAM socket 能作为客户端和/或服务器，异步的发送和/或接收TCP数据。
    当接收TCP数据时，ZMQ_STREAM socket 会在源消息之前预添加一个包含源端身份认证（identity ）消息。收到的消息是以公平队列的形式发出的。
    当发送TCP数据时，ZMQ_STREAM socket 会将包含源端身份认证（identity ）消息移除。当消息不能被发送时将会触发EHOSTUNREACH或EAGAIN错误。
    要关闭一个指定的客户端连接，作为服务器，发送身份标识帧后跟一个长度为零的消息即可（见例部分）。
    ZMQ_MSGMORE 标识在数据帧是被忽略的。你必须发送一个标识帧后面接一个数据帧。另外，请注意，省略ZMQ_MSGMORE 标志将防止同一socket上发送更多的数据（从任何客户端）。
*/
int main (int argc, char *argv[])
{
    //  Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_STREAM);//
    std::cout << "Connecting to hello world server..." << std::endl;
    socket.connect ("tcp://192.168.5.84:10125");

    //  Do 10 requests, waiting each time for a response
//    for (int request_nbr = 0; request_nbr != 10; request_nbr++) {
//        zmq::message_t request (5);
//        memcpy (request.data (), "Hello", 5);
//        std::cout << "Sending Hello " << request_nbr << "..." << std::endl;
//        socket.send (request);
    while(1){
        //  Get the reply.
        zmq::message_t reply;
        socket.recv (&reply);
        std::cout << "Received: " << reply << std::endl;
    }

//    }
    return 0;
}
