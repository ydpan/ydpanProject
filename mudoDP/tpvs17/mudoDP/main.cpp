// #include "mudoDP.h"
// #include <QtWidgets/QApplication>
// 
// int main(int argc, char *argv[])
// {
// 	QApplication a(argc, argv);
// 	mudoDP w;
// 	w.show();
// 	return a.exec();
// }

#include "muduo/base/ThreadPool.h"
#include "muduo/base/CountDownLatch.h"
#include "muduo/base/CurrentThread.h"
#include "muduo/base/Logging.h"

#include <stdio.h>
#include <thread>
using namespace std;
void print()
{
	printf("tid=%d\n", muduo::CurrentThread::tid());
}

void printString(const std::string& str)
{
	LOG_INFO << str;
	std::this_thread::sleep_for(std::chrono::milliseconds(2 * 1000));
}

void test(int maxSize)
{
	LOG_WARN << "Test ThreadPool with max queue size = " << maxSize;
	muduo::ThreadPool pool(maxSize);


	LOG_WARN << "Adding";
	pool.commit(print);
	pool.commit(print);
	for (int i = 0; i < 100; ++i)
	{
		char buf[32];
		snprintf(buf, sizeof buf, "task %d", i);
		pool.commit(std::bind(printString, std::string(buf)));
	}
	LOG_WARN << "Done";

	muduo::CountDownLatch latch(1);
	pool.commit(std::bind(&muduo::CountDownLatch::countDown, &latch));
	latch.wait();
	pool.stop();
}

/*
 * Wish we could do this in the future.
void testMove()
{
  muduo::ThreadPool pool;
  pool.start(2);

  std::unique_ptr<int> x(new int(42));
  pool.run([y = std::move(x)]{ printf("%d: %d\n", muduo::CurrentThread::tid(), *y); });
  pool.stop();
}
*/

int main()
{
	test(1);
// 	test(1);
// 	test(5);
// 	test(10);
// 	test(50);
	int a = 0;
	return 0;
}
