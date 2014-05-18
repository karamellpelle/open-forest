#include "BATB/Run.hpp"
#include "BATB/Run/Prim/Scene.hpp"


namespace BATB
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IterationRunMain0* IterationRunMain0::create(xml::XMLElement* elem)
{
    log << __PRETTY_FUNCTION__ << std::endl;
    static IterationRunMain1 iter1;
    static IterationRunMain0 iter0( &iter1 );

    // FIXME: create iter0 (cannot be overloaded)

    iter1.create( elem );

    return &iter0;
}


void IterationRunMain0::destroy(IterationRunMain0* iter0)
{
    // destroy 'iter'
    iter0->next()->destroy();
    iter0->destroy();

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void IterationRunMain1::create(xml::XMLElement* elem)
{
    using namespace xml;

    XMLHandle xml( elem );
    // init data for this from xml config
    
}

void IterationRunMain1::destroy()
{

}

void IterationRunMain0::destroy()
{

}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




void IterationRunMain0::world_begin(RunWorld& run)
{
    log << "IterationRunMain0::iterate" << std::endl;

    // create Scene
    uint wth, hth;
    Env::screenSize( wth, hth );
    run.scene()->size( wth, hth );

    theRun()->keys->keys_clear(); 

    log << std::endl << "\n\npress INSERT to toggle old-BATB..." << std::endl;
    log << "click ESCAPE to exit from Main...\n\n" << std::endl;
}


void IterationRunMain1::iterate(IterationStackRunWorld& stack, RunWorld& run)
{
    // begin Scene for this frame
    scene_begin( run.scene() );
    
    theRun()->keys->keys_update( Env::tick() );
    
    float_t wth = run.scene()->shape()->wth;
    float_t hth = run.scene()->shape()->hth;


    KeyPointer* pointer = theForest()->keys->pointer;
    float_t x0, y0, x1, y1;
    tick_t ticks;
    if ( pointer->drag( wth, hth, x0, x1, y0, y1, ticks ) )
    {
        std::printf("drag (%#5.2f, %#5.2f) -> (%#5.2f, %#5.2f), time %#5.2f \n", x0, x1, y0, y1, ticks );
    }
    if ( pointer->drop( wth, hth, x0, x1, y0, y1, ticks ) )
    {
        std::printf("drop (%#5.2f, %#5.2f) -> (%#5.2f, %#5.2f), time %#5.2f \n", x0, x1, y0, y1, ticks );
    }

    float_t x;
    float_t y;
    if ( pointer->right_pressed( wth, hth, x, y ) )
    {
        std::printf("right down: (%#5.2f, %#5.2f) \n", x, y );
    }
    if ( pointer->right_released( wth, hth, x, y ) )
    {
        std::printf("right up:   (%#5.2f, %#5.2f) \n", x, y );
    }



    if ( theRun()->keys->pause->click() )
    {
        log << "IterationRunMain ->" << std::endl;
        stack.push();
        return;
    }

    // run old-BATB if INSERT released
    if ( theRun()->keys->old->released() )
    {
        log << "IterationRunMain -> IterationRunOld" << std::endl;
        stack.push( theRun()->prim->iterationRunOld, this );
        return;
    }


    // if no other stack returns, continue with this
    stack.push( this );

}


}

