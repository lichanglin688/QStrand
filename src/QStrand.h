#pragma once

#include <QMutex>
#include <QWaitCondition>
#include <QList>
#include <functional>
#include <QThreadPool>

class QStrand
{
	using Task = std::function<void(void)>;
public:
	QStrand(QThreadPool* threadPool = QThreadPool::globalInstance());
	~QStrand();

	void runAsync(Task handle);
	void waitForFinished();
private:
	void run();
private:
	QList<Task> tasks;
	mutable QMutex mutex;
	//QWaitCondition waitCondition;
	QThreadPool* threadPool;
	bool isRunning{};
};
