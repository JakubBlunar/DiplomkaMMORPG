#ifndef IDMANAGER_H
#define IDMANAGER_H

#include <list>

template <typename  T>
class IDManager
{
public:
	IDManager();
	~IDManager();

	T getId();
	void freeId(T id);

	T max;
	T nextId;

	std::list<T> used;
};


template<typename  T>
IDManager<T>::IDManager()
{
	max = ~0;
	nextId = 0;
}

template<typename  T>
IDManager<T>::~IDManager()
{
}

template<typename  T>
T IDManager<T>::getId()
{
	if (!used.empty()) {
		T id = used.front();
		used.pop_front();
		return id;
	} 

	return ++nextId;

}

template<typename  T>
void IDManager<T>::freeId(T id)
{
	used.push_back(id);
}

#endif