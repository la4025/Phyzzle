#pragma once

#include <coroutine>
#include <exception>
#include <optional>

template<typename T>
struct Coroutine
{
	struct promise_type;

	using handle_type = std::coroutine_handle<promise_type>;

	struct promise_type
	{
		std::optional<T> value_;
		std::exception_ptr exception_;

		promise_type() = default;
		~promise_type() = default;
		promise_type(const promise_type&) = delete;
		promise_type(promise_type&&) = delete;
		promise_type& operator=(const promise_type&) = delete;
		promise_type& operator=(promise_type&&) = delete;

		Coroutine get_return_object()
		{
			return Coroutine(handle_type::from_promise(*this));
		}
		auto initial_suspend() { return std::suspend_always{}; }
		auto final_suspend() noexcept { return std::suspend_always{}; }
		void unhandled_exception() { exception_ = std::current_exception(); }

		template<std::convertible_to<T> From>
		void return_value(const From& from)
		{
			value_ = from;						// 반환값을 저장함
		}

		template<std::convertible_to<T> From>
		void return_value(From&& from) noexcept
		{
			value_ = std::move(from);			// 반환값을 저장함
		}

		template<std::convertible_to<T> From>
		auto yield_value(From&& from) noexcept
		{
			value_ = std::forward<From>(from);	// 값을 저장하고 제어권을 넘김
			return std::suspend_always{};
		}
	};

	Coroutine(handle_type handler) : handler_(handler) {}
	Coroutine(const Coroutine&) = delete;
	Coroutine(Coroutine&&) = delete;
	Coroutine& operator=(const Coroutine&) = delete;
	Coroutine& operator=(Coroutine&&) = delete;

	~Coroutine()
	{
		if (handler_)
			handler_.destroy();
	}

	std::optional<T> operator()()
	{
		if (!handler_.done())
			handler_();

		if (handler_.promise().exception_)
			std::rethrow_exception(handler_.promise().exception_);

		auto ret = std::move(handler_.promise().value_);
		handler_.promise().value_ = std::nullopt;

		return ret;
	}

	bool done()
	{
		return handler_.done();
	}

private:
	handle_type handler_;
};

template <typename T>
struct CoroutineVoid
{
	struct promise_type;

	using handle_type = std::coroutine_handle<promise_type>;

	struct promise_type
	{
		std::optional<T> value_;
		std::exception_ptr exception_;

		promise_type() = default;
		~promise_type() = default;
		promise_type(const promise_type&) = delete;
		promise_type(promise_type&&) = delete;
		promise_type& operator=(const promise_type&) = delete;
		promise_type& operator=(promise_type&&) = delete;

		CoroutineVoid get_return_object()
		{
			return CoroutineVoid(handle_type::from_promise(*this));
		}
		auto initial_suspend() { return std::suspend_always{}; }
		auto final_suspend() noexcept { return std::suspend_always{}; }
		void unhandled_exception() { exception_ = std::current_exception(); }
		void return_void() {}

		template<std::convertible_to<T> From>
		auto yield_value(From&& from) noexcept
		{
			value_ = std::forward<From>(from);		// 값을 저장하고 제어권을 넘김
			return std::suspend_always{};
		}
	};

	CoroutineVoid(handle_type handler) : handler_(handler) {}
	CoroutineVoid(const CoroutineVoid&) = delete;
	CoroutineVoid(CoroutineVoid&&) = delete;
	CoroutineVoid& operator=(const CoroutineVoid&) = delete;
	CoroutineVoid& operator=(CoroutineVoid&&) = delete;

	~CoroutineVoid()
	{
		if (handler_)
			handler_.destroy();
	}

	std::optional<T> operator()()
	{
		if (!handler_.done())
			handler_();

		if (handler_.promise().exception_)
			std::rethrow_exception(handler_.promise().exception_);

		auto ret = std::move(handler_.promise().value_);
		handler_.promise().value_ = std::nullopt;

		return ret;
	}

	bool done()
	{
		return handler_.done();
	}

private:
	handle_type handler_;
};