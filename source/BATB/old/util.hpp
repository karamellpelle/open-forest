#ifndef _UTIL_H
#define _UTIL_H

#include <iostream>
#include <ostream>
#include <algorithm>
#include <functional>
#include <vector>
#include <iterator>


typedef float fptype;


// return random number between min and max
fptype myrandom(fptype min, fptype max);

// seed the random number generator
void myrandomSeed(unsigned int seed);

// get my IP address
char* getMyIP(void);

// return the md5 sum of a string
std::string md5sum(const char* message);

#ifndef PI
#define PI 3.14159265358979323846
#endif

// full circle == 2*PI radians == 360 degrees
// convert radians to degrees
#define Rad2Deg(rad) ((rad)/PI * 180)
// convert degrees to radians
#define Deg2Rad(deg) ((deg)/180 * PI)

// from "The C++ Programming Language", third edition, pp. 530
// copy [first, last) to res iff pred p is true
template<class In, class Out, class Pred>
Out copy_if(In first, In last, Out res, Pred p)
{
    while (first != last)
    {
	if (p(*first))
	    *res++ = *first;
	++first;
    }
    return res;
}

template <class T, class T1>
class compare : public std::unary_function<T, bool>
{
    T1 id;
public:
    compare(T1 t) : id(t) {}
    bool operator()(T foo) const { return foo->id() == id; }
};

// adapter class for printing XML
template <class T>
class printXML : public std::unary_function<T, void>
{
    int indent;
    std::ostream& ostr;
 public:
    printXML(std::ostream& o = std::cout, int i = 0) : indent(i), ostr(o) {}
    void operator()(const T t) const 
    { 
	t->toXML(ostr, indent); 
    }
};

// adapter class
template <class T>
class Reshape : public std::unary_function<T, void>
{
    int x;
    int y;
    int width;
    int height;
 public:
    Reshape(int xpos, int ypos, int w, int h) : x(xpos), y(ypos), width(w), height(h) {}
    void operator()(const T t) const 
    { 
	t->reshape(x, y, width, height); 
    }
};

template <class T>
class PrintDialog : public std::unary_function<T, void>
{
 public:
    PrintDialog() {}
    void operator()(const T t) const 
    { 
	t->print(); 
    }
};

// adapter class for generating IDs
template <class T>
class generateID : public std::unary_function<T, void>
{
    std::ostream& ostr;
 public:
    generateID(std::ostream& o = std::cout) : ostr(o) {}
    void operator()(const T t) const 
    { 
	ostr << t->genID();
    }
};

// adapter class for drawing OpenGL 2D point
template <class T>
class _glVertex2f : public std::unary_function<T, void>
{
    //int indent;
    //std::ostream& ostr;
 public:
    _glVertex2f()  {}
    void operator()(const T t) const 
    { 
	t->_glVertex2f(); 
    }
};

template<class In, class Pred, class Fun>
void for_each_if(In first, In last, Pred pred, Fun fun)
{
    while (first != last)
    {
	if (pred(*first))
	    fun(*first);
	++first;
    }
}

template<class R, class T>
class compare_fun_t : public std::unary_function<T*, bool>
{
    R (T::*pmf)() const;
    R compareTo;
public:
    explicit compare_fun_t(R (T::*p)() const, R r) : pmf(p), compareTo(r) {}
    bool operator()(T* p) const { return (p->*pmf)() == compareTo; }
};

template<class R, class T> 
compare_fun_t<R, T> compare_fun(R (T::*p)() const, R r)
{
    return compare_fun_t<R, T>(p, r);
}

template<class T>
struct del_fun_t
{
    del_fun_t& operator()(T* p) 
    {
	delete p;
	return *this;
    }
};

template<class T>
del_fun_t<T> del_fun()
{
    return del_fun_t<T>();
}

#endif // _UTIL_H

