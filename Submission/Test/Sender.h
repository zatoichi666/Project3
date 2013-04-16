/////////////////////////////////////////////////////////////////////////////
// Sender.h -  Demonstrates send-side communication per specification      //
//               for project 3, CSE-687, Spring 2013                       //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2012                             //
// Platform:    Dell Dimension E6510, Windows 7                            //
// Application: CSE-687                                                    //
// Author:      Matt Synborski                                             //
//              matthewsynborski@gmail.com                                 //
/////////////////////////////////////////////////////////////////////////////
// Package details:                                                        //
// BinTalker and TextTalker both perform perishable threaded socket        // 
// transmit.  BinTalker performs binary transmission with text headers,    //
// TextTalker performs text message transmission.                          //
// Both of these classes use the SenderThread class, and enqueue their     //
// Respective messages into strongly composed BlockingQueues.              //
/////////////////////////////////////////////////////////////////////////////


#ifndef SENDER_H
#define SENDER_H

#include <winsock2.h>
#include "Threads.h"
#include "Sockets.h"
#include "BlockingQueue.h"
#include "Packetizer.h"


typedef enum messageType_e
{
	sendBin,
	queryMd5,
	ackMd5,
	ackBin

};

typedef enum HealthType_e
{
	healthy,
	faulted
};

std::string ToString(int i);


class SenderThread : public threadBase
{
public:

	SenderThread(BlockingQueue<std::string>& q, Socket sock);
	HealthType_e getHealth();

private:
	virtual void run();
	BlockingQueue<std::string>& _q;
	Socket _s;
	HealthType_e _health;

};

class TextTalker
{
public:
	int id();
	TextTalker();	
	void start(messageType_e msgType, std::string ip, int port, std::string filename, std::string listenIp, int listenPort);
	
private:
	std::string makeQueryMd5AckMessage(std::string filename, std::string listenIp, int listenPort);
	std::string makeMd5AckMessage(std::string md5val, std::string ipSender, int portSender);
	std::string makeAckBinMessage(std::string filename, std::string listenIp, int listenPort);
	Socket _s;
	BlockingQueue<std::string> _q;
	SenderThread* pSender;
	int myCount;
	static int count;
};


class BinTalker
{
public:
	int id();
	BinTalker(Packetizer& p);
	void start(std::string ip, int port);
private:
	
	std::string appendHeaderToBinaryPacket(std::string destIp, int destPort, int packetIndex);
	Socket _s;
	BlockingQueue<std::string> _q;
	Packetizer& _p;
	SenderThread* pSender;
	int myCount;
	static int count;
};


#endif