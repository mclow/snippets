//	An implementation of
//	P0052R5: Generic Scope Guard and RAII Wrapper for the Standard Library
//		https://wg21.link/P0052

#include <type_traits>

template <typename _Func>
class scope_exit {
public:
	template <typename _EFF>
	explicit scope_exit (_EFF && __f)
		: __exit_proc{std::forward<_EFF>(__f)}, __execute{true} {}

	scope_exit (scope_exit && __rhs)
		: __exit_proc{std::move(__rhs.__exit_proc)}, __execute{__rhs.__execute}
	    { __rhs.__execute = false; return *this; }

	~scope_exit() { if (__execute) __exit_proc(); }
	
	void release() noexcept { __execute = false; };
	
	scope_exit            (const scope_exit &)  = delete;
	scope_exit& operator= (const scope_exit &)  = delete;
	scope_exit& operator= (      scope_exit &&) = delete;

private:
	_Func __exit_proc;
	bool __execute;
	};

template <typename _Func>
  scope_exit(_Func &&) -> scope_exit<std::decay_t<_Func>>;

#include <iostream>


int main () {
	std::cout << "Hello World" << std::endl;
	{
	scope_exit e ([] {std::cout << "Goodbye Scope #1" << std::endl; });
	}
	{
	scope_exit e ([] {std::cout << "Goodbye Scope #2" << std::endl; });
	e.release();
	}
	std::cout << "Goodbye World" << std::endl;
}
