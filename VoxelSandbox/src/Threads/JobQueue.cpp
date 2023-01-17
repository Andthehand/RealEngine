#include "JobQueue.h"

void JobQueue::Stop() {
	m_Running = false;
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
