#pragma once
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>

class ThreadPool
{
	int m_maxThreads = std::thread::hardware_concurrency() ;
	bool m_stop = false;
	std::mutex m_mutexQueue;
	std::condition_variable m_condition;
	std::queue<std::function<void()>> m_queue;
	std::vector<std::thread> m_pool;
	void threadWaitLoop();
	
public:
	ThreadPool();

	void addTask(std::function<void()> newTask);
	void stop();

};

