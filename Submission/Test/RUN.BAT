:run.bat

cd debug
echo off

cls
echo Testing base64
pause
base64.exe
pause

cls
echo Testing md5.exe
pause
md5.exe
pause

cls
echo Testing FileFind.exe
pause
FileFind.exe
pause

cls
echo Testing Packetizer.exe
pause
Packetizer.exe
pause


cls
echo Testing Sender and Listener.exe (together), press enter when ready
pause
start Listener.exe
start Sender.exe
pause

cls
echo Now testing JFawcett packages
pause


cls
echo Testing NewBlockingQueue.exe
pause
NewBlockingQueue.exe
pause

cls
echo Testing NewLocks.exe
pause
NewLocks.exe
pause

cls
echo Testing NewThreads.exe
pause
NewThreads.exe
pause

cls
echo Testing Sockets.exe
pause
Sockets.exe
pause

cd ..