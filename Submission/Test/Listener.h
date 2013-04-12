#ifndef LISTENER_H
#define LISTENER_H

#include <winsock2.h>
#include "Threads.h"
#include "Sockets.h"
#include "BlockingQueue.h"
#include "FileSystem.h"

//std::string ToString(int i);

///////////////////////////////////////////////////////////////
// ReceverThread derived class

class ReceiverThread : public threadBase
{
public:
	ReceiverThread(BlockingQueue<std::string>& q, Socket sock);
private:
	gCSLock<1> tLock;
	virtual void run();

	BlockingQueue<std::string>& _q;
	Socket& _s;
};


///////////////////////////////////////////////////
// ClientHandlerThread thread

class ClientHandlerThread : public tthreadBase
{
public:
	ClientHandlerThread(Socket s, BlockingQueue<std::string>& q) : s_(s), q_(q) {}
private:
	void run();
	
	Socket s_;
	BlockingQueue<std::string>& q_;
};

///////////////////////////////////////////////////
// listenThread thread
class ListenThread : public threadBase
{
public:
	ListenThread(int port, BlockingQueue<std::string>& q) : listener_(port), q_(q), stop_(false) {}
	void stop(bool end) { stop_ = end; }
private:
	void run();
	bool stop_;
	BlockingQueue<std::string>& q_;
	SocketListener listener_;
};


class Receiver
{
public:
	void start(int port);
	std::vector<FileSystem::File> getFileList();
	
private:
	BlockingQueue<std::string> q_;
	ListenThread* pLt;
	void processMessage(std::string message);
	void Receiver::processQueryMd5Msg(std::string message );
	void Receiver::processAckMd5Msg(std::string message );
	FileSystem::FileInfo Receiver::processSendBinMsg(std::string message, int& isLastPacket);
	std::vector<FileSystem::File> fileList;
};

#endif