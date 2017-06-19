#include "UBVector.h"
#include <iostream>
#include <stdexcept>

using namespace std;


const size_t UBVector::INITIAL_CAPACITY = 5;


UBVector::UBVector(const UBVector& another) :
    num_items(another.num_items),
    current_capacity(another.current_capacity),
    item_ptr(new int[another.num_items])
    {
        for(size_t i = 0; i < num_items; i++)
            item_ptr[i] = another.item_ptr[i];
    }

UBVector::UBVector(size_t n = 0, int val =0) :
    num_items(n),
    current_capacity(max(n, INITIAL_CAPACITY)),
    item_ptr(new int[current_capacity])
    {
        for(size_t i = 0; i < n; i++)
            item_ptr[i] = val;
    }

UBVector::UBVector(int* pBegin, int* pEnd)
{

    size_t new_num = ((pEnd+1)-pBegin);

    num_items = new_num;
    current_capacity = max(new_num, INITIAL_CAPACITY);
    item_ptr = new int[current_capacity];
    for(int i = 0; i < num_items; i++)
    {
        item_ptr[i] = *(pBegin+i);

    }

}

UBVector& UBVector::operator=(const UBVector& another)
{
    UBVector temp(another);
    swap(temp);



    return (*this);

}

UBVector::~UBVector()
{
    delete [] item_ptr;
}

size_t UBVector::size() const
{
    return num_items;
}

size_t UBVector::capacity() const
{
    return current_capacity;
}

bool UBVector::empty()
{
    return num_items == 0;
}

int* UBVector::begin()
{
    return &(*this)[0];
}

int* UBVector::end()
{
    return &(*this)[num_items-1];
}

void UBVector::print_vector()
{
    for(size_t i = 0; i < num_items; i++)
        cout << (*this)[i] << " ";

    cout <<endl;
}

int* UBVector::insert(int* pPosition, const int val)
{
    if(num_items == current_capacity)
        reserve(current_capacity *2);

    for(int* i = end()+1; i > pPosition; --i)
    {

        *i = *(i-1);

        }

    *pPosition = val;
    num_items++;

    return pPosition;
}

int* UBVector::erase(int* pPosition)
{
    if((pPosition - begin()) < num_items)
    {

        for(int* i = pPosition; i < end(); i++)
            *i = *(i+1);

        item_ptr[num_items-1] = int();
        num_items--;
    }

    return &(*this)[*pPosition];
}

void UBVector::push_back(const int& val)
{
    if(num_items == current_capacity)
        reserve(current_capacity * 2);
    item_ptr[num_items++] = val;
}

void UBVector::pop_back()
{
    if(!empty())
    {
        erase(end());
    }

}

void UBVector::swap(UBVector& another)
{
    std::swap(num_items, another.num_items);
    std::swap(current_capacity, another.current_capacity);
    std::swap(item_ptr, another.item_ptr);
}

void UBVector::reserve(size_t n)
{
    if(n > current_capacity)
    {
        current_capacity = max(n, current_capacity);

        int * new_item_ptr = new int[current_capacity];

        for(size_t i = 0; i < num_items; i++)
        { new_item_ptr[i] = item_ptr[i]; }

        delete[] item_ptr;
        item_ptr = new_item_ptr;
    }
}


int& UBVector::front()
{
    return (*this)[0];
}

int& UBVector::back()
{
    return (*this)[num_items -1];
}


int& UBVector::at(size_t n)
{
    if(n < num_items)
        return (*this)[n];
    else
        out_of_range("index out of range");
}

int& UBVector::operator[](size_t n)
{
    if(n < num_items)
        return item_ptr[n];
    else
        throw out_of_range("index out of range");
}


