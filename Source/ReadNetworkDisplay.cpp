#include "ReadNetworkDisplay.h"
#include "QueueMutexUdp.h"
#include <chrono>
#include <thread>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

static const auto NETWORK_TIMEOUT_MS = 10;

ReadNetwork::ReadNetwork(swapBuffer& buffer)
: Thread ("networkThread"), _buffer(buffer),_count(0),_is_timer_running(false),_is_current_packet(true),PacketArrivalIntervalLimit(150)//queue_(buffer)
{
    start_sock();
    
}

ReadNetwork::~ReadNetwork()
{
    stopThread(3000);
}


void ReadNetwork::start_sock()
{
	if (!sock.bindToPort(si_port))
	{
		printf("Error: could not connect to UDP port 8888.\n");
	}
    startThread();
}

NetworkBuffer NetworkBuff;
void ReadNetwork::run()
{
	//int PacketArrivalIntervalLimit = (int)((double)BUFFER_SIZE * (double)NUM_BUFFER_PACKETS / (double)NUM_SAMPLES *1000.0)-1;

    //int PacketIdFlag[NUM_BUFFER_PACKETS]={0};

	while (true)
	{
	    //wait for 10ms if no data is ready
       	if (sock.waitUntilReady(true, NETWORK_TIMEOUT_MS) == 1)
		{
			if(packet_count < NUM_BUFFER_PACKETS )
			{
                //read the data into buffer
                sock.read(static_cast<void*>(&NetworkBuff), sizeof(NetworkBuff), true, si_other, si_port);// true -> suppose that the packet is complete.
                
                //set the filled packet number to 1
                _buffer.mark_filled_packet(NetworkBuff.packet_id_num);
                
                //cooy the data to swapBuffer
                copy(NetworkBuff.buffer, NetworkBuff.buffer + BUFFER_SIZE, _buffer.get_tail_buffer()+ BUFFER_SIZE * NetworkBuff.packet_id_num);
/*
                if ((packet_count ==0) && (ExpiredPacketID != NetworkBuff.packet_id) && (!_is_timer_running))
                {
                    CurrentPacketID = NetworkBuff.packet_id;
                    //start timer when the first small packet arrives
                    _timer_start_instant = hr_clock::now();
                    _is_timer_running = true;
                    //Reset PacketIdFlag to 0
                    //memset(PacketIdFlag, 0, sizeof(PacketIdFlag)*NUM_BUFFER_PACKETS);
                }
*/
              //  if(ExpiredPacketID != NetworkBuff.packet_id && (_is_timer_running))
              //  {
                    //store the small id_num who has been read
              //      PacketIdFlag[NetworkBuff.packet_id_num]= 1;
                    //save the buffer data into big buffer
                    //copy(NetworkBuff.buffer, NetworkBuff.buffer + BUFFER_SIZE, Buff_packets + BUFFER_SIZE * NetworkBuff.packet_id_num);
                    //packet_count++;

/*
                    if (packet_count == NUM_BUFFER_PACKETS)
                    {
                        //have received all the packet before the timer is due, destroy the timer
                        _is_timer_running = false;
                        //push the data to the queue to display
                        for (int i = 0; i < BUFFER_SIZE * NUM_BUFFER_PACKETS; i++)
                        {
                            //copy(Buff_packets[0], Buff_packets[BUFFER_SIZE * NUM_BUFFER_PACKETS-1],_buffer.get_head_buffer());
                            //copy(Buff_packets[0], Buff_packets[BUFFER_SIZE * NUM_BUFFER_PACKETS-1],Buff_packets[0]);
                        }
                        packet_count = 0;
                    }
 */
                }
			}

		}

/*		if (_is_timer_running)
        {
            auto time_elapsed = hr_clock::now() - _timer_start_instant;
            if (time_elapsed > PacketArrivalIntervalLimit)
            {
                ExpiredPacketID=CurrentPacketID;
                //fill what's missing in Buff_packets
                FindMissingOne(PacketIdFlag, packet_count);
                _is_timer_running = false;
                packet_count = 0;
            }
        }*/
    //}
}

/*void ReadNetwork::FillLostPacket(int srcstart, int srcend, int deststart, int sizesubpacket, float buffer[])
{
    copy(buffer + srcstart * sizesubpacket, buffer + srcend * sizesubpacket, buffer + deststart * sizesubpacket);
}

void ReadNetwork::FindMissingOne(int PacketIdFlag[], int PacketRecvNum)
{
    for(int i = 0; i < NUM_BUFFER_PACKETS; i++)

    {
        if(PacketIdFlag[i]==1)
        {
            printf("The small packet id is %d \n", i);
        }
        else
        {
            //std::fill(Buff_packets+i * BUFFER_SIZE, Buff_packets+(i+1)* BUFFER_SIZE, 0.0);
            if(i==0)
            {
                //FillLostPacket(i+1, i+2, i, BUFFER_SIZE, Buff_packets);
                copy(Buff_packets+(i+1) * BUFFER_SIZE, Buff_packets+(i+2) * BUFFER_SIZE, Buff_packets+i * BUFFER_SIZE);
            }
            else if(i==(NUM_BUFFER_PACKETS-1))
            {
                //FillLostPacket(i-1, i, i, BUFFER_SIZE, Buff_packets);
                copy(Buff_packets+(i-1) * BUFFER_SIZE, Buff_packets+ i * BUFFER_SIZE, Buff_packets+i * BUFFER_SIZE);
            }
            else
            {
                //FillLostPacket(i-1, i+2, i, BUFFER_SIZE, Buff_packets);
                copy(Buff_packets+(i-1) * BUFFER_SIZE + BUFFER_SIZE/2, Buff_packets+ i * BUFFER_SIZE, Buff_packets+i * BUFFER_SIZE);
                copy(Buff_packets+(i+1) * BUFFER_SIZE, Buff_packets+ (i+1) * BUFFER_SIZE + BUFFER_SIZE/2, Buff_packets+i * BUFFER_SIZE + BUFFER_SIZE/2);
            }
        }
    }


    for (int i = 0; i < BUFFER_SIZE * NUM_BUFFER_PACKETS; i++)
    {
        queue_.push(Buff_packets[i]);
    }

}
*/

