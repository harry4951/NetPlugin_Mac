#ifndef __SWAPBUFFER_H__
#define __SWAPBUFFER_H__
// ... header code
#include "../JuceLibraryCode/JuceHeader.h"
#include <mutex>
#include <queue>
#include <condition_variable>

class SwapBuffer
{
public:
    SwapBuffer():
    head_(0)
    {
        //Clear all the buffers
        memset(buffers_for_output_,0,sizeof(buffers_for_output_));
        memset(packets_filled_flag_,0,sizeof(packets_filled_flag_));
    }
    
    void switch_buffers()
    {
        
        // Switch head and tail, tail for receiving from network, head for output
        head_ = 1 - head_;
        
        // All the packets in the new tail buffer are now empty,ready for receive new data from network
        memset(&buffers_for_output_[1 - head_][0], 0, sizeof(float) * (AUDIO_BUFFER_SIZE / NETWORK_PACKET_SIZE));
        // Fill in the missing holes of new head buffer
        for(int i=0; i < (AUDIO_BUFFER_SIZE / NETWORK_PACKET_SIZE); i++)
        {
            if (!(packets_filled_flag_[head_][i]))
            {
                // fill the samples inside _buffers[_head][NETWORK_PACKET_SIZE*i:NETWORK_PACKET_SIZE*(i+1)]
                //std::copy(_buffers[_head][i * NETWORK_PACKET_SIZE], _buffers[_head][(i+1) * NETWORK_PACKET_SIZE-1], _buffers[_head][i * NETWORK_PACKET_SIZE]);
                //memset(_buffers[_head][i * NETWORK_PACKET_SIZE],0.0,sizeof(float) * NETWORK_PACKET_SIZE);
                memset(&buffers_for_output_[head_][1], 0, sizeof(float) * AUDIO_BUFFER_SIZE);
            }
        }
    }
    
    //For audio card
    inline float* GetHeadBuffer()
    {
        return buffers_for_output_[head_];
    }
    
    //For network
    inline float* GetTailBuffer()
    {
        return buffers_for_output_[1 - head_];
    }
    
    //Make 1 when certain packet arrives
    inline void MarkFilledPacket(int packet_num)
    {
        packets_filled_flag_[1 - head_][packet_num] = 1;
    }
    
    static const int AUDIO_BUFFER_SIZE = 1024;
    static const int NETWORK_PACKET_SIZE = 64;
    static const int NET_PACKETS_NUM = AUDIO_BUFFER_SIZE / NETWORK_PACKET_SIZE;
private:
    std::atomic<int> head_;
    float buffers_for_output_[2][AUDIO_BUFFER_SIZE];
    int packets_filled_flag_[2][AUDIO_BUFFER_SIZE / NETWORK_PACKET_SIZE];

};
#endif 
