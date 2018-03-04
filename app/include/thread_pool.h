#pragma once

#include "thread_safe_queue.h"

namespace Test
{

class ThreadPool
{
private:
	class Joiner
	{
	public:
		Joiner(std::vector<std::thread>& threads)
			: m_threadsRef(threads)
		{
		}

		~Joiner()
		{
			for (auto& thread : m_threadsRef)
			{
				if (!thread.joinable())
				{
					continue;
				}

				thread.join();
			}
		}

	private:
		std::vector<std::thread>& m_threadsRef;
	};

	class TaskWrapper
	{
	private:
		struct ImplBase
		{
			virtual void invoke() = 0;
			virtual ~ImplBase() = default;
		};

		template <typename F>
		struct Impl : ImplBase
		{
			Impl(F&& f)
				: f(std::move(f))
			{
			}

			virtual void invoke() override
			{
				f();
			}

			F f;
		};

	public:
		TaskWrapper() = default;

		template <typename F>
		TaskWrapper(F&& f)
			: m_impl(std::make_unique<Impl<F>>(std::forward<F>(f)))
		{
		}

		template <typename F>
		TaskWrapper& operator=(F&& f)
		{
			m_impl = std::make_unique<Impl<F>>(std::forward<F>(f));
		}

		void operator()() const
		{
			m_impl->invoke();
		}

	private:
		std::unique_ptr<ImplBase> m_impl;
	};

public:
	ThreadPool()
		: m_done(false)
		, m_joiner(m_threads)
		, m_runningTasksCount(0)
	{
		try
		{
			for (unsigned i = 0; i < qMax(std::thread::hardware_concurrency() / 2, 1u); ++i)
			{
				m_threads.emplace_back(&ThreadPool::workerThreadEntryPoint, this);
			}
		}
		catch (...)
		{
			m_done = true;
			throw;
		}
	}

	void clearTasks()
	{
		std::lock_guard<std::mutex> locker(m_mutex);
		m_queue.clear();
	}

	void wait() const
	{
		while (pendingTasksCount() != 0 && m_runningTasksCount != 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	std::size_t pendingTasksCount() const
	{
		std::lock_guard<std::mutex> locker(m_mutex);
		return m_queue.size();
	}

	~ThreadPool()
	{
		m_done = true;

		m_wakeupThreadCondition.notify_all();
	}

	template <typename F>
	void pushDetachedTask(F&& f)
	{
		std::lock_guard<std::mutex> locker(m_mutex);
		m_queue.push_back(TaskWrapper(std::move(f)));

		m_wakeupThreadCondition.notify_all();
	}

	template <typename F>
	std::future<typename std::result_of<F()>::type> pushTask(F&& f)
	{
		using ReturnType = typename std::result_of<F()>::type;

		std::packaged_task<ReturnType()> packagedTask(f);
		std::future<ReturnType> future(packagedTask.get_future());

		std::lock_guard<std::mutex> locker(m_mutex);
		m_queue.push_back(TaskWrapper(std::move(packagedTask)));

		m_wakeupThreadCondition.notify_all();

		return future;
	}

// 	void executePendingTask()
// 	{
// 		TaskWrapper task;
// 
// 		if (m_queue.pop(task))
// 		{
// 			++m_runningTasksCount;
// 
// 			task();
// 
// 			--m_runningTasksCount;
// 		}
// 		else
// 		{
// 			std::this_thread::yield();
// 		}
// 	}

private:
	void workerThreadEntryPoint()
	{
		while (!m_done)
		{
			TaskWrapper task;

			{
				std::unique_lock<std::mutex> locker(m_mutex);
				m_wakeupThreadCondition.wait(locker, [this] { return !m_queue.empty() || m_done; });

				if (m_done)
				{
					return;
				}

				task = std::move(m_queue.front());
				m_queue.pop_front();
				++m_runningTasksCount;
			}

			task();

			--m_runningTasksCount;

			std::this_thread::yield();
		}
	}

private:
	std::atomic_bool m_done;
	std::deque<TaskWrapper> m_queue;
	//ThreadSafeQueue<TaskWrapper> m_queue;
	mutable std::mutex m_mutex;
	std::condition_variable m_wakeupThreadCondition;
	std::vector<std::thread> m_threads;
	Joiner m_joiner;
	std::atomic<std::size_t> m_runningTasksCount;
};

}