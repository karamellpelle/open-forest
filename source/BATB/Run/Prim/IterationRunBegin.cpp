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
    theScene()->size( wth, hth );

    theRun()->keys->keys_clear(); 
    theForest()->keys->keys_clear();

}


void IterationRunBegin1::iterate(IterationStackRunWorld& stack, RunWorld& run)
{
    // begin Scene for this frame
    scene_begin( theScene() );
    
    theRun()->keys->keys_update( Env::tick() );
    theForest()->keys->keys_update( Env::tick() );
    
    float_t wth = theScene()->shape()->wth;
    float_t hth = theScene()->shape()->hth;


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
        log << "double click ESC to quit." << std::endl;
    }

    if ( theRun()->keys->pause->click_double() )
    {
        stack.push();
    }
    else
    {
        stack.push( this );
    }

}


}

