#include <QCoreApplication>
#include "src/QStrand.h"
#include <QThread>
#include <QDebug>
#include <QRandomGenerator>

int count = 0;
int main(int argc, char* argv[])
{
	QCoreApplication app(argc, argv);
	QStrand strand;
	for (int i = 0; i < 3; i++)
	{
		std::thread t([&strand] () {
			for (int i = 0; i < 1000; i++)
			{
				strand.runAsync([i] ()
				{
					//QThread::msleep(QRandomGenerator::global()->bounded(10, 100));
					qDebug() << i << QThread::currentThreadId() << ++count;
				});
				//QThread::msleep(QRandomGenerator::global()->bounded(10, 100));
			}
		});
		t.join();
	}

	strand.waitForFinished();
	qDebug() << "All tasks finished in main thread" << QThread::currentThreadId();
	return 0;
	//return app.exec();
}
