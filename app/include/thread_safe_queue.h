#pragma once

namespace Test
{

template <typename T>
class ThreadSafeQueue
{
public:
	ThreadSafeQueue() = default;

	void push(const T& object)
	{
		std::lock_guard<std::mutex> locker(m_mutex);
		m_queue.push_back(object);
	}

	void push(T&& object)
	{
		std::lock_guard<std::mutex> locker(m_mutex);
		m_queue.push_back(std::move(object));
	}

	bool pop(T& targetObject)
	{
		std::lock_guard<std::mutex> locker(m_mutex);

		if (m_queue.empty())
		{
			return false;
		}

		targetObject = std::move(m_queue.front());

		m_queue.pop_front();

		return true;
	}

	bool empty()
	{
		std::lock_guard<std::mutex> locker(m_mutex);

		return m_queue.empty();
	}

	void clear()
	{
		std::lock_guard<std::mutex> locker(m_mutex);

		m_queue.clear();
	}

private:
	mutable std::mutex m_mutex;
	std::deque<T> m_queue;
};

}