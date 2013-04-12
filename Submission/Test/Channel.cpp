#ifndef CHANNEL_H
#define CHANNEL_H

#include "Sender.h"
#include "Listener.h"

//#include "Channel.h"
#include <iostream>
#include <thread>

class Channel
{
public:

	Channel::Channel(Receiver r, BinTalker s) : _recv(r), _sndr(s)	
	{ 

	}

	void Channel::sendFile(std::string ip, int port)
	{
		_sndr.start(ip, port);
	}

	void Channel::receiveFile(int listenPort)
	{
		_recv.start(listenPort);
	}

private:	
	Receiver _recv;
	BinTalker _sndr;	
};

#ifdef TEST_CHANNEL

/*
class DemoThread : public threadBase
{
public:
DemoThread(Channel chan) : chan_(chan) {}
private:
void run()
{

Packetizer p("C:\\School\\CSE-687\\Project3\\Submission\\Test\\flyby_plusShaped.scn");
BinTalker t(p);


}
Channel chan_;
};
*/

int main()
{
	int listenPort = 8050;
	std::string listenIp = "127.0.0.1";
	int sendPort = 8080;
	std::string sendIp = "127.0.0.1";

	Packetizer p("C:\\School\\CSE-687\\Project3\\Submission\\Test\\flyby_plusShaped.scn");
	Packetizer p3("C:\\School\\CSE-687\\Project3\\Submission\\Test\\flyby_plusShaped3.scn");
	//BinTalker should create a packetizer only when sending a file
	Receiver r;
	Receiver r3;

	BinTalker t(p);
	BinTalker t3(p3);
	
	Channel c(r, t);
	Channel c3(r3, t3);

	std::thread thr([&]() {c.receiveFile(listenPort);});
	//c.receiveFile(8050);

	std::thread thr3([&]() {c3.sendFile(sendIp, sendPort);});

	c.sendFile(sendIp, sendPort);

	TextTalker ta;
	ta.start(queryMd5, sendIp, sendPort, "flyby_plusShaped.scn",listenIp, listenPort );
	

	thr3.join();
	thr.join();
	

	return 0;
}

#endif
#endif