#ifndef __MY_ALLOC__
#define __MY_ALLOC__


#include <iostream>
#include <memory>
#include <fstream>

extern std::fstream log_file;

template<typename T>
class MyAlloc : public std::allocator <T>
{
	public:
	     typedef size_t     size_type;
	     typedef ptrdiff_t  difference_type;
	     typedef T*         pointer;
	     typedef const T*   const_pointer;
	     typedef T&         reference;
	     typedef const T&   const_reference;
	     typedef T          value_type;


	     template<typename U>
	     struct rebind
	     {
			typedef MyAlloc <U> other; 
	     };

	     MyAlloc() {}

	     template<typename U>
	     MyAlloc(const MyAlloc<U>&) {}
		void deallocate( T* p, std::size_t n )
		{
			//std::cout << "delete: " << p << "\n";
			log_file << "delete: " << p << "|\n";
			_alloc.deallocate(p, n);
		}

		pointer allocate( size_type n, const void * hint = 0 )
		{
			T *tmp = _alloc.allocate(n, hint);
			log_file << "insert: " << tmp << "|\n";
			return tmp;
		}

	private:
		std::allocator<T> _alloc;

};

#endif
