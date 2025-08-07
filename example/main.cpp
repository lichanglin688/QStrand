#include <QCoreApplication>
#include "src/QStrand.h"
#include <QDebug>
#include <QRandomGenerator>

int main(int argc, char* argv[])
{
	QCoreApplication app(argc, argv);
	QStrand strand;
	QObject::connect(&strand, &QStrand::finished, [] ()
	{
		qDebug() << "All tasks finished in strand thread" << QThread::currentThreadId();
	});
	for (int i = 0; i < 100; i++)
	{
		strand.runAsync([i] ()
		{
			//QThread::msleep(QRandomGenerator::global()->bounded(10, 100));
			qDebug() << i << QThread::currentThreadId();
		});
		QThread::msleep(QRandomGenerator::global()->bounded(10, 100));
	}
	strand.waitForFinished();
	//return 0;
	//qDebug() << "waitForFinished" << QThread::currentThreadId();
	return app.exec();
}
