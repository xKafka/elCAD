#pragma once

#include <util/Types.hpp>

#include <functional>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace elcad::fw
{
	using Task = std::function<void()>;

	class ThreadPool
	{
		auto startThreads(u32 threadCount) -> void;

		auto threadPool() -> void;

	public:
        ThreadPool();

		~ThreadPool();

		auto start() -> void;

		auto enqueue(Task&& task) -> void;

	private:
        std::mutex              m_mutex{};

        Vec<std::jthread>       m_workers{};

        std::queue<Task>		m_tasks{};

        std::atomic_bool        m_running{};

        std::condition_variable m_cv{};
	};

	class MainExecutor
	{
	public:
		MainExecutor();

		auto enqueue(Task&& task) -> void;

		auto poll() -> void;

	private:
		std::mutex			m_mutex{};

		std::queue<Task>	m_tasks{};
	};

	class Executor
	{
	public:
		auto onMain(Task&& task) -> void;

		auto onPool(Task&& task) -> void;

		auto poll() -> void;

		auto start() -> void;

	private:
		UPtr<MainExecutor>	m_mainExecutor{};

		UPtr<ThreadPool>	m_threadPool{};
	};
}