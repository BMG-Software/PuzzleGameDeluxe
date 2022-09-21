
#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H

#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>

template <typename T>
class ThreadsafeQueue
{
public:
    ThreadsafeQueue() = default;
    ThreadsafeQueue(const ThreadsafeQueue& tsq)
    {
        std::lock_guard<std::mutex> lk(tsq.m_mutex);
        m_dataQueue = tsq.m_dataQueue;
    }

    void Push(T newValue)
    {
        std::lock_guard<std::mutex> lk(m_mutex);
        m_dataQueue.push(newValue);
        m_dataCondition.notify_one();
    }

    void WaitAndPop(T &value)
    {
        std::lock_guard<std::mutex> lk(m_mutex);
        m_dataCondition.wait(lk, [this] { return !m_dataQueue.empty(); });
        value = m_dataQueue.front();
        m_dataQueue.pop();
    }

    std::shared_ptr<T> WaitAndPop()
    {
        std::lock_guard<std::mutex> lk(m_mutex);
        m_dataCondition.wait(lk, [this] { return !m_dataQueue.empty(); });
        std::shared_ptr<T> res(std::make_shared<T>(m_dataQueue.front()));
        m_dataQueue.pop();
    }

    bool TryPop(T& value)
    {
        std::lock_guard<std::mutex> lk(m_mutex);
        if (m_dataQueue.empty())
        {
            return false;
        }

        value = m_dataQueue.front();
        m_dataQueue.pop();
        return true;
    }

    std::shared_ptr<T> TryPop()
    {
        std::lock_guard<std::mutex> lk(m_mutex);
        if (m_dataQueue.empty())
            return std::shared_ptr<T>();
        std::shared_ptr<T> res(std::make_shared<T>(m_dataQueue.front()));
        m_dataQueue.pop();
        return res;
    }

    bool Empty() const
    {
        std::lock_guard<std::mutex> lk(m_mutex);
        return m_dataQueue.empty();
    }

private:
    mutable std::mutex m_mutex;
    std::queue<T> m_dataQueue;
    std::condition_variable m_dataCondition;
};

#endif // THREAD_SAFE_QUEUE_H

