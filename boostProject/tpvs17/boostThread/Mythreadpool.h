#ifndef _MYTHREADPOOL_H_
#define _MYTHREADPOOL_H_
#include <iostream>
#include <queue>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
using namespace boost;
typedef boost::function<void(void)> MyTask;

class MyTaskQueue : boost::noncopyable
{
private:
	std::queue<MyTask> m_taskQueue;
	boost::mutex m_mutex;//互斥锁
	boost::condition_variable_any m_cond;//条件变量
public:
	void push_Task(const MyTask& task) {
		//加上互斥锁
		boost::unique_lock<boost::mutex> lock(m_mutex);
		m_taskQueue.push(task);
		//通知其他线程启动
		m_cond.notify_one();
	}

	MyTask pop_Task() {
		//加上互斥锁
		boost::unique_lock<boost::mutex> lock(m_mutex);
		if (m_taskQueue.empty())
		{
			//如果队列中没有任务，则等待互斥锁 
			m_cond.wait(lock);//
		}
		//指向队列首部
		MyTask task(m_taskQueue.front());
		//出队列  
		m_taskQueue.pop();
		return task;
	}
	int get_size()
	{
		return m_taskQueue.size();
	}
};
class MyThreadPool : boost::noncopyable
{
private:
	//任务队列
	MyTaskQueue m_taskQueue;
	//线程组 
	boost::thread_group m_threadGroup;
	int m_threadNum;
	/*
	volatile 被设计用来修饰被不同线程访问和修改的变量。
	volatile 告诉编译器i是随时可能发生变化的，每次使用它的时候必须从i的地址中读取，
	因而编译器生成的可执行码会重新从i的地址读取数据放在k中。
	volatile可以保证对特殊地址的稳定访问，不会出错。
	*/
	volatile bool is_run;
	void run() {//线程池中线程的处理函数
		while (is_run) {
			//一直处理线程池的任务
			MyTask task = m_taskQueue.pop_Task();
			task();//运行bind的函数
		}
	}
public:
	MyThreadPool(int num) :m_threadNum(num), is_run(false)//初始化列表
	{

	}
	~MyThreadPool() {
		stop();
	}
	void init()
	{
		if (m_threadNum <= 0) return;
		is_run = true;
		for (int i = 0; i < m_threadNum; i++)
		{
			//生成多个线程，绑定run函数，添加到线程组
			m_threadGroup.add_thread(
				new boost::thread(boost::bind(&MyThreadPool::run, this)));
		}
	}
	//停止线程池
	void stop()
	{
		is_run = false;
	}
	//添加任务
	void AddNewTask(const MyTask& task) {
		m_taskQueue.push_Task(task);
	}
	void wait()
	{
		m_threadGroup.join_all();//等待线程池处理完成！
	}
};

#endif