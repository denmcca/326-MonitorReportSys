#include "Sender.h"
#include <iostream>
#include "PTools.cpp"
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string>
#include <cstring>
#include <stdlib.h>     
#include <time.h>       
#include <sys/types.h>
#include <unistd.h>

/*
	Using as test stub for testing with Receiver program. - Dennis 
	
	Ideas:
	-- How to manage deallocation of queue 
	Receiver 1 creates queue, receiver 2 sends special message to queue, 
	receiver 1 set to terminate, receiver 1 checks if queue is empty to
	deallocate queue, receiver 2 sets to terminate, receiver 2 checks if
	queue is empty, queue is not empty, receiver 2 retrieves any message,
	receiver 2 checks if queue is empty, queue is empty, receiver 2 
	terminates, receiver 1 deallocates queue then terminates.
	
	
	
*/

using namespace std;

const char* ftok_path = ".";
const int ftok_id = 'u';

Sender::Sender(int qidIn)
{
	cout << "qidIn = " << qidIn << endl;
	qid = qidIn;
	marker = numList[assignNumber()];
	MsgPigeon msgr;	// sends and gets values from queue
	
	receiverBit = 11; // for 997 where 1x is r1 and x1 is r2
	
	setMessage("");
	
	srand(time(NULL));
}

int Sender::assignNumber()
{
	cout << "Which number do you want to assign to this sender's marker value?\n";
	
	for (int i = 0; i < numListSize; i++)
	{
		cout << i << ": " << numList[i] << endl;
	}
	
	return getMarkerNumber();
}

int Sender::getMarkerNumber()
{
	int choice;
	
	while(true)
	{
		try
		{
		
			cout << "Enter number: ";
			cin >> choice;
			if (!cin.good() | choice < 0 | choice > numListSize - 1)
				throw int();
			break;
		} 
		catch (int e)
		{
			PTools::flushCin();
			cout << "Invalid selection!" << endl;
		}
	}
	
	return choice;
}

void Sender::setMessage(string msgIn)
{
	strcpy(msgr.message, msgIn.c_str());
}

int Sender::sendMessage(long mTypeIn) // mTypeIn incase need to send other than marker
{
	int result = 0;
	cout << "sendMessage" << endl;
	msgr.mType = mTypeIn;
	
	cout << "qid = " << qid << ", mTypeIn = " << mTypeIn << ", event = " << event << endl;
	
	cout << "msgr.message = " << msgr.message << endl;
	
	cout << "msgr.getSize() = " << msgr.getSize() << endl;
	
	if (strcmp(msgr.message, MSG_TERM.c_str()) == 0)
	{
		terminate();
	}
	
	result = msgsnd(qid, (struct msgbuf*)&msgr, msgr.getSize(), 0);
		
	if (result == -1)
	{
		cout << msgr.message << " did not make it into the msg queue!" << endl;
	}		
	
	// reset message
	strcpy(msgr.message, "");
	msgr.mType = 0;
	
	return result;
}

void Sender::getMessage(long mTypeIn)
{
	cout << "getMessage" << endl;
	
	msgr.mType = mTypeIn;
	
	cout << "qid = " << qid << ", mTypeIn = " << mTypeIn << ", event = " << event << endl;
	
	msgrcv(qid, (struct msgbuf*)&msgr, msgr.getSize(), mTypeIn, 0);
	
	cout << "Event received!" << endl;
	
	cout << "Message = " << msgr.message << endl;
}

bool Sender::terminate() // mType will be for termination calls
{
	cout << "terminate" << endl;
	// sets message to terminate when terminating.
	setMessage(MSG_TERM);
	
	//if 997 notify both receivers
	cout << "marker = " << marker << endl;
	
	if (marker == 997)
	{
	
	cout << "receiverBit = "<< receiverBit << endl;
		if (receiverBit % 10 == 1)
		{	
			receiverBit -= 1;
		}
		if (receiverBit >= 10)
		{
			receiverBit -= 10;
		}
		
		cout << "receiverBit = " << receiverBit << endl;
	}
	
	//if 251 notify receiver 1
	
	//if 257 do not notify any receivers
	
	//somehow exit program
	return true;
}

