#include "Game.hpp"
#include <iostream>
#include <string>
#include <unistd.h>

#include <stdint.h>
typedef uint_fast32_t uint;

class A
{
friend std::ostream& operator<<(std::ostream& , const A& );

public:
    A(uint n) : count_(n) { } 
    
    void modify(uint n)
    {
        count_ += n;
    }

    void append(const std::string& str) { str_ += str; }
    std::size_t strsize() const { return str_.size(); }
    const std::string& string() const { return str_; }

private:
    uint count_;
    std::string str_;
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
    typedef Game::IterationDefault<A> IterationDefaultA;

void f(IterationA* this_, IterationStackA& stack, A& a);
void g(IterationA* this_, IterationStackA& stack, A& a);
void h(IterationA* this_, IterationStackA& stack, A& a);


class MyScoopIteration  : public IterationA
{
public:
    MyScoopIteration() : count_( 2 ) { }
    void iterate(IterationStackA& stack, A& a)
    {
        // output
        std::cout << "MyIteration" << std::endl;

        // do
        --count_;
        
        // think
        if ( count_ == 0 )
        {
            stack.push();
        }
        else
        {
            stack.push( this );
        }
    }
private:
    uint count_;
};


class MyStep : public Game::Step<A>
{
public:
    static MyStep* create() { return new MyStep(); }
    static void destroy(MyStep* step) { step->destroy(); }

    void stepWorld(IterationA* this_, IterationStackA& stack, A& a)
    {
        // do 
        a.append( "*" );

        // think
        if ( 5 <= a.strsize() )
        {
            stack.push();
        }
        else
        {
            stack.push( this_ );
        }
    }

private:
    MyStep() : count_( 4 ) { } 
    void destroy()
    {
        delete this;
    }

    uint count_;
};



class MyOutput : public Game::Output<A>
{
public:
    static MyOutput* create(std::ostream& os) { return new MyOutput(os); }
    static void destroy(MyOutput* out) { out->destroy(); }

    void outputWorld(A& a)
    {
        os_ << a.string() << std::endl;
    }
private:
    MyOutput(std::ostream& os) : os_( os ) { }
    void destroy() { delete this; }

    std::ostream& os_;
};



int main(int argc, char** argv)
{
    A a( 100 );

    IterationStackA stack;

    MyScoopIteration scoop_iter;

    stack.push( Ref::releasing( IterationFunctionA::create( f ) ),
                Ref::releasing( IterationFunctionA::create( g ) ),
                &scoop_iter, 
                Ref::releasing( IterationDefaultA::create(
                                            Ref::releasing( MyOutput::create( std::cout ) ),
                                            Ref::releasing( MyStep::create() ) ) )

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
        stack.push( Ref::releasing( IterationFunctionA::create( h ) ) );
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

