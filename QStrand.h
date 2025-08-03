#pragma once

#include <QMutex>
#include <QWaitCondition>
#include <QList>
#include <functional>

class QStrand
{
	using Task = std::function<void(void)>;
public:
	QStrand() = default;
	void runAsync(Task handle);
	bool isRunning() const;
	void waitForFinished();
private:
	void run();
private:
	QList<Task> tasks;
	mutable QMutex mutex;
	QWaitCondition waitCondition;
};
