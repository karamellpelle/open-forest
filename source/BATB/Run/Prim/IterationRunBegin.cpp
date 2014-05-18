#include "BATB/Run.hpp"
#include "BATB/Run/Prim/Scene.hpp"


namespace BATB
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IterationRunBegin0* IterationRunBegin0::create(xml::XMLElement* elem)
{
    log << __PRETTY_FUNCTION__ << std::endl;
    static IterationRunBegin1 iter1;
    static IterationRunBegin0 iter0( &iter1 );

    // FIXME: create iter0 (cannot be overloaded)

    iter1.create( elem );

    return &iter0;
}


void IterationRunBegin0::destroy(IterationRunBegin0* iter0)
{
    // destroy 'iter'
    iter0->next()->destroy();
    iter0->destroy();

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void IterationRunBegin1::create(xml::XMLElement* elem)
{
    using namespace xml;

    XMLHandle xml( elem );
    // init data for this from xml config
    
}

void IterationRunBegin1::destroy()
{

}

void IterationRunBegin0::destroy()
{

}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




void IterationRunBegin0::world_begin(RunWorld& run)
{
    log << "IterationRunBegin0::iterate" << std::endl;

    // create Scene
    uint wth, hth;
    Env::screenSize( wth, hth );
    run.scene()->size( wth, hth );

    theRun()->keys->keys_clear(); 

}


void IterationRunBegin1::iterate(IterationStackRunWorld& stack, RunWorld& run)
{
    // begin Scene for this frame
    scene_begin( run.scene() );
    
    theRun()->keys->keys_update( Env::tick() );


    // nothing to do yet, continue with Main
    stack.push( theRun()->prim->iterationRunMain );

}


}

