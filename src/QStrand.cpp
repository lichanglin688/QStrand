#include "QStrand.h"
#include <QtConcurrent>
#include <QDebug>

QStrand::QStrand(QThreadPool* threadPool)
	:threadPool(threadPool)
{
}

QStrand::~QStrand()
{
	waitForFinished();
}

void QStrand::runAsync(Task handle)
{
	QMutexLocker locker(&mutex);
	if (isRunning)
	{
		tasks.push_back(std::move(handle));
	}
	else
	{
		tasks.push_back(std::move(handle));
		isRunning = true;
		QtConcurrent::run(threadPool, [this] { run(); });
	}
}

void QStrand::waitForFinished()
{
	threadPool->waitForDone();
}

void QStrand::run()
{
	while (true)
	{
		QMutexLocker locker(&mutex);
		if (tasks.empty())
		{
			return;
		}
		QList<Task> notRunTasks = std::move(this->tasks);
		isRunning = true;
		locker.unlock();
		for (auto& task : notRunTasks)
		{
			task();
		}
		locker.relock();
		isRunning = false;
	}
}