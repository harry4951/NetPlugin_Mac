#include "NetDataReceiver.h"
#include "SwapBuffer.h"
#include <chrono>
#include <thread>
#include <vector>
#include <numeric>
#include <algorithm>



ReadNetwork::ReadNetwork(SwapBuffer& buffer)
: Thread ("NetworkThread"), buffer_(buffer), packet_count_(0)
{
    StartSocket();
    
}

ReadNetwork::~ReadNetwork()
{
    stopThread(3000);
}


void ReadNetwork::StartSocket()
{
	if (!sock_.bindToPort(si_port_))
	{
		printf("Error: could not connect to UDP port 8888.\n");
	}
    
    startThread();
}


void ReadNetwork::run()
{
	while (true)
	{
	    //wait for 10ms if no data is ready
       	if (sock_.waitUntilReady(true, NETWORK_TIMEOUT_MS) == 1)
		{
            if(packet_count_ < SwapBuffer::NETWORK_PACKET_SIZE )
			{
                //read the data into buffer, true -> suppose that the packet is complete.
                sock_.read(static_cast<void*>(&network_buff_), sizeof(network_buff_), true, si_other_, si_port_);
                
                //set the filled packet number to 1
                buffer_.MarkFilledPacket(network_buff_.packet_id_num);
                
                //cooy the data to swapBuffer
                std::copy(network_buff_.buffer, network_buff_.buffer + SwapBuffer::NETWORK_PACKET_SIZE, buffer_.GetTailBuffer()+ SwapBuffer::NETWORK_PACKET_SIZE * network_buff_.packet_id_num);
                
            }
        }
    }
}

