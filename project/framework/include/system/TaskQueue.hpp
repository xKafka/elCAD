#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>

#pragma once

#include <util/Types.hpp>

#include <functional>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>


#include <condition_variable>
#include <mutex>
#include <queue>
#include <optional>

namespace elcad::fw
{
    template<typename Type>
    class AsyncQueue
    {
    public:
        AsyncQueue();

        auto push(Type&& val) -> void;

        auto tryPop() -> Opt<Type>;

        auto blockingPop() -> Opt<Type>;

        auto stop() -> void;

        [[nodiscard]] auto isEmpty() const -> bool;

    private:
        std::mutex              m_mutex{};

        std::condition_variable m_cv{};

        std::queue<Type>        m_queue{};

        bool                    m_running{};
    };

    template<typename Type>
    AsyncQueue<Type>::AsyncQueue()
    {
    }

    template<typename Type>
    auto AsyncQueue<Type>::tryPop() -> Opt<Type>
    {
        auto lock = std::scoped_lock{ m_mutex };

        if (m_queue.empty())
        {
            return std::nullopt;
        }

        auto out = std::move
        (
            m_queue.front()
        );

        m_queue.pop();

        return makeOpt<Type>(std::move(out));
    }

    template<typename Type>
    auto AsyncQueue<Type>::blockingPop() -> Opt<Type>
    {
        auto lock = std::scoped_lock{ m_mutex };

        m_cv.wait(lock, [this]()
        {
            return !m_queue.empty() || !m_running;
        });

        if (!m_running && m_queue.empty()) 
        {
            return std::nullopt;
        }

        auto item = std::move
        (
            m_queue.front()
        );

        m_queue.pop();

        return makeOpt<Type>
        (
            std::move(item)
        );
    }

    template<typename Type>
    auto AsyncQueue<Type>::stop() -> void
    {
        {
            auto lock = std::scoped_lock{ m_mutex };

            m_running = false;
        }

        m_cv.notify_all();
    }

    template<typename Type>
    auto AsyncQueue<Type>::isEmpty() const -> bool
    {
        auto lock = std::scoped_lock{ m_mutex };

        return m_queue.empty();
    }
}
