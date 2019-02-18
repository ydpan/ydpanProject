#include <QtCore/QCoreApplication>
#include <iostream>
#include "Mythreadpool.h"
#include <boost/signals2.hpp>
#include <boost/bind.hpp>

using namespace boost::signals2;
using namespace std;
typedef void(*pFunCallBack)(int i);
void CallBackFun(int i)
{
	std::cout << i << " call back!" << std::endl;
}

void ProcFun(int ti, pFunCallBack callback)
{
	std::cout << "I am Task " << ti << std::endl;
	//task
	for (int i = 0; i < ti * 100000000; i++)
	{
		i*i;
	}
	if (callback != NULL)callback(ti);
}


void CallBackFun2(int i)
{
	std::cout << i << " call back! v2" << std::endl;
}

int ProcFun2(int& ti)
{
	std::cout << "I am Task " << ti << std::endl;
	//task
	for (int i = 0; i < ti * 100000000; i++)
	{
		i*i;
	}
	return ti;
}
void slots1()
{
	cout << "slots1 called" << endl;
}

void slots2()
{
	cout << "slots2 called" << endl;
}
class A
{
public:
	static int staticMemberFunc(int param)
	{
		cout << "A::staticMemberFunc called, param: " << param << endl;
		return 0;
	}
	int memberFunc(int param)
	{
		cout << "A::memberFunc called, param: " << param << endl;
		return 0;
	}
};
void testThreadPool()
{
	

	MyThreadPool tp(2);
	int taskNum = 4;
	for (int i = 0; i < taskNum; i++)
	{
		MyTask task = boost::bind(ProcFun, i + 1, CallBackFun);
		//�ŵ��̳߳��д���bind(f , i) will produce a "nullary" function object that takes no arguments and returns f(i),����ʱ���ɴ����κ����͵ĺ���������������
		tp.AddNewTask(task);
	}

	tp.init();
	//�ȴ��̳߳ش�����ɣ�  



	boost::signals2::signal<void()> sig;
	boost::signals2::signal<int(int)> sig2;
	A a1;
	connection c1 = sig.connect(&slots1);
	connection c2 = sig.connect(&slots2);
	cout << "First call-------------------" << endl;
	sig();
	if (c2.connected())
	{
		c2.disconnect();
	}
	cout << "Second call-------------------" << endl;
	sig();

	connection c3 = sig2.connect(&A::staticMemberFunc);// �󶨳�Ա����
	connection c4 = sig2.connect(boost::bind(&A::memberFunc, &a1, _1));// �󶨾�̬��Ա����

	cout << "Return code is: " << *sig2(44) << endl;// ֻ�ܷ�����󱻵��õĲ�۵ķ���ֵ

	tp.wait();
}

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	testThreadPool();


	cout << "finish" << endl;
	return a.exec();
}
