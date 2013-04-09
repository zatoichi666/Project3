#ifndef SENDER_H
#define SENDER_H

#include <winsock2.h>
#include "Threads.h"
#include "Sockets.h"
#include "BlockingQueue.h"
#include "Packetizer.h"


typedef enum HealthType
{
	healthy,
	faulted
};

std::string ToString(int i)
{
	std::ostringstream conv;
	conv << i;
	return conv.str();
}


class SenderThread : public threadBase
{
public:

	SenderThread(BlockingQueue<std::string>& q, Socket sock, Packetizer& p);
	HealthType getHealth();

private:
	virtual void run();
	BlockingQueue<std::string>& _q;
	Socket _s;
	Packetizer& _p;
	HealthType _health;

};

class Talker
{
public:
	int id();
	Talker(Packetizer& p);

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