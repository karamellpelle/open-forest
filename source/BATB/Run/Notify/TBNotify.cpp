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
#include "BATB/Run.hpp"
#include "BATB/Run/World.hpp"
#include "BATB/Run/Notify.hpp"
#include "BATB/Run/Notify/TBNotify.hpp"
#include "BATB/GUI.hpp"
#include "BATB/GUI/tb/helpers.hpp"
#include "BATB/Value/Run.hpp"
#include "BATB/Keys/Key.hpp"
#include "BATB/Time.hpp"



using namespace batb;
using namespace batb::run;

namespace tb
{

////////////////////////////////////////////////////////////////////////////////
// TBNotify
//

class TBNotifyMessageAnimation : public TBAnimationObject
{
public:
    TBNotifyMessageAnimation(TBNotifyMessage* t);

    enum class Type { Null, Begin, End };
    void type(Type t);

    void OnAnimationStart();
    void OnAnimationUpdate(float progress);
    void OnAnimationStop(bool aborted);


private:
    Type type_ = Type::Null;
    TBNotifyMessage* target_ = nullptr;

    PreferredSize pref_0_;
    PreferredSize pref_1_;
    LayoutParams lp_;

};


TBNotify::TBNotify(BATB* b) : tb::TBLayout( tb::AXIS_Y ), batb( b ), notify_( b->run->notify.get() )
{
    // set layout parameters
    // hardcoded, no .tb.txt here
    
    ////////////////////////////////////////////////////////////////
    // this is a tb::AXIS_Y layout
    // see tb_layout.h for help (this is difficult!)

    // which size should children have?
    // -> size: preferred
    SetLayoutSize( tb::LAYOUT_SIZE_PREFERRED );

    // which x position should children have?
    // -> position: center
    SetLayoutPosition( tb::LAYOUT_POSITION_CENTER );

    // which height should children have when distributing the children?
    // -> distribution: preferred
    SetLayoutDistribution( tb::LAYOUT_DISTRIBUTION_PREFERRED );

    // how should children be moved when distributing the children?
    // -> distribution position: top
    SetLayoutDistributionPosition( tb::LAYOUT_DISTRIBUTION_POSITION_LEFT_TOP );

    // since this is a non-interactive top widget spanning the whole screen,
    // make sure it does not steal input
    SetIsFocusable( false );
    SetIgnoreInput( true );

}




void TBNotify::step(World& run)
{
    auto wth = run.scene.wth;
    auto hth = run.scene.hth;

    // span out this layout to the whole screen
    SetRect( tb::TBRect(0, 0, wth, hth) );

    tick_t tick = batb->time->get();
    

    auto i = std::begin( tb_notify_messages_ );
    while ( i != std::end( tb_notify_messages_ ) )
    {
        TBNotifyMessage* tbmsg = *i;
        bool remove = false;

        auto message = tbmsg->notifymessage;
        tick_t ts = message->tick + message->duration;

        // duration specifies minimum time
        if ( ts <= tick ) 
        {
            if ( message->key )
            {
                if ( message->key->is_down() )
                {
                    remove = true;
                }
            }
            else
            {
                remove = true;
            }
        }
        
        // remove if this message is up and running
        // FIXME: make sure message is up
        if ( tbmsg->isActive() && remove )
        {
            // remove the actual message from Notify
            message->finish();

            // TBWidget::Die() calls delete for us unless a TBWidgetListener stops that process
            // we stop the process, animate removal, remove it from TBNotify, and deletes it
            // if we not let a TBWidgetListener stop the process, we must remove it from TBNotify
            // first, otherwise we get an assertion error.
            //
            // another way to do it is to create an animation which calls Die() on end, cf.
            // TBWidgetAnimationOpacity
            tbmsg->Die(); 

            // erase our reference. it's still a child of this TBLayout.
            i = tb_notify_messages_.erase( i );    
        }
        else
        {
            ++i;
        }
    }

}


void TBNotify::push(NotifyMessage* m)
{
    constexpr const char* path = "static://BATB/Run/notifymessage.tb.txt";  

    // read widget from TBNode in file.
    TBNode node;
    if ( node.ReadFile( path ) )
    {
        // find a TBNotifyMessage definition in file
        for (TBNode* child = node.GetFirstChild(); child; child = child->GetNext())
        {
              if ( std::string( "TBNotifyMessage" ).compare( child->GetName() ) == 0 )
              {
                  // TODO: look at id and compare to NotifyMessage type (warning, tip, etc, new chat, song etc).

                  auto ptr = new_widget<TBNotifyMessage>( this, child );

                  // witdh and height are defined in the layout parameters in .tb.txt
                  // here is how:
                  //tb::LayoutParams lp;
                  //lp.min_w = lp.max_w = lp.pref_w = 440;
                  //lp.min_h = 80;
                  //SetLayoutParams( lp );

                  // pass arguments to TBNotifyMessage (has empty constructor)
                  ptr->tb_notify = this;
                  ptr->message( m );

                  // keep reference to our new TBNotifyMessage
                  tb_notify_messages_.push_back( ptr );
                  
                  // child created, all done
                  return;

              }
        }
        batb->log << "TBNotify: no TBNotifyMessage defined in " << path << std::endl;
    }
    else
    {
        batb->log << "TBNotify: could not read " << path << std::endl;
    }

    

    
    
}

////////////////////////////////////////////////////////////////////////////////
// TBNotifyMessage


// without this, TBNotify is never read (g_widgets_reader->LoadNodeTree( this, &node )):
TB_WIDGET_FACTORY(TBNotifyMessage, TBValue::TYPE_STRING, WIDGET_Z_TOP) { }


TBNotifyMessage::TBNotifyMessage()
{
    // listen to itself: what happens - born og dying?
    AddListener( this );
}

TBNotifyMessage::~TBNotifyMessage()
{
    // listen to itself: what happens - born og dying?
    RemoveListener( this );
}

void TBNotifyMessage::begin()
{
    // remove from TBNotify
    active_ = true;

}

void TBNotifyMessage::end()
{
    // remove from TBNotify
    auto* parent = GetParent();
    parent->RemoveChild( this );

    // :)
    delete this;
}

bool TBNotifyMessage::isActive() const
{
    return active_; 
}

void TBNotifyMessage::OnInflate(const INFLATE_INFO &info)
{
    TBWidget::OnInflate( info );

    // read custom properties

    // read fade in/out ticks
    time_fadein = info.node->GetValueFloat( "fade-in", time_fadein );
    time_fadeout = info.node->GetValueFloat( "fade-out", time_fadeout );

    // since 'OnAdded()' is called before 'OnInflate()', we have to start animation
    // here, after time properties has been read
    auto anim = new TBNotifyMessageAnimation( this );
    anim->type( TBNotifyMessageAnimation::Type::Begin );

    TBAnimationManager::StartAnimation( anim, ANIMATION_CURVE_SMOOTH, time_fadein );

}


void TBNotifyMessage::OnWidgetAdded(TBWidget *parent, TBWidget *child)
{
    //std::cout << "widget added :)" << std::endl;
    
}
bool TBNotifyMessage::OnWidgetDying(TBWidget* w)
{
    //std::cout << "widget is dying!" << std::endl;

    // this signalizes message handled, and prevents deletion of widget
    return true;
}

void TBNotifyMessage::OnDie()
{
    active_ = false;

    // start Die animation
    auto anim = new TBNotifyMessageAnimation( this );
    anim->type( TBNotifyMessageAnimation::Type::End );

    TBAnimationManager::StartAnimation( anim, ANIMATION_CURVE_SMOOTH, time_fadeout );

}

void TBNotifyMessage::OnAdded()
{
    //std::cout << "widget OnAdded()" << std::endl;
    // start born animation
    // NOTE: this has been moved into OnInflate() since this callback OnAdded()
    // is called before OnInflate(). we need the 'time_fadein' property.
}

void TBNotifyMessage::message(NotifyMessage* msg)
{
    
    // here it would be nice to have a global variable 'batb'!
    auto batb = tb_notify->batb;

    // what this represents
    notifymessage = msg;

    if ( ( tb_editfield = GetWidgetByIDAndType<TBEditField>( TBIDC( "edit" ) ) ) )
    {
        // set text
        tb_editfield->SetText( msg->str.c_str() );

        // SetTextAlign only works on selections, or TBBlock (i.e. line) of 
        // current position (i.e. beginning), see tb_style_edit.h . 
        // hence we have to select all before aligning
        tb_editfield->GetStyleEdit()->selection.SelectAll();
        tb_editfield->GetStyleEdit()->SetAlign( TB_TEXT_ALIGN_CENTER );
        tb_editfield->GetStyleEdit()->selection.SelectNothing();

    }
    else
    {
        batb->log << "TBNotifyMessage: 'edit' not defined\n";
    }

}


////////////////////////////////////////////////////////////////////////////////
// TBNotifyMessageAnimation
//

TBNotifyMessageAnimation::TBNotifyMessageAnimation(TBNotifyMessage* t) : target_( t )
{

}

void TBNotifyMessageAnimation::OnAnimationStart()
{
    // TBRect:        current size (and position relative to parent)
    // LayoutParams:  the size behaviour
    // PreferredSize: the actual, computed preferred size. 

    pref_0_ = target_->GetPreferredSize();  
    pref_1_ = target_->GetPreferredSize();  
    // ^ TODO: use GetRect()?
    lp_ = *target_->GetLayoutParams();  

    if ( type_ == Type::Begin )
    {
        // start at 0 opacity
        target_->SetOpacity( 0.0 );

        // 0 -> 1
        pref_0_.pref_h = 0;
    }
    if ( type_ == Type::End )
    {
        // 0 -> 1
        pref_1_.max_h = 0;

    }

}

void TBNotifyMessageAnimation::OnAnimationUpdate(float a)
{
    float h = 0.0;

    if ( type_ == Type::Begin )
    {
        float h0 = (float)( pref_0_.pref_h );
        float h1 = (float)( pref_1_.pref_h );
        h = (1.0f - a) * h0 + a * h1;

        target_->SetOpacity( a );
    }
    if ( type_ == Type::End )
    {
        float h0 = (float)( pref_0_.max_h );
        float h1 = (float)( pref_1_.max_h );
        h = (1.0f - a) * h0 + a * h1;

        target_->SetOpacity( 1.0f - a );
    }

    // set height
    lp_.SetHeight( (int)( h ) );
    target_->SetLayoutParams( lp_ ); // <- this calls InvalidateLayout


}

void TBNotifyMessageAnimation::OnAnimationStop(bool aborted)
{

    if ( type_ == Type::Begin )
    {
        target_->SetOpacity( 1.0 );

        // the target is offically up and running
        target_->begin();
    }
    if ( type_ == Type::End )
    {
        // all over
        target_->end();
        target_ = nullptr; 
    }
}

void TBNotifyMessageAnimation::type(Type t)
{
    type_ = t;
}



} // namespace tb

