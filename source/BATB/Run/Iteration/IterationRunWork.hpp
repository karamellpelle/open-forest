//    open-forest: an orientering game.
//    Copyright (C) 2018  karamellpelle@hotmail.com
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
#ifndef BATB_RUN_ITERATION_ITERATIONRUNWORK_HPP
#define BATB_RUN_ITERATION_ITERATIONRUNWORK_HPP
#include "BATB/Run/Iteration/IterationRun.hpp"
#include "BATB/ThreadGLContext.hpp"


namespace batb
{


class BATB;

namespace run
{


class RunWorkTBWidget;

////////////////////////////////////////////////////////////////////////////////
//  

class WorkItem
{
friend class IterationRunWork;
friend class Work;
private:
    enum class Mode { Empty, Definite, Indefinite };
    Mode mode = Mode::Empty;
    std::string tag;
    float_t alpha = 0.0;
    // tick_t tick = 0; // to be used for indefinites!

};

////////////////////////////////////////////////////////////////////////////////
//  

class Work : private ThreadGLContext<WorkItem>
{
friend class IterationRunWork; 
public: 
    void definite(uint );
    void indefinite();
    void state(const std::string& , float_t );
    void state(const std::string& );
    void state();
    void finish();
    

private:
    using WorkFunction = std::function<void(Work& )>;

    Work(BATB* b) : batb( b ) { }
    void work(WorkFunction f) { function_ = f; }

    void run() override;


    BATB* batb;
    WorkFunction function_;
    WorkItem::Mode mode_;
    uint n_ = 0;
    uint ns_ = 0;

};


// load various stuff 
class IterationRunWork : public IterationRun
{
public:
    using WorkFunction = std::function<void(Work& )>;

    // construct from the containing Run object
    IterationRunWork(BATB* );
    //IterationRunWork(BATB* b, WorkFunction f) : IterationRunWork( b ) { work( f ); }

    template <typename F>
    IterationRunWork(BATB* b, const F& f) : IterationRunWork( b ) { work( WorkFunction( f ) );  }
    template <class Clazz, class Method>
    IterationRunWork(BATB* b, Clazz& c, Method& m) : IterationRunWork( b, std::bind( m, c ) ) { }
    ~IterationRunWork();

    ////////////////////////////////////////
    // iterate
    IterationStack iterate_run(World& world) override;

    // setup before iteration
    void iterate_begin(World& );


    ////////////////////////////////////////

    // set job to work on
    void work(WorkFunction );
    template <class Clazz, class Method>
    void work(Clazz& c, Method& m) { work( std::bind( m, c ) ); }


private:
    void output(run::World& );

    Work work_;
    WorkItem* item_ = nullptr;

    RunWorkTBWidget* tb_widget_ = nullptr;

};

////////////////////////////////////////////////////////////////////////////////
//
//

// convenient name for class used for loading something
template <typename Type>
class LoadWorker
{
public:
    void operator()(Work& ) { }

};


// convenient name for class used for unloading something
template <typename Type>
class UnloadWorker
{
public:
    void operator()(Work& ) { }

};





} // namespace run

} // namespace batb

#endif
