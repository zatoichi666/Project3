#include <iostream>
#include <ostream>
#include <sstream>
#include "Sender.h"
#include "Packetizer.h"
#include "filefind.h"
#include "FileSystem.h"


//----< demo thread constructor >------------------------------

SenderThread::SenderThread(BlockingQueue<std::string>& q, Socket sock, Packetizer& p) : _q(q), _s(sock), _p(p) {}

//----< getState returns thread health >-----------------------

HealthType SenderThread::getHealth()
{
	return _health; 
}

//----< Thread's implementation routine >----------------------

void SenderThread::run()
{
	sout << "Starting SenderThread\n";
	doLog("Starting SenderThread");

	_health = healthy;
	std::string line;

	int count = 0;
	// Get a line out of the queue, write the line to the socket
	do 
	{

		line = _q.deQ();
		if (!_s.writeLine(line))
		{
			sout << "Hit a bad line in SenderThread\n";
			_health = faulted;
			break;
		}
		count++;
	} while (line != "stop");
	_s.disconnect();
}


Talker::Talker(Packetizer& p) : _p(p) { }



void Talker::start(std::string ip, int port)
{
	sout << locker << "\n Sender #" << id() << " started" << unlocker;
	pSender = new SenderThread(_q, _s, _p);
	pSender->start();

	if(!_s.connect(ip, port))
	{
		sout << locker << "\n  couldn't connect to " << ip << ":" << port << "\n\n" << unlocker;
		delete pSender;
		return;
	}
	else
	{
		std::string logMsg = "\n  connected to " + ip + ":" + ToString(port);
		doLog(logMsg.c_str());
	}
	doLog("starting Talker");

	std::string msg;
	for(int i=0; i<_p.size(); ++i)
	{
		doLog("sending message");		
		_q.enQ(appendHeaderToBinaryPacket(ip, port, i) + _p[i]);

		if(pSender->getHealth() == faulted)
			break;
	}
	_q.enQ("stop");
	msg = "sender#" + ToString(id()) + ": stop";

	pSender->join();
	delete pSender;
}


std::string Talker::appendHeaderToBinaryPacket(std::string destIp, int destPort, int packetIndex)
{
	std::string header;


	header = "[sendBin;";
	header+="file='" + _p.getFileName() + "'";
	header+="pCount='" + ToString(_p.size()) + "'";
	header+="pInd='" + ToString(packetIndex) + "'";
	header+="dIp='" + destIp + "'";
	header+="dPort='" + ToString(destPort) + "'";
	header+= "]";

	return header;
}

int Talker::id() { return myCount; }

int Talker::count = 0;

#ifdef TEST_SENDER

class DemoThread : public threadBase
{
public:
	DemoThread(Talker sndr) : sndr_(sndr) {}
private:
	void run()
	{
		sndr_.start("127.0.0.1", 8080);
	}
	Talker sndr_;
};


void main()
{

	int ret = 0;
	try
	{

		Packetizer p("C:\\School\\CSE-687\\Project3\\Submission\\Test\\flyby_plusShaped.scn");
		Packetizer p2("C:\\School\\CSE-687\\Project3\\Submission\\Test\\flyby_plusShaped2.scn");

		// run two senders concurrently
		Talker sndr1(p);
		Talker sndr2(p2);
		DemoThread t1(sndr1);
		DemoThread t2(sndr2);
		t1.start();
		t2.start();
		t1.join();
		t2.join();
		sout << "\n\n  That's all Folks!\n\n";
	}
	catch(std::exception& ex)
	{
		sout << "\n\n  " << ex.what();
		ret = 1;
	}
	catch(...)
	{
		sout << "\n\n  something bad happend to a sender";
		ret = 1;
	}


}

#endif

