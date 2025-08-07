#include "QStrand.h"
#include <QtConcurrent>
#include <QDebug>
#include <QFuture>

QStrand::QStrand(QThreadPool* threadPool, QObject* parent)
	:QObject(parent)
	,threadPool(threadPool)
{
}
QStrand::~QStrand()
{
	waitForFinished();
}

void QStrand::runAsync(Task handle)
{
	QMutexLocker locker(&mutex);
	if (!isRunning)
	{
		tasks.push_back(std::move(handle));
		QFutureWatcher<void>* watcher = new QFutureWatcher<void>(this);
		connect(watcher, &QFutureWatcher<void>::finished, this, &QStrand::onFinished);
		QFuture<void> future = QtConcurrent::run(threadPool, [this] { run(); });
		watcher->setFuture(future);
		watchers.push_back(watcher);
		return;
	}
	tasks.push_back(std::move(handle));
}

void QStrand::onFinished()
{
	QFutureWatcher<void>* watcher = static_cast<QFutureWatcher<void>*>(sender());
	watcher->deleteLater();
	QMutexLocker locker(&mutex);
	watchers.removeOne(watcher);
	if (watchers.isEmpty())
	{
		emit finished();
	}
}

void QStrand::waitForFinished()
{
	for(auto& watcher : watchers)
	{
		watcher->waitForFinished();
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