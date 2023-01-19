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
		while (m_Running) {
			std::function<void()> job = m_JobQueue.Pop();
			//If it returns nullptr then the thread will exit
			if (job == nullptr) return;

			job();
		}
	}

}
