#include "ThreadPool.h"

void ThreadPool::threadWaitLoop()
{
	while (!m_stop)
	{
		std::unique_lock<std::mutex> lock(m_mutexQueue);
		m_condition.wait(lock, [&] {return !m_queue.empty() || m_stop;});
		if (!m_queue.empty() && !m_stop)
		{
			auto task = m_queue.front();
			m_queue.pop();
			lock.unlock();
			task();
		}
		else lock.unlock();
	}
}



ThreadPool::ThreadPool()
{
	if (!m_maxThreads)
		m_maxThreads = 2;
	for (int i = 0; i < m_maxThreads; i++)
	{
		m_pool.push_back(std::thread(&ThreadPool::threadWaitLoop, this));
	}
}

void ThreadPool::addTask(std::function<void()> newTask)
{
	std::unique_lock<std::mutex> lock(m_mutexQueue);
	m_queue.push(newTask);
	lock.unlock();
	m_condition.notify_one();
}

void ThreadPool::stop()
{
	m_stop = true;
	m_condition.notify_all(); 
	for (std::thread& thread : m_pool)
	{
		thread.join();
	}
	m_pool.clear();
}
