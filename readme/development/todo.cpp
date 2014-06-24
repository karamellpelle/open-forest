
// set callback => use KEys for this
// backend init:
//  * set up GLFW, set callbacks
//  * create TBRenderer
// tb_core_init: init TB
// set skin
// register font renderer, and init that
// set skin background for RootWidget
// DemoApplication::Init: 
//  * Application::Init: TBWidgetsAnimationManager::Init();
//  * block animations
//  * run tests
//  * create various windows
// DemoApplication::Run == Application::Run
//    while ( run )
//        if ( backend::has_pending_update) window_refresh_callback

// DemoApplication::Shutdown == Application::Shutdown
//    TBWidgetsAnimationManager::Shutdown();
//    tb_core_shutdown
//    delete renderer
//

// window_refresh_callback
//  * Application::Process:
//      TBAnimationManager::Update();
//      GetRoot()->InvokeProcessStates();
//      GetRoot()->InvokeProcess();
//
//  * Application::RenderFrame:
//      g_renderer->BeginPaint(window_w, window_h);
//      GetRoot()->InvokePaint(TBWidget::PaintProps());
//      g_renderer->EndPaint();
//      
//      // If we want continous updates or got animations running, reinvalidate immediately
//      if (continuous_repaint || TBAnimationManager::HasAnimationsRunning())
//          GetRoot()->Invalidate();
//      

// nice stuff: draw directly to screen
//GetRoot()->GetFont()->DrawString(5, 5, TBColor(255, 255, 255), str);
// widget opacity:
//   SetOpacity(alpha)
// AddChild/RemoveChild
// GetWidgetAt(x, y)
// ConvertTo/FromRoot(x,y)
// Invalidate()/OnInvalid(): widget not valid now
// Die: "animated deletion of widget" (?)
// lots of static variables for TBWidget: focused_widget, ...
//
// Root
// InvokeEvent/PointerDown/PointerUp/PointerMove/Wheel/Key
// SetRect(0, 0, wth, hth)

// File structure:
//

batb/BATB.xml
batb/gui/GUI.xml
         skins/
         resources/
         ...
batb/run/Run.xml
batb/run/Keys.xml


// NOTE: Scene part of run::World! (multiple scenes per game instance)
//
namespace batb
{

class BATB
{
public
   run::Run           run; 
   forest::Forest     forest;
   xml::XML           xml;
   gui::GUI           gui;
   keys::Keys         keys;
   players::Players   players;
   values::Values     values;
}

class GUI
{
public:
    GUI(BATB& b) : batb( b )
    {
    }
    

    BATB& batb;
    xml::XMLElement xml;
    
};

class Run
{
    Run(BATB& b) : batb( b )
    {
    }
    
    BATB& batb;
    IterationRunBegin iterationRunBegin;
    IterationRunMain  iterationRunMain;
};


batb::gui::create(batb::gui::GUI& gui)
{
    xml
}

batb::gui::destroy(batb::gui::GUI& gui);


class batb::run::RootWidget : batb::gui::RootWidget
{
    // parse
    // callbacks, helpers
    // setup screen
};

};





// char_callback
//    if InvokeShortcut
//        send shortcut event to focused widget (InvokeEvent)
//    else
//        send key event to widget (InvokeKey)
// key_callback
//    convert GLFW into TB, InvokeKey
// mouse_button_callback
//    InvokePointerDown/Up, or InvokeEvent if context menu
// cursor_position_callback
//    InvokePointerMove
//  scroll_callback
//    InvokeWheel
//  window_size_callback
//    SetRect(0, 0, wth, hth)
//  timer_callback
//    ??
//  drop_callback
//    send drop event, InvokeEvent
//

// FIXME
// TBMessageHandler::ProcessMessages, timer_callback() ???
//
        // this is our program configuration
        batb::BATB batb( File::dynamicData( "BATB.xml" );
        batb::xml::XMLHandle batb.xml.FirstChildElement( "BATB" );
       
        // modify from command line, take actions
        if ( int ret = cmdline( argc, argv, batb ) )
        {
            return ret;
        }

        // init Env, from configuration
        env::begin( batb.xml.FirstChildElement("Env").ToElement() );

        //////////////////////////////////////////////////////////
        //      OpenGL

        //////////////////////////////////////////////////////////
        //      OpenAL


        using namespace BATB;


        // init BATB.
        // this creates only the necessary part of Run, and the rest
        // is created by 'iterationRunBegin'
        batb::begin( batb );

        batb::run::World run;
        game::IterationStack<batb::BATB, batb::run::World> stack;

        stack.push( batb.run.iterationRunBegin,
                    batb.run.iterationRunEnd    );
        

        batb.log.info << "Game::IterationStack<RunWorld> starting. " << std::endl;
        batb.log.error << "" << std::endl;

        // "main loop"
        while ( !stack.empty() )
        {
            // begin frame for iteration
            env::frame_begin();

            // make 1 iteration of RunWorld:
            stack.iterate( run ); 

            // end frame for iteration (swap buffers, poll events)
            Env::frame_end();
        }

        batb.log.info << "Game::IterationStack<RunWorld> empty. " << std::endl;
      
        batb::end( batb ); // end this object
    }
    catch (std::exception& e)
    {
        std::cerr << "main:    FATAL: " << e.what() << std::endl;
        ret = 1;
    }


    // end Env
    env::end();


    // end BATB
    batb::end(); // end all BATB-objects



    // quit...
    return ret;

}





