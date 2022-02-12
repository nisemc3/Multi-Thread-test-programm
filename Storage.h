#pragma once

#include <mutex>
#include <vector>
#include <algorithm>
#include <iostream>


class Storage
{
private:
	explicit Storage(int maxItems);
	~Storage() {}
	Storage(Storage& other) = delete;
	void operator=(const Storage& other) = delete;

	static Storage* m_instance;
	static std::mutex m_mutex;

	std::vector<int> m_storage{};
	int m_maxItems{};

public:
	static Storage* getInstance(int maxItems = 0);
	void addData(int data);
	int makeCalculations();

};

