#include "MWorkThread.h"

MWorkThread::MWorkThread() :is_run(true), _bStream(false)
{
	_sleepms = 100;
}

MWorkThread::~MWorkThread()
{
}

void MWorkThread::init()
{
	std::thread thred(&MWorkThread::run, this);
	thred.detach();
}

void MWorkThread::onTrigeOnce()
{
	std::lock_guard<std::mutex> guard(m);
	if (_bStream == true)
		_bStream = false;
	_func();
}

void MWorkThread::onStartStream()
{
	_bStream = true;
}

void MWorkThread::onStopStream()
{
	_bStream = false;
}

void MWorkThread::run()
{
	while (is_run) {
		if (_bStream)
		{
			std::lock_guard<std::mutex> guard(m);
			//std::cout<<"run func"<<std::endl;
			_func();
			// std::cout<<"run func end"<<std::endl;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(_sleepms));
	}
}
