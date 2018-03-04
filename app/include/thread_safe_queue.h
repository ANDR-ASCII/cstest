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

	bool popAll(QVector<T>& container)
	{
		std::lock_guard<std::mutex> locker(m_mutex);

		if (m_queue.empty())
		{
			return false;
		}

		container.reserve(static_cast<int>(m_queue.size()));

		std::copy(
			std::make_move_iterator(m_queue.begin()),
			std::make_move_iterator(m_queue.end()),
			std::back_inserter(container)
		);

		m_queue.clear();

		return true;
	}

	bool empty() const noexcept
	{
		std::lock_guard<std::mutex> locker(m_mutex);

		return m_queue.empty();
	}

	std::size_t size() const noexcept
	{
		std::lock_guard<std::mutex> locker(m_mutex);

		return m_queue.size();
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