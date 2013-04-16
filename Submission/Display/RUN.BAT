:run.bat

cd debug
echo off
echo Displaying technical requirements 3 - 7 for CSE-687 Spring 2013 Project 3
echo Please move the window for Channel2.exe aside from Channel1.exe so concurrent functionality can be 
echo viewed.  Also please close these windows once the ackBin messages are displayed, since receive thread is nonterminating.
echo
echo Please make sure to review the RequirementsTraceMatrix.xlsx file.  Thanks!
echo -Matt Synborski, matthewsynborski@gmail.com 

pause
start Channel1.exe
start Channel2.exe