#ifndef _MWORKTHREAD_H_
#define _MWORKTHREAD_H_
#include "basedefine.h"
#include <mutex>
#include <thread>

class MWorkThread
{
public:
	MWorkThread();
	~MWorkThread();
	void CallBackFunc(Task func) {
		_func = func;
	}
	void init();
	void onTrigeOnce();
	void onStartStream();
	void onStopStream();
	void setSleepMs(int times) { _sleepms = times; };
protected:
	void run();
private:
	int _sleepms;
	volatile bool is_run;
	int m_FPS;
	Task _func;
	bool _bStream;
	mutable std::mutex m;

};


#endif
