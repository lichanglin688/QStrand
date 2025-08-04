#pragma once

#include <QMutex>
#include <QWaitCondition>
#include <QList>
#include <functional>

class QStrand
{
	using Task = std::function<void(void)>;
public:
	QStrand();
	~QStrand();

	void runAsync(Task handle);
	void waitForFinished();
private:
	void run();
private:
	QList<Task> tasks;
	mutable QMutex mutex;
	QWaitCondition waitCondition;
};
