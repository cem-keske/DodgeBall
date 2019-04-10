/**
 * file: map.h
 * 
 * authors:	Cem Keske
 * 			Emre Yazici
 */
#include "define.h"
#include "gui.h"
#include <iostream>
#include <cairomm/context.h>


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
  
}

bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  

  return true;
}


//--------------------------------------

MyEvent::MyEvent() :
	frame(Gtk::ORIENTATION_VERTICAL,10),
	//interaction_box(Gtk::ORIENTATION_HORIZONTAL, 10),
	canvas(Gtk::ORIENTATION_HORIZONTAL,10),
	m_Button_Exit("Exit"),
	m_Button_Open("Open"),
	m_Button_Save("Save"),
	m_Button_Start("Start"),
	m_Button_Step("Step"),
	m_Label_Message("No Message")
  
{
	// Set title and border of the window
	set_title("DodgeBall");
  
	// Add outer box to the window (because the window
	// can only contain a single widget)
  
	add(frame);
	canvas.set_size_request(DIM_MAX*2,DIM_MAX*2);
	//Put the inner boxes and the separator in the outer box:
	frame.add(interaction_box);
	frame.add(canvas);
  
	
	canvas.pack_start(m_Area);
	/*
	interaction_box.pack_start(m_Button_Exit,false,false);// keep fixed width
	interaction_box.pack_start(m_Button_Open,false,false); // and aligned to left;  
	interaction_box.pack_start(m_Button_Save,false,false);
	interaction_box.pack_start(m_Button_Start,false,false);
	interaction_box.pack_start(m_Button_Step,false,false);
	*/
	interaction_box.set_layout(Gtk::ButtonBoxStyle::BUTTONBOX_START);
	interaction_box.add(m_Button_Exit);
	interaction_box.add(m_Button_Open);
	interaction_box.add(m_Button_Save);
	interaction_box.add(m_Button_Start);
	interaction_box.add(m_Button_Step);
	interaction_box.add(m_Label_Message);
	
	
	
	// Connect the clicked signal of the button to
	// their signal handler
	connect_buttons_to_handlers();

  // Show all children of the window
  set_resizable(false);
  show_all_children();
}

MyEvent::~MyEvent()
{
}

void MyEvent::connect_buttons_to_handlers(){
	m_Button_Exit.signal_clicked().connect(sigc::mem_fun(*this,
										   &MyEvent::on_button_clicked_exit));
	m_Button_Open.signal_clicked().connect(sigc::mem_fun(*this,
										   &MyEvent::on_button_clicked_open));         
	m_Button_Save.signal_clicked().connect(sigc::mem_fun(*this,
										   &MyEvent::on_button_clicked_save));  
	m_Button_Start.signal_clicked().connect(sigc::mem_fun(*this,
										   &MyEvent::on_button_clicked_start));	  
	m_Button_Step.signal_clicked().connect(sigc::mem_fun(*this,
										   &MyEvent::on_button_clicked_step));
}

void MyEvent::on_button_clicked_exit(){
	
	std::cout << "exit" << std::endl;
}

void MyEvent::on_button_clicked_open(){
	
	std::cout << "open" << std::endl;
}

void MyEvent::on_button_clicked_save(){
	
	std::cout << "save" << std::endl;
}

void MyEvent::on_button_clicked_start(){
	
	std::cout << "start" << std::endl;
}

void MyEvent::on_button_clicked_step(){
	
	std::cout << "step" << std::endl;
}
