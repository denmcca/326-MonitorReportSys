#include "MsgPigeon.cpp"

class Sender
{
	public:
	int qid;	// id for queue.
	int marker;	// holds modulus value for events
	static const int numListSize = 3;	// size of list that holds marker values
	int numList[numListSize] = {251, 257, 997};	// marker values
	MsgPigeon msgr;	// Sends and gets values from queue
	int event;	// Holds last event integer generated
	char eventMsg[50];	// Holds event message to be passed to queue
	
	Sender(int);	// Constructor
	int assignNumber();	// prompts user to select from available marker values
	int getMarkerNumber(); // prompts user to select value from marker list
	void sendEvent(long); // sends event
	bool terminate();
	int generateRandomNumber();
	bool processNumber();	// Checks if previous event is modular to marker.
};


