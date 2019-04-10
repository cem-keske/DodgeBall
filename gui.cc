/**
 * file: map.h
 * 
 * authors:	Cem Keske
 * 			Emre Yazici
 */

#include "gui.h"
#include <iostream>
#include <cairomm/context.h>

using namespace std;

MyArea::MyArea(): empty(false)
{
}

MyArea::~MyArea()
{
}

void MyArea::clear()
{
  empty = true; 
  refresh();
}

void MyArea::draw()
{
  empty = false;
  refresh();
}

void MyArea::refresh()
{
  auto win = get_window();
  if(win)
  {
	  Gdk::Rectangle r(0,0, get_allocation().get_width(), get_allocation().get_height());
	  win->invalidate_rect(r,false);
  }
}

bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  if(not empty)
  {
	  Gtk::Allocation allocation = get_allocation();
	  const int width = allocation.get_width();
	  const int height = allocation.get_height();
	
	  // coordinates for the center of the window
	  int xc, yc;
	  xc = width / 2;
	  yc = height / 2;
	
	  cr->set_line_width(10.0);
	
	  // draw red lines out from the center of the window
	  cr->set_source_rgb(0.8, 0.0, 0.0);
	  cr->move_to(0, 0);
	  cr->line_to(xc, yc);
	  cr->line_to(0, height);
	  cr->move_to(xc, yc);
	  cr->line_to(width, yc);
	  cr->stroke();
  }
  else
  {
	  cout << "Empty !" << endl;
  }

  return true;
}


//--------------------------------------

MyEvent::MyEvent() :
  m_Box(Gtk::ORIENTATION_VERTICAL,10),
  m_Box_Top(Gtk::ORIENTATION_HORIZONTAL, 10),
  m_Box_Bottom(Gtk::ORIENTATION_HORIZONTAL, 10),
  m_Button_Clear("Clear"),
  m_Button_Draw("Draw")
{
  // Set title and border of the window
  set_title("Drawing Area and Buttons");
  set_border_width(0);
  
  // Add outer box to the window (because the window
  // can only contain a single widget)
  
  add(m_Box);

  //Put the inner boxes and the separator in the outer box:
  m_Box.pack_start(m_Box_Top);
  m_Box.pack_start(m_Box_Bottom);
  
  m_Area.set_size_request(200,200);
  m_Box_Top.pack_start(m_Area);
  
  m_Box_Bottom.pack_start(m_Button_Clear,false,false);// keep fixed width
  m_Box_Bottom.pack_start(m_Button_Draw,false,false); // and aligned to left;  
  
  // Connect the clicked signal of the button to
  // their signal handler
  m_Button_Clear.signal_clicked().connect(sigc::mem_fun(*this,
              &MyEvent::on_button_clicked_clear) );

  m_Button_Draw.signal_clicked().connect(sigc::mem_fun(*this,
              &MyEvent::on_button_clicked_draw) );

  // Show all children of the window
  show_all_children();
}

MyEvent::~MyEvent()
{
}

void MyEvent::on_button_clicked_clear()
{
  cout << "Clear" << endl;
  m_Area.clear();
}

void MyEvent::on_button_clicked_draw()
{
  cout << "Draw" << endl;
  m_Area.draw();
}
