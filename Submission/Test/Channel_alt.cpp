#ifndef CHANNELALT_H
#define CHANNELALT_H

#include "Sender.h"
#include "Listener.h"

#include <iostream>
#include <thread>

class ChannelAlt
{
public:


	ChannelAlt::ChannelAlt(Receiver r, BinTalker s) : _recv(r), _sndr(s)	
	{ 

	}

	void ChannelAlt::sendFile(std::string ip, int port)
	{
		_sndr.start(ip, port);
	}

	void ChannelAlt::receiveFile(int listenPort)
	{
		_recv.start(listenPort);
	}

private:	
	Receiver _recv;
	BinTalker _sndr;	
};

#ifdef TEST_CHANNELALT

int main()
{
	int listenPort = 8080;
	std::string listenIp = "127.0.0.1";
	int sendPort = 8050;
	std::string sendIp = "127.0.0.1";

	Packetizer p2("C:\\School\\CSE-687\\Project3\\Submission\\Test\\flyby_plusShaped2.scn");
	Packetizer p4("C:\\School\\CSE-687\\Project3\\Submission\\Test\\flyby_plusShaped4.scn");
	//BinTalker should create a packetizer only when sending a file
	Receiver r2;
	Receiver r4;

	BinTalker t2(p2);
	BinTalker t4(p4);
	
	ChannelAlt c2(r2, t2);
	ChannelAlt c4(r4, t4);

	std::thread thr2([&]() {c2.receiveFile(listenPort);});
	std::thread thr4([&]() 
	{
		c4.sendFile(sendIp, sendPort);
		TextTalker ta4;
		ta4.start(queryMd5, sendIp, sendPort, "flyby_plusShaped4.scn",listenIp, listenPort );
	});

	c2.sendFile(sendIp, sendPort);
	TextTalker ta2;
	ta2.start(queryMd5, sendIp, sendPort, "flyby_plusShaped2.scn",listenIp, listenPort );

	thr2.join();
	thr4.join();
}

#endif
#endif