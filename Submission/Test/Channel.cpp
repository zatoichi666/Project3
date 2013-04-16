/////////////////////////////////////////////////////////////////////////////
// Channel.cpp - Demonstrates socket communication per specification       //
//               for project 3, CSE-687, Spring 2013                       //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2012                             //
// Platform:    Dell Dimension E6510, Windows 7                            //
// Application: CSE-687                                                    //
// Author:      Matt Synborski                                             //
//              matthewsynborski@gmail.com                                 //
/////////////////////////////////////////////////////////////////////////////

#ifndef CHANNEL_H
#define CHANNEL_H

#include "Sender.h"
#include "Listener.h"

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

int main()
{
	int listenPort = 8050;
	std::string listenIp = "127.0.0.1";
	int sendPort = 8080;
	std::string sendIp = "127.0.0.1";

	Packetizer p("C:\\School\\CSE-687\\Project3\\Submission\\Test\\flyby_plusShaped.scn");
	Packetizer p3("C:\\School\\CSE-687\\Project3\\Submission\\Test\\flyby_plusShaped3.scn");
	
	sout << "\n";
	sout << "Demonstrating requirements 3 - 7 below.\n";

	Receiver r;
	Receiver r3;

	BinTalker t(p);
	BinTalker t3(p3);

	Channel c(r, t);
	Channel c3(r3, t3);

	std::thread thr([&]() {c.receiveFile(listenPort);});
	
	std::thread thr3([&]() 
	{
		c3.sendFile(sendIp, sendPort);
		TextTalker ta3;
		ta3.start(queryMd5, sendIp, sendPort, "flyby_plusShaped3.scn",listenIp, listenPort );
	});

	c.sendFile(sendIp, sendPort);
	TextTalker ta;
	ta.start(queryMd5, sendIp, sendPort, "flyby_plusShaped.scn",listenIp, listenPort );

	thr.join();
	thr3.join();
	return 0;
}

#endif
#endif