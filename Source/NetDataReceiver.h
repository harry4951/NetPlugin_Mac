#ifndef __NETDATARECEIVER_H__
#define __NETDATARECEIVER_H__
// ... header code
#include "../JuceLibraryCode/JuceHeader.h"
#include "SwapBuffer.h"

typedef std::chrono::high_resolution_clock hr_clock;
typedef std::chrono::time_point<hr_clock> hr_time_point;

//Currently has to be consistent with the sender side packet
typedef struct NetworkBuffer
{
    //big packet id, which will be split into NUM_BUFFER_PACKETS small packet to be sent via network
	int packet_id;
    //the id of small packet
	int packet_id_num;
	float buffer[SwapBuffer::NETWORK_PACKET_SIZE];
} NetworkBuffer;

class ReadNetwork: public Thread, public SwapBuffer
{
public:

    ReadNetwork(SwapBuffer& buffer);

    ~ReadNetwork();
    
	void StartSocket();

	void run();

private:
	
    SwapBuffer& buffer_;
	DatagramSocket sock_;
    String si_other_;
    NetworkBuffer network_buff_;
    NetworkBuffer buff_[SwapBuffer::NETWORK_PACKET_SIZE];

    int si_port_ = 8888;
    int packet_count_;
	float buff_packets[SwapBuffer::AUDIO_BUFFER_SIZE];
    static const auto NETWORK_TIMEOUT_MS = 10;
};


#endif 
