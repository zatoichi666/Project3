#include <iostream>
#include <ostream>
#include <sstream>
#include "Listener.h"
#include "FileSystem.h"
#include "base64.h"

#define TRACING

void ClientHandlerThread::run()
{
	std::string msg;
	do
	{
		doLog("receive thread reading line");
		msg = s_.readLine();
		if(msg == "")
			break;
		q_.enQ(msg);
	} while(msg != "quit");

	sout << "ClientThread: Got stop!\n";
}

void ListenThread::run()
{
	while(!stop_)
	{
		SOCKET s = listener_.waitForConnect();
		ClientHandlerThread* pCh = new ClientHandlerThread(s, q_);
		pCh->start();
	}
}

void Receiver::start(int port)
{
	sout << "Receiver started on port " << port << "\n";
	pLt = new ListenThread(port, q_);
	try
	{
		pLt->start();
		std::string msg;
		int count = 0;
		do
		{
			msg = q_.deQ();
			processMessage(msg);
		} while(true);
	}
	catch(std::exception& ex)
	{
		delete pLt;
	}
	catch(...)
	{
		delete pLt;
	}
}

std::vector<FileSystem::File> Receiver::getFileList()
{
	return fileList;
}


//----< Act on the received message >--------------------------------------

void Receiver::processMessage(std::string message)
{
	size_t posOpenSquareBracket = message.find_first_of("[");
	size_t posMsgTypeSemicolon = message.find_first_of(";");

	std::string messageType = message.substr(posOpenSquareBracket+1,posMsgTypeSemicolon - posOpenSquareBracket - 1);

	if (messageType == "sendBin")
	{
		
		size_t posFileHeader = message.find("file='") + 6;
		size_t fileEntryLength = message.find("'",posFileHeader) - posFileHeader;
		std::string fileName = message.substr(posFileHeader,fileEntryLength);
		size_t pospCountHeader = message.find("pCount='") + 8;
		size_t pCountEntryLength = message.find("'",pospCountHeader) - pospCountHeader;
		std::string pCount_s = message.substr(pospCountHeader,pCountEntryLength);

		int pCount;
		if ( ! (std::istringstream(pCount_s) >> pCount) ) pCount = 0;

		size_t pospIndHeader = message.find("pInd='") + 6;
		size_t pIndEntryLength = message.find("'",pospIndHeader) - pospIndHeader;
		std::string pInd_s = message.substr(pospIndHeader,pIndEntryLength);

		int pInd;
		if ( ! (std::istringstream(pInd_s) >> pInd) ) pInd = 0;

		size_t posdIpHeader = message.find("dIp='") + 5;
		size_t dIpEntryLength = message.find("'",posdIpHeader) - posdIpHeader;
		std::string dIp = message.substr(posdIpHeader,dIpEntryLength);
		size_t posdPortHeader = message.find("dPort='") + 7;
		size_t dPortEntryLength = message.find("'",posdPortHeader) - posdPortHeader;
		std::string dPort_s = message.substr(posdPortHeader,dPortEntryLength);

		int dPort;
		if ( ! (std::istringstream(dPort_s) >> dPort) ) dPort = 0;

		size_t posBinBegin = message.find("]") + 1;
		size_t posBinEnd = message.size();
		sout << "Got a sendBin message\n";
		std::string binBlock = message.substr(posBinBegin,posBinEnd - posBinBegin);
		std::string binDec = base64::base64_decode(binBlock);
		FileSystem::Block b(&binDec[0],&binDec[binDec.size()]);
		FileSystem::File outFile(fileName);
		if (pInd == 0)
			outFile.open(FileSystem::File::out, FileSystem::File::binary);
		else
			outFile.open(FileSystem::File::append, FileSystem::File::binary);
		if(outFile.isGood())
			outFile.putBlock(b);
	}

}

#ifdef TEST_LISTENER

const std::string base64::base64_chars = 
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz"
	"0123456789+/";

void main()
{
	int ret = 0;
	try
	{
		Receiver rcvr;
		rcvr.start(8080);
	}
	catch(std::exception& ex)
	{
		std::cout << "\n\n  " << ex.what();
		ret = 1;
	}
	catch(...)
	{
		sout << "\n  something bad happened";
		ret = 1;
	}
	sout << "\n\n";
}
#endif