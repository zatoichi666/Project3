/////////////////////////////////////////////////////////////////////////////
// Listener.h   - Demonstrates receive-side socket comms per specification //
//               for project 3, CSE-687, Spring 2013                       //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2012                             //
// Platform:    Dell Dimension E6510, Windows 7                            //
// Application: CSE-687                                                    //
// Author:      Matt Synborski                                             //
//              matthewsynborski@gmail.com                                 //
/////////////////////////////////////////////////////////////////////////////
// Package details:                                                        //
// Receivers aggregate ListenThreads, which wait for incoming connections  //
// When a ListenThread gets a connection, it aggregates a ClientHandler    //
// Thread, which pulls messages out of the Socket and stuffs them into     // 
// A BlockingQueue.  The Receiver retrieves the messages from the Blocking //
// Queue, and handles their contents.  The Receiver to ListenThread        //
// relationship is 1:1, while the ListenThread to ClientHandlerThread      //
// relationship is 1:*, this way, a single Listener is designed to handle  //
// concurrent socket communication with Senders from multiple connections. //
/////////////////////////////////////////////////////////////////////////////

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
	void Receiver::sendAckBinMsg(std::string fileName, int port, std::string ip );
	void Receiver::processAckBinMsg(std::string message );
	void Receiver::processQueryMd5Msg(std::string message );
	void Receiver::processAckMd5Msg(std::string message );
	FileSystem::FileInfo Receiver::processSendBinMsg(std::string message, int& isLastPacket);
	std::vector<FileSystem::File> fileList;
};

#endif