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
	ackMd5

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

	//SenderThread(BlockingQueue<std::string>& q, Socket sock, Packetizer& p);
	SenderThread(BlockingQueue<std::string>& q, Socket sock);
	HealthType_e getHealth();

private:
	virtual void run();
	BlockingQueue<std::string>& _q;
	Socket _s;
	//Packetizer& _p;
	HealthType_e _health;

};

class TextTalker
{
public:
	int id();
	TextTalker();	
	void TextTalker::start(messageType_e msgType, std::string ip, int port, std::string filename, std::string listenIp, int listenPort);
	
private:
	std::string TextTalker::makeQueryMd5AckMessage(std::string filename, std::string listenIp, int listenPort);
	std::string TextTalker::makeMd5AckMessage(std::string md5val, std::string ipSender, int portSender);
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
	//BinTalker(std::string pathToFile);
	//BinTalker(std::string filename);

	void start(std::string ip, int port);
	std::string makeSendBinAckMessage(std::string filename, std::string ipOfFileRecipient);
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