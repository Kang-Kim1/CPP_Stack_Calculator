#ifndef _UBVECTOR_H
#define _UBVECTOR_H
#include <string>

using namespace std;

class UBVector
{
    public:
        // constructor, destructor

        UBVector(const UBVector& another);
        explicit UBVector(size_t n, int val);
        UBVector (int* pBegin, int*  pEnd);
        ~UBVector();

        // = operator
        UBVector& operator=(const UBVector& x);

        // modifier
        size_t size() const;
        size_t capacity() const;
        bool empty();

        // accessor
        int *begin();
        int *end();

        // modifier
        int* insert(int* pPosition, const int val);
        int* erase(int* pPosition);
        void push_back(const int& val);
        void pop_back();
        void swap(UBVector& another);
        void reserve(size_t n);

        int& at(size_t n);
        int& operator[](size_t n);

        int& front();
        int& back();

        void print_vector();


        typedef int* iterator;


    private:
        size_t num_items;
        size_t current_capacity;
        static const size_t INITIAL_CAPACITY;
        int* item_ptr;
};

#endif // UBVECTOR_H
