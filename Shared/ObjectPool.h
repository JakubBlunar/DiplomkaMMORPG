#ifndef OBJECT_POOL_H
#define OBJECT_POOL_H

#include <stack> 
#include "ObjectPoolItem.h"

template <class T>
class ObjectPool
{
public:
	ObjectPool() {}
	~ObjectPool() {
		sf::Lock mutexLock(lock);
		while (!pool.empty()) 
	    { 
			ObjectPoolItem* temp = pool.top();
			pool.pop();
			delete temp;
	    }
	}

	void addObject(T* object) {
		sf::Lock mutexLock(lock);
		if (dynamic_cast<ObjectPoolItem*>(object) == nullptr) {
			throw "param does not inherit from ObjectPoolItem";
		}

		object->reset();
		pool.push(object);
	}

	T* getObject() {
		sf::Lock mutexLock(lock);
		if (pool.empty()) {
			throw "Object pool cannot be empty";
		}

		if (pool.size() <= 1) {
			lock.unlock();
			return (T*)pool.top()->clone();
		}

		ObjectPoolItem* temp = pool.top();
		pool.pop();
		return (T*)temp;
	}

	size_t size() const {
		return pool.size();
	}

private:
	std::stack<ObjectPoolItem*> pool;
	sf::Mutex lock;
};


#endif