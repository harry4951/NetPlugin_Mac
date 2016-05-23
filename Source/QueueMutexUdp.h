#ifndef __QUEUEMUTEXUDP_H__
#define __QUEUEMUTEXUDP_H__
// ... header code

#include "../JuceLibraryCode/JuceHeader.h"
#include <mutex>
#include <queue>
#include <condition_variable>

/*
	template <typename T>

	class QueueMutexUdp
	{
	public:

		T pop()
		{
            std::unique_lock<std::mutex> mlock(mutex_);
			while (queue_.empty())
			{
				cond_.wait(mlock);
			}
			auto item = queue_.front();
			queue_.pop();
			return item;
		}

		//void pop(T& item);

		void push(const T& item)
		{
			std::unique_lock<std::mutex> mlock(mutex_);
			queue_.push(item);
			mlock.unlock();
			cond_.notify_one();
		}

	private:
		std::queue<T> queue_;
		std::mutex mutex_;
		std::condition_variable cond_;
	};
*/

static const int AUDIO_BUFFER_SIZE = 1024;
static const int NETWORK_PACKET_SIZE = 64;

class swapBuffer
{
public:
    swapBuffer():
    _head(0)
    {
        memset(_buffers,0,sizeof(_buffers));
        memset(_packets_filled,0,sizeof(_packets_filled));
    }
    
    void switch_buffers()
    {
        //_head =0;
        // Switch head and tail, tail for receiving from network, head for output
        _head = 1 - _head;
        // All the packets in the new tail buffer are now empty
        memset(&_packets_filled[1 - _head][0], 0, sizeof(float) * (AUDIO_BUFFER_SIZE / NETWORK_PACKET_SIZE));
        // Fill the missing holes inside new head buffer
        for(int i=0; i < (AUDIO_BUFFER_SIZE / NETWORK_PACKET_SIZE); i++)
        {
            if (!(_packets_filled[_head][i]))
            {
                // fill the samples inside _buffers[_head][NETWORK_PACKET_SIZE*i:NETWORK_PACKET_SIZE*(i+1)]
                //std::copy(_buffers[_head][i * NETWORK_PACKET_SIZE], _buffers[_head][(i+1) * NETWORK_PACKET_SIZE-1], _buffers[_head][i * NETWORK_PACKET_SIZE]);
                //memset(_buffers[_head][i * NETWORK_PACKET_SIZE],0.0,sizeof(float) * NETWORK_PACKET_SIZE);
            }
        }
    }
    
    float* get_head_buffer()
    {
        return _buffers[_head];
    }
    
    float* get_tail_buffer()
    {
        return _buffers[1 - _head];
    }
    
    void mark_filled_packet(int packetNum)
    {
        _packets_filled[1 - _head][packetNum] = 1;
    }
    
    
private:
    std::atomic<int> _head;
    //int _head;
    float _buffers[2][AUDIO_BUFFER_SIZE];
    int _packets_filled[2][AUDIO_BUFFER_SIZE / NETWORK_PACKET_SIZE];
};
//#include "QueueMutexUdp.cpp"
#endif // __QUEUEMUTEXUDP_H__
