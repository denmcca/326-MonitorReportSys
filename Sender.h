#include "MsgPigeon.cpp"
#include <string>

using namespace std;

class Sender
{
	public:
	int qid;	// id for queue.
	int marker;	// holds modulus value for events
	static const int numListSize = 3;	// size of list that holds marker values
	int numList[numListSize] = {251, 257, 997};	// marker values
	MsgPigeon msgr;	// Sends and gets values from queue
	int event;	// Holds last event integer generated
	char eventMsg;	// Holds event message to be passed to queue
	
	int receiverBit; // right bit = receiver 1, left bit = receiver 2 // fix location later.
	
	const int EVENT_MIN = 100000;	
	const string MSG_TERM = "Terminating";
	const string MSG_ALIVE = "I am alive!";
	
	Sender(int);	// Constructor
	int assignNumber();	// prompts user to select from available marker values
	int getMarkerNumber(); // prompts user to select value from marker list
	void setMessage(string);
	int sendMessage(long); // sends event
	void getMessage(long);	// gets event (acknowledgements, termination messages)
	bool terminate();
	int generateRandomNumber();
	bool processNumber();	// Checks if previous event is modular to marker.
};

