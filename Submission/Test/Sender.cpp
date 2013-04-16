/////////////////////////////////////////////////////////////////////////////
// Sender.cpp -  Demonstrates send-side communication per specification    //
//               for project 3, CSE-687, Spring 2013                       //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2012                             //
// Platform:    Dell Dimension E6510, Windows 7                            //
// Application: CSE-687                                                    //
// Author:      Matt Synborski                                             //
//              matthewsynborski@gmail.com                                 //
/////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <ostream>
#include <sstream>
#include "Sender.h"
#include "Packetizer.h"
#include "filefind.h"
#include "FileSystem.h"
#include "md5.h"

std::string ToString(int i)
{
	std::ostringstream conv;
	conv << i;
	return conv.str();
}

//----< Sender thread constructor >------------------------------

SenderThread::SenderThread(BlockingQueue<std::string>& q, Socket sock) : _q(q), _s(sock) {}

//----< getHealth returns thread health >-----------------------

HealthType_e SenderThread::getHealth()
{
	return _health; 
}

//----< Sender Thread's implementation routine >----------------------

void SenderThread::run()
{
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

//----< Text-based message sender constructor >------------------------------

TextTalker::TextTalker() {}

//----< Text based-message start handles each type of text message enumerated by Sender.h-> messageType_e >------------------------------

void TextTalker::start(messageType_e msgType, std::string ip, int port, std::string payload, std::string listenIp, int listenPort)
{
	sout << locker << " TextTalker: " << id() << " started" << unlocker << "\n";
	pSender = new SenderThread(_q, _s);
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
	doLog("starting TextTalker");

	if (msgType == queryMd5)
		_q.enQ(makeQueryMd5AckMessage(payload, listenIp, listenPort ));

	if (msgType == ackMd5)
		_q.enQ(makeMd5AckMessage(payload, listenIp, listenPort ));

	if (msgType == ackBin)
		_q.enQ(makeAckBinMessage(payload, ip, port ));

	_q.enQ("stop");

	pSender->join();
	delete pSender;
}

//----< Text-based message for querying a remote file's MD5 signature >----------------------

std::string TextTalker::makeQueryMd5AckMessage(std::string filename, std::string ipSender, int portSender)
{
	std::string header;

	header = "[queryMd5;";
	header+="file='" + filename + "'";
	header+="ipSender='" + ipSender + "'";
	header+="portSender='" + ToString(portSender) + "'";
	header+= "]";

	return header;
}

//----------< Text-based message for servicing a query >------------------------------------- 
//----------< calculates MD5 and sends the result back to the queryer >----------------------

std::string TextTalker::makeMd5AckMessage(std::string md5val, std::string ipSender, int portSender)
{
	std::string header;

	header = "[ackMd5;";
	header+="ipSender='" + ipSender + "'";
	header+="portSender='" + ToString(portSender) + "'";
	header+="md5val='" + md5val + "'";
	header+= "]";

	return header;
}

//----------< Text-based message for confirming receipt of a complete binary file >------------------------------------- 
//----------< Doesn't calculate MD5, that is a separate optional transaction via queryMD5 >----------------------

std::string TextTalker::makeAckBinMessage(std::string fileName, std::string ipSender, int portSender)
{
	std::string header;
	header = "[ackBin;";
	header+="file='" + fileName + "'";
	header+="ipSender='" + ipSender + "'";
	header+="portSender='" + ToString(portSender) + "'";	
	header+= "]";

	return header;
}

int TextTalker::id() { return myCount; }

int TextTalker::count = 0;


//----< Binary-plus-text-based message sender constructor >------------------------------

BinTalker::BinTalker(Packetizer& p) : _p(p) {}

void BinTalker::start(std::string ip, int port)
{
	sout << locker << " BinTalker sending: " << _p.getFileName() << " started" << unlocker << "\n";
	pSender = new SenderThread(_q, _s);
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
	doLog("starting BinTalker");

	std::string msg;
	for(size_t i=0; i<_p.size(); ++i)
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

//-----------< Takes a binary packet and prepends a text-based header >--------------------------

std::string BinTalker::appendHeaderToBinaryPacket(std::string destIp, int destPort, int packetIndex)
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

int BinTalker::id() { return myCount; }

int BinTalker::count = 0;

#ifdef TEST_SENDER

class DemoThread : public threadBase
{
public:
	DemoThread(BinTalker sndr) : sndr_(sndr) {}
private:
	void run()
	{
		sndr_.start("127.0.0.1", 8080);
	}
	BinTalker sndr_;
};

void main()
{

	int ret = 0;
	try
	{
		Packetizer p("C:\\School\\CSE-687\\Project3\\Submission\\Test\\flyby_plusShaped.scn");
		Packetizer p2("C:\\School\\CSE-687\\Project3\\Submission\\Test\\flyby_plusShaped2.scn");

		// run two senders concurrently
		BinTalker sndr1(p);
		BinTalker sndr2(p2);
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

