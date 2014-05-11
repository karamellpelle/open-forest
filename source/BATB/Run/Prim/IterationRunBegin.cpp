#include "BATB/Run/Prim/IterationRunBegin.hpp"
#include "BATB/Run.hpp"


namespace BATB
{


IterationRunBegin0* IterationRunBegin0::create(xml::XMLElement* elem)
{
    static IterationRunBegin0 ret;

    ret.next_ = IterationRunBegin1::create( elem );
    return &ret;
}


void IterationRunBegin0::destroy(IterationRunBegin0* iter)
{
    IterationRunBegin1::destroy( iter->next_ );
}

IterationRunBegin1* IterationRunBegin1::create(xml::XMLElement* elem)
{
    static IterationRunBegin1 ret;
   
    // FIXME: begin iteration from cfg...
    
    return &ret;
}

void IterationRunBegin1::destroy(IterationRunBegin1* iter)
{
    // FIXME: end iteration
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////
//




// to be run once, then calling IterationRunBegin1
void IterationRunBegin0::iterate(IterationStackRunWorld& stack, RunWorld& run)
{
    log << "IterationRunBegin0::iterate" << std::endl;

    theRun()->keys->keys_clear(); 
    theForest()->keys->keys_clear();
    // now next iteration
    //stack.next( next_ );
    stack.push( next_ );
}

void IterationRunBegin1::iterate(IterationStackRunWorld& stack, RunWorld& run)
{
    // begin Scene for this frame
    scene_begin( run.scene );
    
    theRun()->keys->keys_update( Env::tick() );
    theForest()->keys->keys_update( Env::tick() );
    
    float_t wth = run.scene.shape.wth;
    float_t hth = run.scene.shape.hth;


    KeyPointer* pointer = theForest()->keys->pointer;
    float_t x0, y0, x1, y1;
    tick_t ticks;
    if ( pointer->drag( run.scene.shape.wth, run.scene.shape.hth, x0, x1, y0, y1, ticks ) )
    {
        std::printf("drag (%#5.2f, %#5.2f) -> (%#5.2f, %#5.2f), time %#5.2f \n", x0, x1, y0, y1, ticks );
    }
    if ( pointer->drop( run.scene.shape.wth, run.scene.shape.hth, x0, x1, y0, y1, ticks ) )
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

