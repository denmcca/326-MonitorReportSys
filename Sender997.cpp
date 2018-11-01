#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include "MsgPigeon.cpp"
#include <string>
#include <sstream>

using namespace std;

const int ID = 997;
<<<<<<< HEAD
const int ACK_ID_R1 = 3;
const int ACK_ID_R2 = 30;
const int R1_ID = 2;
=======
const int ACK_ID_R1 = 11;
const int ACK_ID_R2 = 21;
const int R1_ID = 8;
>>>>>>> aee4432ff4590b2afd06e8357541868a230bbc42
const int R2_ID = 20;

// Converts a int to a string
string intToString (int a)
{
	ostringstream temp;
	temp << a;
	return temp.str();
}

class Sender997
{
public:
	int qid;
	bool sendToR1;
<<<<<<< HEAD
	bool sendToR2; // initialized to "true", set to "false" after R2 terminates.

<<<<<<< HEAD
=======
=======
	bool sendToR2;
	
>>>>>>> 727baec07174e375c954db5050d8989c4522b7e4
>>>>>>> aee4432ff4590b2afd06e8357541868a230bbc42
	Sender997();
	void initQID();
	int generateRandomNumber();
	void sendMessage(string msgContent, long mType);
	string getMessage(long mType);
	void runMainLoop();
};

Sender997::Sender997()
{
	qid = -1;
	sendToR1 = true;
	sendToR2 = true;
	srand(time(NULL));
}

// Generate a random 32-bit int
int Sender997::generateRandomNumber()
{
	return rand();
}

// Sends a message to the queue with the given content and mtype.
void Sender997::sendMessage(string msgContent, long mType)
{
	cout << "Sending '" << msgContent << "' to Receiver ";
	if (mType == R1_ID) cout << "1\n";
	else cout << "2\n";

	MsgPigeon msg;
	msg.mType = mType;
	msg.message.srcID = ID;
	strcpy(msg.message.message, msgContent.c_str());
	if(msgsnd(qid, (struct MsgPigeon *)&msg, msg.getSize(), 0) < 0)
		throw int(-10);
}

<<<<<<< HEAD
int ACK_COUNT_R1 = 0;
int ACK_COUNT_R2 = 0;
=======
<<<<<<< HEAD
int ACK_COUNT_R1 = 0;
int ACK_COUNT_R2 = 0;
=======
// Receives a message from the queue with the given mtype.
// Returns the message.
>>>>>>> 727baec07174e375c954db5050d8989c4522b7e4
>>>>>>> aee4432ff4590b2afd06e8357541868a230bbc42
string Sender997::getMessage(long mType)
{

	////////////////////////////if (mType == ACK_ID_R1) cout << "Getting ACK #" << ++ACK_COUNT_R1 << " from " << mType << endl << flush;
	////////////////////////////////else cout << "Getting ACK #" << ++ACK_COUNT_R2 << " from " << mType << endl << flush;
	MsgPigeon msg;
	if (msgrcv(qid, (struct msgbuf*)&msg, msg.getSize(), mType, 0) < 0) throw int(-11);
	cout << "Reply received from Receiver " << msg.message.srcID << ": ";
	cout << msg.message.message << endl;
	return msg.message.message;
}

// Initilize the qid for Sender997.
void Sender997::initQID()
{
	const char* ftok_path = ".";
	const int ftok_id = 'u';
	cout << "Waiting for message queue..";
	while (true)
	{
		//cout << "." << flush; sleep(1); // Causes
		if (qid == -1)	qid = msgget(ftok(ftok_path,ftok_id),0);
		else
		{
			cout << "997 found the queue! Now ready! " << endl;
			break;
		}
	}
}

// Run the main loop of Sender 997.
// Generate random numbers and send events to R1 and R2.
void Sender997::runMainLoop()
{
	MsgPigeon msg;
	msg.mType = R2_ID;
	strcpy(msg.message.message, "Sender 997 Ready");
	msg.message.srcID = ID;
	msgsnd(qid, (struct msgbuf *)&msg, msg.getSize(), 0); // sending init call to receiver
	msgrcv(qid, (struct msgbuf *)&msg, msg.getSize(), ID, 0); // Starting message

	while (sendToR1 | sendToR2)
	{
		// Generate and process random number
		int randInt = generateRandomNumber();
<<<<<<< HEAD
		if (randInt < 5)
=======
		if (randInt < 10)
>>>>>>> aee4432ff4590b2afd06e8357541868a230bbc42
		{
			sendMessage("Terminating", R1_ID);
			if (sendToR2) sendMessage("Terminating", R2_ID);
			cout << "Terminated with " << randInt << '!' << endl;
			sendToR1 = false;
			sendToR2 = false;
		}
		else if ((randInt % 997) == 0)
		{
<<<<<<< HEAD
=======
			//////////////////////////////////////sleep(1);
>>>>>>> aee4432ff4590b2afd06e8357541868a230bbc42
			string msgContent = intToString(randInt);
			sendMessage(msgContent, R1_ID);
			// Get ackowledgement from R1
			/////////////////////////////////////////cout << "Awaiting acknowledgement." << endl << endl;
			if (strcmp(getMessage(ACK_ID_R1).c_str(), "Terminating") == 0)
			{
				cout << "Received R1 Terminating message" << endl;
				sendToR1 = false;
				sendMessage("Terminating", R1_ID);
			}

			if (sendToR2)
			{
<<<<<<< HEAD
				// Send event to R2
				sendMessage(msgContent, R2_ID);

=======
<<<<<<< HEAD
				// Send event to R2
				sendMessage(msgContent, R2_ID);

=======
				sendMessage(msgContent, R2_ID);
				// Get ackowledgement from R2
>>>>>>> 727baec07174e375c954db5050d8989c4522b7e4
>>>>>>> aee4432ff4590b2afd06e8357541868a230bbc42
				if (strcmp(getMessage(ACK_ID_R2).c_str(), "Terminating") == 0)
				{
					cout << "Received R2 Terminating message" << endl;
					sendToR2 = false;
					sendMessage("Terminating", R2_ID);
				}
			}
		}
	}
}

int main()
{
	Sender997 snd;
	snd.initQID();
<<<<<<< HEAD
	cout << "qid == " << snd.qid << endl << flush;
=======
>>>>>>> aee4432ff4590b2afd06e8357541868a230bbc42

	try
	{
		snd.runMainLoop();
	}
	catch (int err)
	{
		if (err == -10)
			cout << "sendMessage error!" << endl << flush;
		if (err == -11)
			cout << "getMessage error!" << endl << flush;
<<<<<<< HEAD
		return -1;
=======
>>>>>>> aee4432ff4590b2afd06e8357541868a230bbc42
	}
	cout << "Sender 997 has finished!\n";
}
