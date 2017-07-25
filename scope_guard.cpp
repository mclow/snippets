//	An implementation of
//	P0052R5: Generic Scope Guard and RAII Wrapper for the Standard Library
//		https://wg21.link/P0052

#define _LIBCPP_DISABLE_AVAILABILITY

#include <type_traits>
#include <exception>
#include <limits>

template <typename _Func, typename _Policy>
class __scope_guard {
public:
	template <typename _EFF>
	explicit __scope_guard (_EFF && __f)
		: __exit_proc{std::forward<_EFF>(__f)}, __policy{} {}

	__scope_guard (__scope_guard && __rhs)
		: __exit_proc{std::move(__rhs.__exit_proc)}, 
		  __policy   {std::move(__rhs.__policy)}
	    { return *this; }

	~__scope_guard() { if (__policy) __exit_proc(); }
	
	void release() noexcept { __policy.release(); }
	
	__scope_guard            (const __scope_guard &)  = delete;
	__scope_guard& operator= (const __scope_guard &)  = delete;
	__scope_guard& operator= (      __scope_guard &&) = delete;

private:
	_Func __exit_proc;
	_Policy __policy;
	};


struct __scope_exit {
	__scope_exit() noexcept : __execute{true} {}
	__scope_exit(__scope_exit &&__rhs) noexcept
		: __execute{__rhs.__execute}
		{ __rhs.release(); }
	void release() noexcept { __execute = false; }
	explicit operator bool () const noexcept { return __execute; }
	bool __execute;
};

struct __scope_fail {
	__scope_fail() noexcept : __num_exceptions{std::uncaught_exceptions()} {}
	__scope_fail(__scope_fail &&__rhs) noexcept
		: __num_exceptions{__rhs.__num_exceptions}
		{ __rhs.release(); }
	void release() noexcept { __num_exceptions = std::numeric_limits<int>::max(); }
	explicit operator bool () const noexcept { return __num_exceptions < std::uncaught_exceptions(); }
	int __num_exceptions;
};

struct __scope_success {
	__scope_success() noexcept : __num_exceptions{std::uncaught_exceptions()} {}
	__scope_success(__scope_success &&__rhs) noexcept
		: __num_exceptions{__rhs.__num_exceptions}
		{ __rhs.release(); }
	void release() noexcept { __num_exceptions = 0; }
	explicit operator bool () const noexcept { return __num_exceptions >= std::uncaught_exceptions(); }
	int __num_exceptions;
};

template <typename _EFF>
struct scope_exit : public __scope_guard<_EFF, __scope_exit> 
	{ using __scope_guard<_EFF, __scope_exit>::__scope_guard; };

// template <typename _EFF>
// using scope_exit = scope_exit<_EFF>;


// template <typename _Func>
// class scope_exit {
// public:
// 	template <typename _EFF>
// 	explicit scope_exit (_EFF && __f)
// 		: __exit_proc{std::forward<_EFF>(__f)}, __execute{true} {}
// 
// 	scope_exit (scope_exit && __rhs)
// 		: __exit_proc{std::move(__rhs.__exit_proc)}, __execute{__rhs.__execute}
// 	    { __rhs.__execute = false; return *this; }
// 
// 	~scope_exit() { if (__execute) __exit_proc(); }
// 	
// 	void release() noexcept { __execute = false; };
// 	
// 	scope_exit            (const scope_exit &)  = delete;
// 	scope_exit& operator= (const scope_exit &)  = delete;
// 	scope_exit& operator= (      scope_exit &&) = delete;
// 
// private:
// 	_Func __exit_proc;
// 	bool __execute;
// 	};



template <typename _Func>
  scope_exit(_Func &&) -> scope_exit<std::decay_t<_Func>>;

#include <iostream>


int main () {
	std::cout << "Hello World" << std::endl;
	{
	scope_exit e ([&] {std::cout << "Goodbye Scope #1" << std::endl; });
	}
	{
	scope_exit e ([&] {std::cout << "Goodbye Scope #2" << std::endl; });
	e.release();
	}
	std::cout << "Goodbye World" << std::endl;
}
