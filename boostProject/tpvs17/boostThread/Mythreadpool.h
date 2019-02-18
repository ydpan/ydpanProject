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
	boost::mutex m_mutex;//������
	boost::condition_variable_any m_cond;//��������
public:
	void push_Task(const MyTask& task) {
		//���ϻ�����
		boost::unique_lock<boost::mutex> lock(m_mutex);
		m_taskQueue.push(task);
		//֪ͨ�����߳�����
		m_cond.notify_one();
	}

	MyTask pop_Task() {
		//���ϻ�����
		boost::unique_lock<boost::mutex> lock(m_mutex);
		if (m_taskQueue.empty())
		{
			//���������û��������ȴ������� 
			m_cond.wait(lock);//
		}
		//ָ������ײ�
		MyTask task(m_taskQueue.front());
		//������  
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
	//�������
	MyTaskQueue m_taskQueue;
	//�߳��� 
	boost::thread_group m_threadGroup;
	int m_threadNum;
	/*
	volatile ������������α���ͬ�̷߳��ʺ��޸ĵı�����
	volatile ���߱�����i����ʱ���ܷ����仯�ģ�ÿ��ʹ������ʱ������i�ĵ�ַ�ж�ȡ��
	������������ɵĿ�ִ��������´�i�ĵ�ַ��ȡ���ݷ���k�С�
	volatile���Ա�֤�������ַ���ȶ����ʣ��������
	*/
	volatile bool is_run;
	void run() {//�̳߳����̵߳Ĵ�����
		while (is_run) {
			//һֱ�����̳߳ص�����
			MyTask task = m_taskQueue.pop_Task();
			task();//����bind�ĺ���
		}
	}
public:
	MyThreadPool(int num) :m_threadNum(num), is_run(false)//��ʼ���б�
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
			//���ɶ���̣߳���run��������ӵ��߳���
			m_threadGroup.add_thread(
				new boost::thread(boost::bind(&MyThreadPool::run, this)));
		}
	}
	//ֹͣ�̳߳�
	void stop()
	{
		is_run = false;
	}
	//�������
	void AddNewTask(const MyTask& task) {
		m_taskQueue.push_Task(task);
	}
	void wait()
	{
		m_threadGroup.join_all();//�ȴ��̳߳ش�����ɣ�
	}
};

#endif