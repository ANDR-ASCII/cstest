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
	{
		try
		{
			for (unsigned i = 0; i < 1/*std::thread::hardware_concurrency()*/; ++i)
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
		m_queue.clear();
	}

	~ThreadPool()
	{
		m_done = true;

		m_wakeupThreadCondition.notify_all();
	}

	template <typename F>
	void pushDetachedTask(F&& f)
	{
		m_queue.push(TaskWrapper(std::move(f)));

		m_wakeupThreadCondition.notify_all();
	}

	template <typename F>
	std::future<typename std::result_of<F()>::type> pushTask(F&& f)
	{
		using ReturnType = typename std::result_of<F()>::type;

		std::packaged_task<ReturnType()> packagedTask(f);
		std::future<ReturnType> future(packagedTask.get_future());

		m_queue.push(TaskWrapper(std::move(packagedTask)));

		m_wakeupThreadCondition.notify_all();

		return future;
	}

	void executePendingTask()
	{
		TaskWrapper task;

		if (m_queue.pop(task))
		{
			task();
		}
		else
		{
			std::this_thread::yield();
		}
	}

private:
	void workerThreadEntryPoint()
	{
		while (!m_done)
		{
			{
				std::unique_lock<std::mutex> locker(m_mutex);
				m_wakeupThreadCondition.wait(locker, [this] { return !m_queue.empty() || m_done; });

				if (m_done)
				{
					return;
				}
			}

			executePendingTask();
		}
	}

private:
	std::atomic_bool m_done;
	ThreadSafeQueue<TaskWrapper> m_queue;
	mutable std::mutex m_mutex;
	std::condition_variable m_wakeupThreadCondition;
	std::vector<std::thread> m_threads;
	Joiner m_joiner;
};

}