#pragma once
#include "Core.h"

namespace RealEngine {
	template<typename T>
	class CircularBuffer {
	public:
		CircularBuffer(size_t capacity) :
			m_Capacity(capacity), m_Buffer(new T[capacity]), m_Head(0), m_Tail(0), m_Size(0) {}

		~CircularBuffer() {
			RE_PROFILE_FUNCTION();
			delete[] m_Buffer;
		}

		void push_back(const T& item) {
			RE_PROFILE_FUNCTION();
			if (m_Size == m_Capacity) {
				// If buffer is full, overwrite the head
				m_Buffer[m_Head] = item;
				m_Head = (m_Head + 1) % m_Capacity;
			}
			else {
				// If buffer is not full, simply insert at tail
				m_Buffer[m_Tail] = item;
				m_Tail = (m_Tail + 1) % m_Capacity;
				++m_Size;
			}
		};

		T pop_front() {
			RE_PROFILE_FUNCTION();
			RE_CORE_ASSERT(m_Size != 0);

			T item = m_Buffer[m_Head];
			m_Head = (m_Head + 1) % m_Capacity;
			--m_Size;
			return item;
		}

		T Average() {
			RE_PROFILE_FUNCTION();

			T average = 0;
			for (size_t i = 0; i < m_Size; i++) {
				average += m_Buffer[i];
			}
			return average / m_Size;
		}

		// Get the number of elements in the buffer
		size_t Size() const { return m_Size; }

		// Get the capacity of the buffer
		size_t Capacity() const { return m_Capacity; }
	private:
		size_t m_Capacity;  
		T* m_Buffer;        
		size_t m_Head;      
		size_t m_Tail;      
		size_t m_Size;      
	};
}
