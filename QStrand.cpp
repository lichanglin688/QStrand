#include "QStrand.h"
//#include <QThreadPool>
#include <QtConcurrent>

void QStrand::runAsync(Task handle)
{
	QMutexLocker locker(&mutex);
	tasks.push_back(std::move(handle));
	if (tasks.size() == 1)
	{
		QtConcurrent::run([this] { run(); });
	}
}

bool QStrand::isRunning() const
{
	QMutexLocker locker(&mutex);
	return !tasks.empty();
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
			return;
		}
		Task task = std::move(tasks.first());
		locker.unlock();
		task();
		locker.relock();
		tasks.removeFirst();
		waitCondition.notify_all();
	}
}
