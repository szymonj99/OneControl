#pragma once

#include <queue>
#include <mutex>

namespace oc
{
	template<typename T>
	class Queue
	{
	private:
		std::queue<T> m_queue = std::queue<T>();
		std::mutex m_mutex = std::mutex();
		std::size_t m_maxSize = SIZE_MAX; // Limit of how many elements can be stored in the queue. If this is reached, the next `push` will get rid of the first element.
		void m_emptyNonBlocking() const;

	public:
		oc::Queue<T>() = default;
		oc::Queue<T>(const std::size_t maxSize);
		bool empty() const; // Check if the queue is empty.
		T pop(); // Get the first element from the queue AND remove it from the queue.
		void push(const T& t); // Push an element onto the back queue.
		void setMaxSize(const std::size_t size); // Set the new maximum size of the internal queue. If smaller than current size, the first x elements will be dropped.
	};

	// Constructors

	template<typename T>
	inline oc::Queue<T>::Queue(const std::size_t maxSize)
	{
		if (maxSize <= 0)
		{
			throw std::runtime_error(std::string("Can't create a Queue whose max size is 0 or less."));
		}

		this->m_maxSize = maxSize;
	}

	// Other functions

	template<typename T>
	inline void Queue<T>::m_emptyNonBlocking() const
	{
		return this->m_queue.empty();
	}

	template<typename T>
	inline bool oc::Queue<T>::empty() const
	{
		std::unique_lock lock(this->m_mutex);
		// Let's be inconsistent here.
		// I want to try using `this->` for a bit and see how that affects the code.
		// We could just call m_queue here. *Shrugs*
		// Python uses `self` and it seems to work just fine.
		return this->m_queue.empty();
	}

	template<typename T>
	inline T oc::Queue<T>::pop()
	{
		std::unique_lock lock(this->m_mutex);
		if (this->m_emptyNonBlocking())
		{
			// It'd be nice to use something like `std::nested_exception` and `std::throw_with_nested` here. That looks neat! https://stackoverflow.com/questions/8480640/how-to-throw-a-c-exception
			throw std::runtime_error(std::string("Called `pop` on an empty queue!"));
		}

		// I think we could get a copy of the object here. Maybe...
		const T item = this->m_queue.front();
		this->m_queue.pop();
		return item;
	}

	template<typename T>
	inline void Queue<T>::push(const T& t)
	{
		std::unique_lock lock(m_mutex);

		if (this->m_queue.size == this->m_maxSize)
		{
			this->m_queue.pop();
		}

		this->m_queue.push(t);
	}

	template<typename T>
	inline void Queue<T>::setMaxSize(const std::size_t size)
	{
		std::unique_lock lock(this->m_mutex);

		if (size <= 0)
		{
			throw std::runtime_error(std::string("Can't create a Queue whose max size is 0 or less."));
		}

		this->m_maxSize = size;

		while (this->m_queue.size() > this->m_maxSize)
		{
			this->m_queue.pop();
		}
	}
}
