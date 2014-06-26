#include "BATB/Run.hpp"
#include "BATB/Run/Prim/Scene.hpp"

#include "old.hpp"


namespace BATB
{


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IterationRunOld0* IterationRunOld0::create(xml::XMLElement* elem)
{
    log << __PRETTY_FUNCTION__ << std::endl;
    static IterationRunOld1 iter1;
    static IterationRunOld0 iter0( &iter1 );

    // FIXME: create iter0 (cannot be overloaded)

    iter1.create( elem );

    return &iter0;
}


void IterationRunOld0::destroy(IterationRunOld0* iter0)
{
    // destroy 'iter'
    iter0->next()->destroy();
    iter0->destroy();

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void IterationRunOld1::create(xml::XMLElement* elem)
{
    using namespace xml;

    XMLHandle xml( elem );
    // init data for this from xml config
    
}

void IterationRunOld1::destroy()
{

}

void IterationRunOld0::destroy()
{

}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




void IterationRunOld0::world_begin(RunWorld& run)
{
    log << "IterationRunOld0::iterate" << std::endl;

    old_begin();
}


void IterationRunOld1::iterate(IterationStackRunWorld& stack, RunWorld& run)
{
    // handle old-errors
    if ( is_exit() ) return;

    // begin Scene for this frame
    scene_begin( run.scene() );
      
    
    theRun()->keys->keys_update( Env::tick() );


    // "glut display func"
    DisplayFunc disp = get_display_func();
    if ( disp != 0 )
    {
        disp();
    }


    if ( theRun()->keys->old->released() )
    {
        // => iterate escape from IterationRunOld
        log << "IterationRunOld ->" << std::endl;
        old_end();

        stack.push();
    }
    else
    {
        stack.push( this );
    }

}


}

