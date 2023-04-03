#include "repch.h"
#include "WorkerThread.h"

#include "JobQueue.h"

namespace RealEngine {
	WorkerThread::WorkerThread(JobQueue& queue) : m_JobQueue(queue) {
		m_Thread = std::thread(&WorkerThread::Run, this);
	}

	WorkerThread::~WorkerThread() {
		Stop();
		m_Thread.join();
	}

	void WorkerThread::Start() {
		if (!m_Running) {
			m_Running = true;
			m_Thread = std::thread(&WorkerThread::Run, this);
		}
	}

	void WorkerThread::Stop() {
		m_Running = false;
	}

	void WorkerThread::Run() {
		std::string name = "WorkerThread: " + std::hash<std::thread::id>{}(std::this_thread::get_id());
		RE_PROFILE_NEW_THREAD("Worker Thread {}");

		while (m_Running) {
			std::function<void()> job;
			{
				RE_PROFILE_FUNCTION("Waiting for Job");
				job = m_JobQueue.Pop();
				//If it returns nullptr then the thread will exit
				if (job == nullptr) return;
			}

			RE_PROFILE_FUNCTION("Executing Job");
			job();
		}
	}

}
