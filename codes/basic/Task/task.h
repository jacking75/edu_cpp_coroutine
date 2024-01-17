//출처: https://kukuta.tistory.com/222

#pragma once

#include <utility>
#include <coroutine>

class Task
{
public:
	// 규칙 1. C++에서 정의된 규칙을 구현한 promise_type 이라는 이름의 타입이 정의되어야 한다.
	struct promise_type
	{
		Task get_return_object()
		{
			return Task{ std::coroutine_handle<promise_type>::from_promise(*this) };
		}

		auto initial_suspend() { return std::suspend_always{}; }

		// co_return을 사용하는 경우 구현. 나중에 코루틴 종료를 설명 할 때 같이 설명
		auto return_void() { return std::suspend_never{}; }

		// 코루틴 종료 시 호출. 나중에 코루틴 종료를 설명 할 때 같이 설명
		auto final_suspend() { return std::suspend_always{}; }

		// 코루틴 수행 중 예외 발생 시 호출
		void unhandled_exception() { std::exit(1); }
	};

	// 규칙 2. std::coroutine_handle<promise_type> 타입의 멤버 변수가 있어야 한다.
	std::coroutine_handle<promise_type> co_handler;

	// 규칙 3. std::coroutine_handle<promise_type> 을 인자로 받아
	// 멤버 변수를 초기화 하는 생성자가 있어야 한다.
	Task(std::coroutine_handle<promise_type> handler) : co_handler(handler) { }

	// 규칙 4. 소멸자에서 std::coroutine_handle<promise_type> 타입의
	// 코루틴 핸들러 멤버 변수의 destroy를 호출 해야 한다.
	~Task()
	{
		if(true == (bool)co_handler) {
			co_handler.destroy();
		}
	}
};
