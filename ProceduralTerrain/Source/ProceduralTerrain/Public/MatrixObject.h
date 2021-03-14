// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include "CoreMinimal.h"


/**
 * 
 */

template <class T>
class PROCEDURALTERRAIN_API MatrixObject
{
private:
	size_t rows, columns;
    std::vector<T> matrix;
public:
    MatrixObject(size_t numrow = 0, size_t numCols = 0) : rows(numrow), columns(numCols), matrix(rows * columns) {};

    T & operator()(size_t row, size_t column);
    T operator()(size_t row, size_t column) const;

    T & operator()(size_t index);
    T operator()(size_t index) const;

    size_t GetRowsSize() const;
    size_t GetColumnsSize() const;

    void Add(T value) const;
};

template<class T>
T& MatrixObject<T>::operator()(size_t row, size_t column) 
{
    return matrix[row * columns + column];
}

template<class T>
T MatrixObject<T>::operator()(size_t row, size_t column) const
{
    return matrix[row * columns + column];
}

template<class T>
T& MatrixObject<T>::operator()(size_t index) 
{
    return matrix[index];
}

template<class T>
T MatrixObject<T>::operator()(size_t index) const
{
    return matrix[index];
}

template<class T>
size_t MatrixObject<T>::GetRowsSize() const
{
    return rows;
}

template<class T>
size_t MatrixObject<T>::GetColumnsSize() const
{
    return columns;
}

template<class T>
void MatrixObject<T>::Add(T value) const
{
    
}