int Sender::generateRandomNumber() // how to generate 32 bit integers?
{
	event = rand();
	
	//cout << "random number generated = " << event << endl;

	return event;
}

bool Sender::processNumber() // modular event will be 118
{	
	if (event % marker == 0)
	{
		//string eventConv = std::to_string(event);
		strcpy(msgr.message, std::to_string(event).c_str());
		return true;
	}
	
	return false;
}

bool tempConfirmContinue(Sender sendIn)
{
	cout << "tempConfirmContinue" << endl;
	
	char checkQueue; // used to control loop temporarily.

	cout << "qid = " << sendIn.qid << endl;
	
	cout << "Do you want to send to queue again? (y/n): ";
	cin >> checkQueue;
	
	if (checkQueue != 'y' & checkQueue != 'Y')
	{
		return false;
	}
	
	return true;
}

int main()
{		
	Sender sender(msgget(ftok(ftok_path,ftok_id),0));	
	
	/* Testing 251 mType
	sender.msgr.mType = 251;
	strcpy(sender.msgr.message, "TESTING");
	sender.sendMessage(sender.marker); // working
	*/	
	
	/* Testing 251 mType with Termination message
	sender.msgr.mType = 251;
	strcpy(sender.msgr.message, "Terminating");
	sender.sendMessage(sender.marker); // working
	//*/
	
	/* Testing 997 to receiver 1 with acknowledgement
	sender.msgr.mType = 997;
	strcpy(sender.msgr.message, "Hello from 997");
	sender.sendMessage(sender.marker);
	sender.getMessage(sender.marker + 1); // working
	//*/	
	
	/* Testing 997 to receiver 2 with acknowledgement
	sender.msgr.mType = 1097;
	strcpy(sender.msgr.message, "Terminating");
	sender.sendMessage(1097);
	sender.getMessage(sender.marker + 101); // working
	//*/
	
	/* Testing 997 termination to both receivers
	sender.msgr.mType = 997;
	strcpy(sender.msgr.message, "Terminating");
	sender.sendMessage(sender.marker);
	sender.getMessage(sender.marker + 1);
	  
	sender.msgr.mType = 1097;
	strcpy(sender.msgr.message, "Terminating");
	sender.sendMessage(1097);
	sender.getMessage(sender.marker + 101); // working
	//*/
	
	/* Testing 997 to receiver 1 with loop, acknowledgement, and terminate
	int test_loop_counter = 0;
	
	while(true)
	{		
		cout << "test_loop_counter = " << test_loop_counter << endl;
		
		sender.generateRandomNumber();
		
		if (sender.processNumber())
		{
			sender.sendMessage(sender.marker);
			sender.getMessage(sender.marker + 1);
		}
		
		if (test_loop_counter++ == 100000)
		{
			sender.msgr.mType = 997;
			strcpy(sender.msgr.message, "Terminating");
			sender.sendMessage(sender.marker);
			sender.getMessage(sender.marker + 1);
			
			// need final acknowledgement from sender
			// or queue gets closed before sender
			// receives final message from receiver.
			
			sender.msgr.mType = 997;
			strcpy(sender.msgr.message, "Goodbye, #1!");
			sender.sendMessage(sender.marker);
			
			break;		
		}		
	}
	
	// Testing message queue clean up
	strcpy(sender.msgr.message, "junk 1");
	sender.sendMessage(32);
	strcpy(sender.msgr.message, "r2 terminated!");	// r2 termination notification
	sender.sendMessage(2);
	strcpy(sender.msgr.message, "junk 2");
	sender.sendMessage(122);
	strcpy(sender.msgr.message, "junk 3");
	sender.sendMessage(442);
	//*/
	
	//* Testing 997 to receiver 1 with loop, and event generator
	
	// Receiver 2 termination notification
	//strcpy(sender.msgr.message, "r2 terminated!");
	//sender.sendMessage(2);	
	
	// Testing message queue clean up
	strcpy(sender.msgr.message, "junk 1");
	sender.sendMessage(32);
	strcpy(sender.msgr.message, "junk 2");
	sender.sendMessage(122);
	strcpy(sender.msgr.message, "junk 3");
	sender.sendMessage(442); // working
	
	while(sender.receiverBit > 0)
	{
		cout << "(1)receiverBit = " << sender.receiverBit << endl;
		if (sender.receiverBit % 10 == 1)
		{
			sender.setMessage(sender.MSG_ALIVE);
			sender.sendMessage(1096);
		}
		
		sender.generateRandomNumber();
		
		if (sender.event - 1 < sender.EVENT_MIN)
		{
			sender.setMessage(sender.MSG_TERM);
			sender.sendMessage(997);
			
			sender.setMessage(sender.MSG_TERM);
			sender.sendMessage(1097);
		}
		if (sender.processNumber() && sender.receiverBit > 0)
		{
			if (sender.receiverBit >= 10)
			{
				if (sender.sendMessage(997) != -1)
					sender.getMessage(998);
				else
					break; // msg queue missing
			}
			
			if (sender.receiverBit % 10 == 1)
			{
				sender.setMessage(std::to_string(sender.event));
					
				if (sender.sendMessage(1097) != -1)
					sender.getMessage(1098);
				else
					break; // msg queue missing
			}
			
		}
		
		if (sender.receiverBit % 10 == 1) // if r2 active
		{
			cout << "Polling now" << endl;
			sender.getMessage(1096);
			cout << "message = " << sender.msgr.message << endl;
			
			if (strcmp(sender.msgr.message, sender.MSG_TERM.c_str()) == 0) // if message r2 term
			{
				sender.receiverBit -= 1;
				cout << "(2)receiverBit = " << sender.receiverBit << endl;
			}
		} 
	}	
	//*/	
	
	/* Testing 997 events to termination loop to both receivers
	int test_loop_counter = 0;
	
	while(true)
	{				
		sender.generateRandomNumber();
		
		if (sender.processNumber())
		{
			sender.sendMessage(sender.marker);
			sender.getMessage(sender.marker + 1);
			
			sender.sendMessage(sender.marker + 100);
			sender.getMessage(sender.marker + 101);
		}
		
		if (test_loop_counter++ == 100000)
		{
			sender.msgr.mType = 997;
			strcpy(sender.msgr.message, "Terminating");
			sender.sendMessage(sender.marker);
			sender.getMessage(sender.marker + 1);
			  
			sender.msgr.mType = 1097;
			strcpy(sender.msgr.message, "Terminating");
			sender.sendMessage(1097);
			sender.getMessage(sender.marker + 101);
			
			// need final acknowledgement from sender
			// or queue gets closed before sender
			// receives final message from receiver.
			
			sender.msgr.mType = 997;
			strcpy(sender.msgr.message, "Goodbye, #1!");
			sender.sendMessage(sender.marker);
			  
			sender.msgr.mType = 1097;
			strcpy(sender.msgr.message, "Goodbye, #2!");
			sender.sendMessage(1097);
			
			break;
			
		}
	} // working
	//////////////////////////*/
	
	/* Test receiver 2 termination with 997
	while(true)
	{				
		sender.generateRandomNumber();
		
		if (sender.processNumber())
		{
			sender.sendMessage(sender.marker);
			sender.getMessage(sender.marker + 1);
			
			sender.sendMessage(sender.marker + 100);
			sender.getMessage(sender.marker + 101);
		}
		
		if ()
		{
			sender.msgr.mType = 997;
			strcpy(sender.msgr.message, "Terminating");
			sender.sendMessage(sender.marker);
			sender.getMessage(sender.marker + 1);
			  
			sender.msgr.mType = 1097;
			strcpy(sender.msgr.message, "Terminating");
			sender.sendMessage(1097);
			sender.getMessage(sender.marker + 101);
			
			// need final acknowledgement from sender
			// or queue gets closed before sender
			// receives final message from receiver.
			
			sender.msgr.mType = 997;
			strcpy(sender.msgr.message, "Goodbye, #1!");
			sender.sendMessage(sender.marker);
			  
			sender.msgr.mType = 1097;
			strcpy(sender.msgr.message, "Goodbye, #2!");
			sender.sendMessage(1097);
			
			break;
			
		}
	}
	//*/
	
	/* Testing mType < 0
	sender.msgr.mType = 997;
	strcpy(sender.msgr.message, "TESTING");
	sender.sendMessage(sender.marker);
	sender.getMessage(-10);
	//*/
		
	return 0;
}
