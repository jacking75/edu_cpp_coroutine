#include <coroutine>
#include <utility>
#include <thread>

class Task
{
public:
	struct promise_type
	{
		int value;

		Task get_return_object()
		{
			return Task{ std::coroutine_handle<promise_type>::from_promise(*this) };
		}

		auto initial_suspend() { return std::suspend_always{}; }
		auto return_void() { return std::suspend_never{}; }
		auto final_suspend() noexcept { return std::suspend_always{}; }	
		void unhandled_exception() {}
	};

	std::coroutine_handle<promise_type> co_handler;

	Task(std::coroutine_handle<promise_type> handler) : co_handler(handler) 
	{
	}

	~Task()
	{
		if (co_handler)
		{
			co_handler.destroy();
		}
	}

};

struct switch_to_new_thread
{
	constexpr bool await_ready() const noexcept { return false; }

	void await_suspend(std::coroutine_handle<> handle) const noexcept
	{
		std::thread t([handle]() {
				handle.resume();
			});
		
		t.detach();
	}

	constexpr void await_resume() const noexcept {}
};


