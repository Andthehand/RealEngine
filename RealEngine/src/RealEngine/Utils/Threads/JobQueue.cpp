#include "repch.h"
#include "JobQueue.h"

namespace RealEngine {
	JobQueue::JobQueue(uint32_t numThreads) {
		for (uint32_t i = 0; i < numThreads; i++) {
			m_WorkerThreads.emplace_back(std::make_unique<WorkerThread>(*this));
		}
	}

	void JobQueue::Start() {
		for (auto& thread : m_WorkerThreads) {
			thread->Start();
		}
	}

	void JobQueue::Stop() {
		m_Running = false;
		Clear();
		m_CV.notify_all();
	}

	void JobQueue::Push(std::function<void()> job) {
		std::lock_guard<std::mutex> lock(m_Mtx);
		m_Jobs.push(job);
		m_CV.notify_one();
	}

	std::function<void()> JobQueue::Pop() {
		std::unique_lock<std::mutex> lock(m_Mtx);
		while (m_Jobs.empty()) {
			m_CV.wait(lock);
			if (!m_Running) return nullptr;
		}
		auto job = m_Jobs.front();
		m_Jobs.pop();
		return job;
	}

	void JobQueue::Clear() {
		std::queue<std::function<void()>>().swap(m_Jobs);
	}

}
