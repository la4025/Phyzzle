#pragma once
#include <coroutine>
#include <exception>

template <typename T>
class Coroutine
{
	class awaitable;
	struct promise_type;
	
	using handle_type = std::coroutine_handle<promise_type>;
	// using traits_type = std::coroutine_traits<promise_type>;

	//class awaitable
	//{
	//	bool ready_;
	//public:
	//	explicit(false) awaitable(bool ready) : ready_(ready) {}
	//
	//	bool await_ready() const noexcept { return ready_; }
	//	void await_suspend(std::coroutine_handle<> h) noexcept {}
	//	void await_resume() noexcept {}
	//};

	struct promise_type
	{
		Coroutine get_return_object() {
			return Coroutine(handle_type::from_promise(*this));
		}
		auto initial_suspend() { return std::suspend_always{}; }
		auto final_suspend() noexcept { return std::suspend_always{}; }
		void unhandled_exception() { exception_ = std::current_exception(); }
		void return_void() {}

		template<std::convertible_to<T> From>
		void return_value(const From& from)
		{
			return_value_ = from;
		}

		template<std::convertible_to<T> From> 
		void return_value(From&& from) noexcept
		{
			return_value_ = std::move(from);
		}

		template<std::convertible_to<T> From>
		auto yield_value(From&& from) noexcept
		{
			curr_value_ = std::forward<From>(from);	// 값을 저장하고 제어권을 넘김
			return std::suspend_always{};
		}

		//auto await_transform(std::suspend_always) { return awaitable(!ready_); }
		//void disable_suspension() { ready_ = false; }

	private:
		T curr_value_;
		T return_value_;
		std::exception_ptr exception_;
		bool ready_{ true };
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

	void disable_suspension() const
	{
		if (handler_.done())
			return;
		handler_.promise().disable_suspension();
		handler_();
	}

	bool operator()()
	{
		if (!handler_.done())
			handler_();
		
		if (handler_.promise().exception_)
			std::rethrow_exception(handler_.promise().exception_);
		
		// 함수를 완료하면 true를 반환함
		return handler_.done();
	}

	T yield_value()
	{
		return handler_.promise().curr_value_;
	}

	T return_value()
	{
		return handler_.promise().return_value_;
	}

private:
	handle_type handler_;
};

