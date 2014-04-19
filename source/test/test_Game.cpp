#include "Game.hpp"
#include <iostream>
#include <string>
#include <unistd.h>



class A
{
friend std::ostream& operator<<(std::ostream& , const A& );

public:
    A(uint n) : count_(n) { } 

    void modify(uint n)
    {
        count_ += n;
    }

private:
    uint count_;
};

std::ostream& operator<<(std::ostream& os, const A& a)
{
    return os << "A: " << a.count_;
}

    typedef Game::IterationStack<A> IterationStackA;
    typedef Game::Iteration<A> IterationA;
    typedef Game::IterationFunction<A> IterationFunctionA;
    typedef Game::Ref Ref;
    typedef Game::Step<A> StepA;

void f(IterationA* this_, IterationStackA& stack, A& a);
void g(IterationA* this_, IterationStackA& stack, A& a);
void h(IterationA* this_, IterationStackA& stack, A& a);


class MyIteration : public IterationA
{
public:
    void iterate(IterationStackA& stack, A& a) { std::cout << "MyIteration" << std::endl; }
};

/*
class MyStep : public StepA
{
public:
    void stepWorld(IterationStackA& stack, A& a)
    {
        static uint ix = 2;
        if ( --ix == 0 )
        {
            stack.push();
        }
        else
        {
            stack.push( iteration_ );
        }
    }

};
*/
int main(int argc, char** argv)
{
    A a( 100 );
    //MyStep mystep;

    IterationStackA stack;
    stack.push( IterationStackA::releasing( IterationFunctionA::create( f ) ),
                IterationStackA::releasing( IterationFunctionA::create( g ) )
              );
    
    while ( !stack.empty() )
    {
        stack.iterate( a );
        usleep( 1500000 );
    }

    
    
    std::cout << "Stack complete.\n"
              << a
              << std::endl;
}



void f(IterationA* this_, IterationStackA& stack, A& a)
{
    static uint i = 4;

    // output
    std::cout << "f: " << a << " ( i == " << i << " ) " << std::endl;

    // step

    // do
    a.modify( 1 );

    // think
    if ( --i == 0 )
    {
        stack.push();
    }
    else
    {
        stack.push( this_ );
    }
}

void g(IterationA* this_, IterationStackA& stack, A& a)
{
    static uint j = 2;

    // output
    std::cout << "g: " << a << " ( j == " << j << " ) " << std::endl;

    // step

    // do
    a.modify( 1 );

    // think
    if ( --j == 0 )
    {
        stack.push( IterationStackA::releasing( IterationFunctionA::create( h ) ) );
    }
    else
    {
        stack.push( this_ );
    }

}


void h(IterationA* this_, IterationStackA& stack, A& a)
{
    std::cout << "h: " << a << std::endl;

    a.modify( 1 );

    stack.push();
}

