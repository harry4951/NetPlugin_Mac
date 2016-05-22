#ifndef __READNETWORKDISPLAY_H__
#define __READNETWORKDISPLAY_H__
// ... header code
#include "../JuceLibraryCode/JuceHeader.h"
#include "QueueMutexUdp.h"

using namespace std;
#define NUM_SAMPLES 1024
#define BUFFER_SIZE 32
#define NUM_BUFFER_PACKETS 4

typedef std::chrono::high_resolution_clock hr_clock;
typedef std::chrono::time_point<hr_clock> hr_time_point;

typedef struct NetworkBuffer
{
	int packet_id;//big packet id, which will be split into NUM_BUFFER_PACKETS small packet to be sent via network
	int packet_id_num; //the id of small packet
	float buffer[BUFFER_SIZE];
} NetworkBuffer;

class ReadNetwork: public Thread
{
public:
	/*ReadNetwork(QueueMutexUdp<float>& buffer)
		: queue_(buffer),_count(0),_is_timer_running(false),_is_current_packet(true),PacketArrivalIntervalLimit(150) //_timer_start_instant(0),
	{
	}*/
    
    ReadNetwork(QueueMutexUdp<float>& buffer);

    ~ReadNetwork();
    
	void start_sock();

	void run();

	void FindMissingOne(int PacketIdFlag[],int PacketRecvNum);

	void FillLostPacket(int srcstart, int srcend, int deststart, int sizesubpacket, float buffer[]);

	int getTimerInterval();

	void stopTimer();

private:
	QueueMutexUdp<float>&  queue_;
	DatagramSocket sock;

	float Buff_packets[BUFFER_SIZE * NUM_BUFFER_PACKETS];
	NetworkBuffer Buff[BUFFER_SIZE];
	int si_port = 8888;
	int MaxreadByte = BUFFER_SIZE * sizeof(float);
	String si_other;
    int _count;
	bool _is_timer_running;
	//bool _is_timer_expired;
	bool _is_current_packet;
    int CurrentPacketID;
    int ExpiredPacketID;
    int packet_count=0;
    hr_time_point _timer_start_instant;
    std::chrono::milliseconds PacketArrivalIntervalLimit;
    //std::atomic<int> packet_count=0;
};


class Display
{
public:

	Display(QueueMutexUdp<float>& buffer)
		: queue_(buffer)
	{}

	void run();
private:
	QueueMutexUdp<float>&  queue_;
};

#endif // __READNETWORKDISPLAY_H__
