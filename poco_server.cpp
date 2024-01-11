#include <iostream>
#include "Poco/Net/TCPServer.h"
#include "Poco/Net/TCPServerParams.h"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Net/Socket.h"

using namespace std;

#define BUFFER_SIZE         255
#define TCP_PORT            28888
#define COUNT_THREAD        14
#define COUNT_QUEUE         14
#define THREAD_IDLE_TIME    100

class newConnection: public Poco::Net::TCPServerConnection
{
    void reverseBytes(unsigned char *ucBuffer, int size)
    {
        unsigned char ch;
        int i = 0;
        for(i = 0; i < size/2; i++)
        {
            ch = ucBuffer[i];
            ucBuffer[i] = ucBuffer[size - 1 - i];
            ucBuffer[size -1 - i] = ch;
        }
    }
    public:
        newConnection(const Poco::Net::StreamSocket& s) :
            Poco::Net::TCPServerConnection(s)
        {
        }

        void run()
        {
            cout << "New connection from: " << socket().peerAddress().host().toString() <<  endl << flush;
            bool isOpen = true;
            Poco::Timespan timeOut(10,0);
            unsigned char ucBuffer[BUFFER_SIZE];
            while(isOpen)
            {
                if (socket().poll(timeOut,Poco::Net::Socket::SELECT_WRITE) == false)
                {
                    cout << "TIMEOUT!" << endl << flush;
                }
                else
                {
                    int nBytes = -1; 
                    try
                    {
                        nBytes = socket().sendBytes("Welcome to POCO TCP server. Enter you string:\n", 46);
                        if(nBytes > 0)
                        {
                            if(nBytes > BUFFER_SIZE)
                            {
                                socket().sendBytes("The message length is more than 255\n", 36);
                            }
                            else
                            {
                                while(isOpen)
                                {
                                    nBytes = socket().receiveBytes(ucBuffer, sizeof(ucBuffer));
                                    if(nBytes > 0)
                                    {
                                        reverseBytes(ucBuffer, nBytes-2);
                                        socket().sendBytes(ucBuffer, nBytes);
                                        memset(ucBuffer, 0, BUFFER_SIZE);
                                    }
                                }
                            }
                        }
                    }
                    catch (Poco::Exception& exc)
                    {
                        cerr << "Network error: " << exc.displayText() << endl;
                        isOpen = false;
                    }
                }
            }
            cout << "Connection finished!" << endl << flush;
        }
};

int main(int argc, char** argv)
{
    Poco::Net::ServerSocket srv(TCP_PORT);
    Poco::Net::TCPServerParams *pParams = new Poco::Net::TCPServerParams();
    pParams->setMaxThreads(COUNT_THREAD);
    pParams->setMaxQueued(COUNT_QUEUE);
    pParams->setThreadIdleTime(THREAD_IDLE_TIME);
    Poco::Net::TCPServer myServer(new Poco::Net::TCPServerConnectionFactoryImpl<newConnection>(), srv, pParams);
    myServer.start();

    while(1)
    {
        ;
    }
    return 0;
}
