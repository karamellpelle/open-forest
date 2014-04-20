#include "include.hpp"



int main(int argc, char** argv)
{
    
    // init Env
    Env::begin();

    // set up environment (GL, ...)
    // ...


    using namespace BATB;

    // init BATB,
    // i.e. create resourceRunData. ForestData created by 'iterationDataBegin'...
    BATB::begin();

    RunWorld run;
    Game::IterationStack<RunWorld> stack;

    stack.push( resourceRunData()->prim->iterationDataBegin,    // create game data
                resourceRunData()->prim->iterationIntro,        // game intro
                resourceRunData()->prim->iterationMain,         // "main menu"
                resourceRunData()->prim->iterationOutro,        // game outro
                resourceRunData()->prim->iterationDataEnd       // destroy game data
              );

    BATB::log << "Game::IterationStack<RunWorld> starting. " << std::endl;

    // "main loop"
    while ( !stack.empty() )
    {
        // make 1 iteration of RunWorld:
        stack.iterate( run ); 
    }

    BATB::log << "Game::IterationStack<RunWorld> empty. " << std::endl;

    // end BATB
    BATB::end();

    Env::end();
}
