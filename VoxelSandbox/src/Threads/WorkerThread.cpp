#include "WorkerThread.h"


WorkerThread::WorkerThread(std::shared_ptr<JobQueue> queue) : m_JobQueue(queue) {
	m_Thread = std::thread(&WorkerThread::Run, this);
}

WorkerThread::~WorkerThread() {
	Stop();
	m_Thread.join();
}

void WorkerThread::Stop() {
	m_Running = false;
}

void WorkerThread::Run(){
	while(m_Running) {
		std::function<void()> job = m_JobQueue->Pop();
		if (job != nullptr) {
			job();
		}
		else {
			return;
		}
	}
}
