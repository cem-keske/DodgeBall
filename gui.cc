/**
 * file: map.h
 * 
 * authors:	Cem Keske
 * 			Emre Yazici
 */
#include "define.h"
#include "gui.h"
#include <iostream>
#include <cmath>
#include <cairomm/context.h>


Canvas::Canvas() : center(DIM_MAX,DIM_MAX), sim_running(false) {
	set_size_request(DIM_MAX*2,DIM_MAX*2);
	
}

Canvas::~Canvas(){
}

void Canvas::draw(){
	
}

void Canvas::refresh()
{
  
}

bool Canvas::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{	
	draw_disk(Circle(100),{1,0.0,0.0},get_window()->create_cairo_context());

	return true;
}
Coordinate Canvas::convert_coordinate(Coordinate const& pos){
	return center.symmetric_x_axis()+pos;
}

void Canvas::draw_disk(Circle const& original,const Cairo::RefPtr<Cairo::Context>& cr, 
					   Color const& color = default_disk_color){
	cr->save();
	Coordinate converted(convert_coordinate(original.center()));
	cr->set_source_rgb(color.r,color.g,color.b);
	cr->arc(converted.x, converted.y, original.radius(), 0, M_PI * 2);
	cr->stroke_preserve();
	cr->fill();		   
	cr->restore();
}
void Canvas::draw_arc(Coordinate const& original, Length thickness, Angle alpha, 
			   Length outer_radius, const Cairo::RefPtr<Cairo::Context>& cr,
			   Color const& color){
	cr->save();
	Coordinate converted(convert_coordinate(original.center()));
	cr->set_line_width(thickness);
	cr->set_source_rgb(color.r, color.g, color.b);
	cr->arc(converted.x, converted.y, original.radius(), -M_PI_2 , alpha-M_PI_2);
	cr->stroke_preserve();
	cr->fill();		   
	cr->restore();
}
void Canvas::draw_square(Rectangle const& original, 
					     const Cairo::RefPtr<Cairo::Context>& cr, bool fill,
					     Color const& color){
	cr->save();
	Coordinate converted(convert_coordinate(original.center()));
	cr->set_line_width(thickness);
	cr->set_source_rgb(color.r, color.g, color.b);
	cr->rectangle(converted.x, converted.y, original.base(), original.height());
	cr->stroke_preserve();
	cr->fill();		   
	cr->restore();				 
}




//--------------------------------------

Gui_Window::Gui_Window() :
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
	sim_arena.add(canvas);
	//add components to the big box
	the_big_box.add(interaction_box);
	the_big_box.add(sim_arena);
	//add the big box to the window and show everything
	add(the_big_box);
	set_resizable(false);
	show_all_children();
}

Gui_Window::~Gui_Window()
{
}

void Gui_Window::connect_buttons_to_handlers(){
	button_exit.signal_clicked().connect(sigc::mem_fun(*this,
										   &Gui_Window::on_button_clicked_exit));
	button_open.signal_clicked().connect(sigc::mem_fun(*this,
										   &Gui_Window::on_button_clicked_open));         
	button_save.signal_clicked().connect(sigc::mem_fun(*this,
										   &Gui_Window::on_button_clicked_save));  
	button_start_stop.signal_clicked().connect(sigc::mem_fun(*this,
										   &Gui_Window::on_button_clicked_start_stop));	  
	button_step.signal_clicked().connect(sigc::mem_fun(*this,
										   &Gui_Window::on_button_clicked_step));
}

void Gui_Window::add_button_panel_components(){
	interaction_box.add(button_exit);
	interaction_box.add(button_open);
	interaction_box.add(button_save);
	interaction_box.add(button_start_stop);
	interaction_box.add(button_step);
	interaction_box.add(label_message);
}

void Gui_Window::on_button_clicked_exit(){
	
	std::cout << button_exit.get_label() << std::endl;
}

void Gui_Window::on_button_clicked_open(){
	
	std::cout <<button_open.get_label() << std::endl;
}

void Gui_Window::on_button_clicked_save(){
	
	std::cout << button_save.get_label() << std::endl;
}

void Gui_Window::on_button_clicked_start_stop(){
	static std::string labels[] = {"Start","Stop"};
	
	
	
	
	
	//print on the console and change the label
	std::cout << button_start_stop.get_label() << std::endl;
	button_start_stop.set_label((button_start_stop.get_label()==labels[0]) ? 
								 labels[1] : labels[0]);
}

void Gui_Window::on_button_clicked_step(){
	std::cout << button_step.get_label() << std::endl;
}





