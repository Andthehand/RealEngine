#pragma once
#include <thread>
#include <atomic>

#include "JobQueue.h"

class WorkerThread {
public:
	WorkerThread(std::shared_ptr<JobQueue> queue);
	~WorkerThread();

	void Stop();
	void Run();
private:
	std::thread m_Thread;
	std::shared_ptr<JobQueue> m_JobQueue;
	std::atomic<bool> m_Running = true;
};
