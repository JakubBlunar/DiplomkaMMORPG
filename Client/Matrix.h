#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

template<typename T>
class Matrix
{
public:
    Matrix(int rows, int cols);
	~Matrix();
    T& operator()(int x, int y);
    T* operator()(int x, int y) const;
	void set(int x, int y, T* value);
	T* get(int x, int y);

private:
    int rows;
    int cols;
    std::vector<T*> data;
};

template<typename T>
Matrix<T>::Matrix(int rows, int cols)
: rows(rows),
  cols(cols),
  data(rows * cols)
{
	
}

template <typename T>
Matrix<T>::~Matrix()
{
	for (int i = 0; i < data.size(); i++)
	{
		delete data[i];
	}
}

template<typename T>
T& Matrix<T>::operator()(int x, int y)
{
    return data[x * cols + y];
}

template<typename T>
T* Matrix<T>::operator()(int x, int y) const
{
	return data[x * cols + y];
}

template <typename T>
void Matrix<T>::set(int x, int y, T* value)
{
	data[x * cols + y] = value;
}

template <typename T>
T* Matrix<T>::get(int x, int y)
{
	return data[x * cols + y];
}

#endif
