#include "QStrand.h"
#include <QtConcurrent>
#include <QDebug>

QStrand::QStrand()
{
}

QStrand::~QStrand()
{
	waitForFinished();
}

void QStrand::runAsync(Task handle)
{
	QMutexLocker locker(&mutex);
	tasks.push_back(std::move(handle));
	if (tasks.size() == 1)
	{
		QtConcurrent::run([this] { run(); });
	}
}

void QStrand::waitForFinished()
{
	QMutexLocker locker(&mutex);
	while (!tasks.empty())
	{
		waitCondition.wait(&mutex);
	}
}

void QStrand::run()
{
	while (true)
	{
		QMutexLocker locker(&mutex);
		if (tasks.empty())
		{
			waitCondition.notify_one();
			return;
		}
		Task task = std::move(tasks.first());
		locker.unlock();
		task();
		locker.relock();
		tasks.removeFirst();
	}
}