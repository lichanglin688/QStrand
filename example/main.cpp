#include <QCoreApplication>
#include "src/QStrand.h"
#include <QThread>
#include <QDebug>
#include <QRandomGenerator>

int main(int argc, char* argv[])
{
	QCoreApplication app(argc, argv);
	QStrand strand;
	for (int i = 0; i < 100; i++)
	{
		strand.runAsync([i] ()
		{
			QThread::msleep(QRandomGenerator::global()->bounded(10, 100));
			qDebug() << i << QThread::currentThreadId();
		});
		//QThread::msleep(QRandomGenerator::global()->bounded(10, 100));
	}
	strand.waitForFinished();
	qDebug() << "All tasks finished in main thread" << QThread::currentThreadId();
	return 0;
	//return app.exec();
}
