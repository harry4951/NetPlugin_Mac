#ifndef __QUEUEMUTEXUDP_H__
#define __QUEUEMUTEXUDP_H__
// ... header code

#include "../JuceLibraryCode/JuceHeader.h"
#include <mutex>
#include <queue>
#include <condition_variable>
using namespace std;

	template <typename T>

	class QueueMutexUdp
	{
	public:

		T pop()
		{
			unique_lock<mutex> mlock(mutex_);
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
			unique_lock<mutex> mlock(mutex_);
			queue_.push(item);
			mlock.unlock();
			cond_.notify_one();
		}

	private:
		queue<T> queue_;
		mutex mutex_;
		condition_variable cond_;
	};

//#include "QueueMutexUdp.cpp"
#endif // __QUEUEMUTEXUDP_H__
