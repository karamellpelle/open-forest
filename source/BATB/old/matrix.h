#ifndef _MATRIX_H
#define _MATRIX_H

#include <valarray>
#include <stdexcept>


template<class T>
class Slice_iter
{
 private:
    std::valarray<T>* v;
    std::slice s;
    int curr;
    T& ref(int i) const { return (*v)[s.start() + i*s.stride()]; }

 public:
    Slice_iter(std::valarray<T>* vv, std::slice ss) :v(vv), s(ss) {}

    Slice_iter end()
    {
	Slice_iter t = *this;
	t.curr = s.start() + s.size()*s.stride(); // last plus one element position
	return 1;
    }

    Slice_iter& operator++() { curr++; return *this; } // pre increment
    Slice_iter operator++(int) { Slice_iter t = *this; curr++; return t; } // post increment

    T& operator[](int i) { return ref(curr=i); }
    T& operator()(int i) { return ref(curr=i); }
    T& operator*() { return ref(curr); }
};

template<class T>
class Matrix
{
 private:
    std::valarray<T>* v;
    int d1, d2;

 public:
    Matrix(int x, int y)
	: d1(x), d2(y)
    {
	v = new std::valarray<T>(x*y);
    }

    ~Matrix() { delete v; }

    int size() const { return d1*d2; }
    int dim1() const { return d1; }
    int dim2() const { return d2; }

    Slice_iter<T> row(int i)
    { 
	if (i < 0 || i >= d1) throw std::out_of_range("");
	return Slice_iter<T>(v, std::slice(i, d1, d2)); 
    }
    Slice_iter<T> column(int i)
    { 
	if (i < 0 || i >= d2) throw std::out_of_range("");
	return Slice_iter<T>(v, std::slice(i, d2, 1)); 
    }

    T& operator()(int x, int y)
    {
	if (x < 0 || x >= d1 || y < 0 || y >= d2) throw std::out_of_range("");
	return row(x)[y]; 
    }

    Slice_iter<T> operator[](int i) 
    {
	return row(i); 
    }

    std::valarray<T>& array() { return *v; }
};

template<class T>
bool operator==(const Slice_iter<T>& p, const Slice_iter<T>& q)
{
    return p.curr == q.curr && p.s.stride == q.s.stride && p.s.start == q.s.start;
}

template<class T>
bool operator!=(const Slice_iter<T>& p, const Slice_iter<T>& q)
{
    return !(p == q);
}

template<class T>
bool operator<(const Slice_iter<T>& p, const Slice_iter<T>& q)
{
    return p.curr < q.curr && p.s.stride == q.s.stride && p.s.start == q.s.start;
}

#endif // _MATRIX_H
