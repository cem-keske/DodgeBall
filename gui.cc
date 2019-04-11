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
	button_exit("Exit"),
	button_open("Open"),
	button_save("Save"),
	button_start_stop("Start"),
	button_step("Step"),
	label_message("No Message")
{
	set_title("DodgeBall");
	//init button panel
	interaction_box.set_layout(Gtk::ButtonBoxStyle::BUTTONBOX_START);
	add_button_panel_components();
	connect_buttons_to_handlers();
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
	button_exit.signal_clicked().connect(sigc::mem_fun(*this,
										   &MyEvent::on_button_clicked_exit));
	button_open.signal_clicked().connect(sigc::mem_fun(*this,
										   &MyEvent::on_button_clicked_open));         
	button_save.signal_clicked().connect(sigc::mem_fun(*this,
										   &MyEvent::on_button_clicked_save));  
	button_start_stop.signal_clicked().connect(sigc::mem_fun(*this,
										   &MyEvent::on_button_clicked_start_stop));	  
	button_step.signal_clicked().connect(sigc::mem_fun(*this,
										   &MyEvent::on_button_clicked_step));
}

void MyEvent::add_button_panel_components(){
	interaction_box.add(button_exit);
	interaction_box.add(button_open);
	interaction_box.add(button_save);
	interaction_box.add(button_start_stop);
	interaction_box.add(button_step);
	interaction_box.add(label_message);
}

void MyEvent::on_button_clicked_exit(){
	
	std::cout << button_exit.get_label() << std::endl;
}

void MyEvent::on_button_clicked_open(){
	
	std::cout <<button_open.get_label() << std::endl;
}

void MyEvent::on_button_clicked_save(){
	
	std::cout << button_save.get_label() << std::endl;
}

void MyEvent::on_button_clicked_start_stop(){
	static std::string labels[] = {"Start","Stop"};
	
	
	
	
	
	//print on the console and change the label
	std::cout << button_start_stop.get_label() << std::endl;
	button_start_stop.set_label((button_start_stop.get_label()==labels[0]) ? 
								 labels[1] : labels[0]);
}

void MyEvent::on_button_clicked_step(){
	
	std::cout << button_step.get_label() << std::endl;
}// benim adım cem

