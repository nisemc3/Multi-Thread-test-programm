#include "Storage.h"

Storage* Storage::m_instance{ nullptr };
std::mutex Storage::m_mutex;

Storage::Storage(int maxItems = 0): m_maxItems(maxItems)
{
    m_storage.reserve(m_maxItems+1);
}

Storage* Storage::getInstance(int maxItems)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_instance == nullptr)
    {
        m_instance = new Storage(maxItems);
    }
    return m_instance;
}

void Storage::addData(int data)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_storage.push_back(data);
    std::cout << "m_storage = " << m_storage.size() << '\n';
}

int Storage::makeCalculations()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_storage.size() == m_maxItems)
    {
        std::sort(m_storage.begin(), m_storage.end(), std::greater<int>());
        if (m_maxItems % 2 == 0)
            return (m_storage.at((m_maxItems-1) / 2) + m_storage.at((m_maxItems-1) / 2 + 1)) / 2;
        else
            return m_storage.at((m_maxItems-1) / 2);
    }
    else return 0;
}
