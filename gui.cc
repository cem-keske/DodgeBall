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

Canvas::Canvas(): empty(false)
{
}

Canvas::~Canvas()
{
}

void Canvas::clear()
{
  empty = true; 
  refresh();
}

void Canvas::draw()
{
  empty = false;
  refresh();
}

void Canvas::refresh()
{
  
}

bool Canvas::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  

  return true;
}


//--------------------------------------

MyEvent::MyEvent() :
	m_Button_Exit("Exit"),
	m_Button_Open("Open"),
	m_Button_Save("Save"),
	m_Button_Start("Start"),
	m_Button_Step("Step"),
	m_Label_Message("No Message")
{
	set_title("DodgeBall");
	//init buttons
	interaction_box.set_layout(Gtk::ButtonBoxStyle::BUTTONBOX_START);
	connect_buttons_to_handlers();
	add_buttons();
	//init canvas
	canvas.set_size_request(DIM_MAX*2,DIM_MAX*2);
	sim_arena.add(canvas);
	//add components to the big box
	the_big_box.add(interaction_box);
	the_big_box.add(sim_arena);
	//add the big box to the window and show everything
	add(the_big_box);
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

void MyEvent::add_buttons(){
	
	interaction_box.add(m_Button_Exit);
	interaction_box.add(m_Button_Open);
	interaction_box.add(m_Button_Save);
	interaction_box.add(m_Button_Start);
	interaction_box.add(m_Button_Step);
	interaction_box.add(m_Label_Message);

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
}// benim adım cem

