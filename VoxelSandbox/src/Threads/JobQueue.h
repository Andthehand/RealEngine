#pragma once
#include <queue>
#include <mutex>
#include <functional>
#include <condition_variable>

#include <RealEngine.h>

#include "WorkerThread.h"

class JobQueue {
public:
	JobQueue(uint32_t numThreads);

	void Stop();

	void Push(std::function<void()> job);

	std::function<void()> Pop();

private:
	std::queue<std::function<void()>> m_Jobs;
	std::mutex m_Mtx;
	std::atomic<bool> m_Running = true;

	//This is used to notify the worker threads when a job is added
	std::condition_variable m_CV;

	std::vector<std::unique_ptr<WorkerThread>> m_WorkerThreads;
};
