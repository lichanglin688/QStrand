#pragma once

#include <QMutex>
#include <QList>
#include <functional>
#include <QThreadPool>
#include <QFutureWatcher>

class QStrand : public QObject
{
	Q_OBJECT
	using Task = std::function<void(void)>;
public:
	QStrand(QThreadPool* threadPool = QThreadPool::globalInstance(), QObject* parent = nullptr);
	~QStrand();

	void runAsync(Task handle);
	void waitForFinished();
signals:
	void finished();
private:
	void run();
private slots:
	void onFinished();
private:
	QList<Task> tasks;
	mutable QMutex mutex;
	QThreadPool* threadPool;
	QList<QFutureWatcher<void>*> watchers;
	bool isRunning{};
};
