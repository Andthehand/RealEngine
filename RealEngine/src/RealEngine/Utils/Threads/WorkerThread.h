#pragma once
#include <thread>
#include <atomic>

namespace RealEngine {
	class JobQueue;

	class WorkerThread {
	public:
		WorkerThread(JobQueue& queue);
		~WorkerThread();

		void Start();
		void Stop();

		void Run();
	private:
		std::thread m_Thread;
		JobQueue& m_JobQueue;
		std::atomic<bool> m_Running = true;
	};

}
