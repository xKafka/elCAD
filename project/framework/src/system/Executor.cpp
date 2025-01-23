#include "system/Executor.hpp"

namespace elcad::fw
{
    ThreadPool::ThreadPool()
    {
    }

    ThreadPool::~ThreadPool()
    {
        m_running.store(false);

        m_cv.notify_all();
    }

    auto ThreadPool::start() -> void
    {
        static constexpr u32 ThreadCount{ 8 };

        startThreads(ThreadCount);
    }

    auto ThreadPool::startThreads(u32 threadCount) -> void
    {
        m_running = true;

        auto loop = [&]()
        {
            while (m_running)
            {
                threadPool();
            }
        };

        for (u32 i{ 0 }; i < threadCount; ++i)
        {
            m_workers.emplace_back
            (
                std::jthread(loop)
            );
        }
    }

    auto ThreadPool::threadPool() -> void
    {
        auto task = Task{};

        {
            auto lock = std::unique_lock{ m_mutex };

            m_cv.wait
            (
                lock, [&]()
                {
                    return m_running && !m_tasks.empty();
                }
            );

            if (!m_running || m_tasks.empty())
            {
                return;
            }

            task = std::move(m_tasks.front());

            m_tasks.pop();
        }

        if (task)
        {
            task();
        }
    }

	auto ThreadPool::enqueue(Task&& task) -> void
	{
        {
            auto lock = std::scoped_lock{ m_mutex };

            if (!m_running)
            {
                throw std::runtime_error{ "Cannot enqueue on a stopped ThreadPool" };
            }

            m_tasks.emplace
            (
                std::move(task)
            );
        }

        m_cv.notify_one();
	}

    MainExecutor::MainExecutor()
    {

    }

    auto MainExecutor::enqueue(Task&& task) -> void
    {
        auto lock = std::scoped_lock{ m_mutex };

        m_tasks.emplace
        (
            std::move(task)
        );
    }

    auto MainExecutor::poll() -> void
    {
        auto tasks = std::queue<Task>{};

        {
            auto lock = std::scoped_lock{ m_mutex };

            std::swap(tasks, m_tasks);
        }

        while (!tasks.empty())
        {
            auto task = std::move(m_tasks.front());

            tasks.pop();

            if (task)
            {
                task();
            }
        }
    }

	auto Executor::onMain(Task&& task) -> void
	{
        m_mainExecutor->enqueue
        (
            std::move(task)
        );
	}

	auto Executor::onPool(Task&& task) -> void
	{
		m_threadPool->enqueue
        (
            std::move(task)
        );
	}

    auto Executor::poll() -> void
    {
        m_mainExecutor->poll();
    }

    auto Executor::start() -> void
    {
        m_threadPool->start();
    }
}