#pragma once
#include <coroutine>
#include <exception>
#include <optional>
#include <map>
#include <type_traits>

#define CO_AWAIT co_await std::suspend_always{}

struct ICoroutine
{
    virtual bool done() = 0;
    virtual ~ICoroutine() = default;
};

template<typename T>
class CoroutineBase : public ICoroutine
{
public:
    struct promise_type_base;

    using handle_type = std::coroutine_handle<promise_type_base>;

    struct promise_type_base
	{
        std::optional<T> value_;
        std::exception_ptr exception_;

        promise_type_base() = default;
        ~promise_type_base() = default;

        CoroutineBase get_return_object()
    	{
            return CoroutineBase(handle_type::from_promise(*this));
        }

        auto initial_suspend() { return std::suspend_always{}; }
        auto final_suspend() noexcept { return std::suspend_always{}; }
        void unhandled_exception() { exception_ = std::current_exception(); }
    };

    CoroutineBase(handle_type handler) : handler_(handler) {}
    CoroutineBase(const CoroutineBase&) = delete;
    CoroutineBase(CoroutineBase&&) = delete;
    CoroutineBase& operator=(const CoroutineBase&) = delete;
    CoroutineBase& operator=(CoroutineBase&&) = delete;

    ~CoroutineBase() override
	{
        if (handler_) {
            handler_.destroy();
        }
    }

    std::optional<T> operator()()
	{
        if (!handler_.done()) 
        {
            handler_();
        }

        if (handler_.promise().exception_) 
        {
            std::rethrow_exception(handler_.promise().exception_);
        }

        auto ret = std::move(handler_.promise().value_);
        handler_.promise().value_ = std::nullopt;

        return ret;
    }

    bool done() override
	{
        return handler_.done();
    }

protected:
    handle_type handler_;
};

template<typename T>
struct Coroutine : public CoroutineBase<T>
{
    using CoroutineBase<T>::CoroutineBase;

    struct promise_type : public CoroutineBase<T>::promise_type_base
	{
        using CoroutineBase<T>::promise_type_base::promise_type_base;

        template<std::convertible_to<T> From>
        auto yield_value(From&& from) noexcept {
            value_ = std::forward<From>(from);
            return std::suspend_always{};
        }

        template<std::convertible_to<T> From>
        void return_value(From&& from) noexcept
    	{
            this->value_ = std::forward<From>(from);
        }

        template<std::convertible_to<T> From>
        auto yield_value(From&& from) noexcept
    	{
            value_ = std::forward<From>(from);
            return std::suspend_always{};
        }
    };
};

template<>
struct CoroutineVoid : public CoroutineBase<void>
{
    using CoroutineBase<void>::CoroutineBase;

    struct promise_type : public CoroutineBase<void>::promise_type
	{
        using CoroutineBase<void>::promise_type::yield_value;
        void return_void() {}
    };
};

class CoroutineSystem {
public:
    static std::map<int, std::unique_ptr<ICoroutine>> coroutine;

    template<typename CoroutineType>
    static void addCoroutine(int id, CoroutineType coroutine)
	{
        coroutine.emplace(id, std::make_unique<CoroutineType>(std::move(coroutine)));
    }

    static void removeCoroutine(int id)
	{
        coroutine.erase(id);
    }

    static void runCoroutine(int id)
	{
        if (coroutine.find(id) != coroutine.end() && !coroutine[id]->done()) 
        {
            (*coroutine[id]);
        }
    }
};

std::map<int, std::unique_ptr<ICoroutine>> CoroutineSystem::coroutine;