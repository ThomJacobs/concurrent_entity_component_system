#ifndef NP_ECS_THREAD_SAFE_QUEUE_H
#define NP_ECS_THREAD_SAFE_QUEUE_H

//Includes:
#include<mutex>
#include<atomic>
#include<assert.h>

//Defines:
#define EMPTY 0

namespace thread_safe
{
	template<typename Type> struct Node
	{
	public:
		//Attributes:
		Type value;
		Node* next;

		//Constructor:
		Node() : value(NULL), next(nullptr) {}
		Node(Type value, Node* next = nullptr) : value(value), next(next) {}
	};

	/// <summary>A thread safe implementation of the queue data structure.</summary>
	template<typename Type> class Queue
	{
	private:
		//Attributes:
		Node<Type>* front;
		Node<Type>* rear;
		std::atomic<unsigned int> size;
		std::mutex mutex;

	public:
		//Constructor:
		Queue() : front(nullptr), rear(nullptr) {}

		//Methods:
		bool IsEmpty() 
		{ 
			return front == nullptr; 
		}

		unsigned int Size() { return size; }

		void Enqueue(Type value)
		{
			std::lock_guard<std::mutex> lock(mutex); //Lock instance.

			Node<Type>* temp = new Node<Type>(std::move(value));
			if (IsEmpty())
			{
				front = rear = temp; //Remember, C++ performs operations in reverse.
				return;
			}

			rear->next = temp; //Extend the queue.
			rear = temp; //Define the new ending component.
			size++;
		}

		void Dequeue()
		{
			std::lock_guard<std::mutex> lock(mutex); //Lock instance.

			if(IsEmpty()) return;

			Node<Type>* temp = front;
			front = front->next;

			if (front == nullptr) rear = nullptr;
			delete(temp);
			size--;
		}

		bool TryRetrieve(Type* data)
		{
			std::lock_guard<std::mutex> lock(mutex); //Lock instance.

			if (front == nullptr) return false;

			Node<Type>* temp = front;
			front = front->next;
			*data = temp->value;

			if (front == nullptr) rear = nullptr;
			delete(temp);
			size--;

			return true;
		}

		Type Retrieve()
		{
			std::lock_guard<std::mutex> lock(mutex); //Lock instance.

			assert(!IsEmpty(), "Queue has no components to retrieve.");

			Node<Type>* temp = front;
			front = front->next;
			Type value = temp->value;

			if (front == nullptr) rear = nullptr;
			delete(temp);
			size--;
			return value;
		}

		//Destructor:
		~Queue()
		{
			std::lock_guard<std::mutex> lock(mutex); //Lock instance.

			while (!IsEmpty())
			{
				Node<Type>* temp = front;
				front = front->next;
				delete(temp);
			}

			rear = nullptr;
			size = 0;
		}
	};
}
#endif //!NP_ECS_THREAD_SAFE_QUEUE_H