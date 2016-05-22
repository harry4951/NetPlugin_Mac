
#include "QueueMutexUdp.h"
#include "ReadNetworkDisplay.h"
#include <thread>
using namespace std;

int main(int argc, char* argv[])
{
	QueueMutexUdp<float> queue;
	ReadNetwork read(queue);
	Display show(queue);

	read.start_sock();

	thread ReadNetwork_thread(&ReadNetwork::run, &read);
	thread Display_thread(&Display::run, &show);

	ReadNetwork_thread.join();
	Display_thread.join();



	return 0;
}
