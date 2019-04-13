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

static constexpr int default_border_thickness(5);

// ===== Utility Functions =====

const Color& predefined_color_chooser(Predefined_Color color){
	switch(color) {
		case RED    :	return Tools::COLOR_RED;
		case ORANGE :	return Tools::COLOR_ORANGE;
		case GREEN  :	return Tools::COLOR_GREEN;
		case YELLOW :	return Tools::COLOR_YELLOW;
	}
	return Tools::COLOR_BLACK;
}


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
	draw_all_player_bodies(cr);
	draw_background(cr);
	draw_border(cr, default_border_thickness);
	Length radius(30);
	draw_disk(Circle(radius),cr, Tools::COLOR_RED);
	draw_disk(Circle({70,0},radius),cr, Tools::COLOR_GREEN);
	draw_disk(Circle({140,0},radius),cr, Tools::COLOR_YELLOW);
	draw_disk(Circle({210,0},radius),cr, Tools::COLOR_ORANGE);
	Rectangle rect({30,50},80,80);
	draw_rectangle(rect,cr);
	draw_arc({0,0},radius/4,radius,0.45*M_PI,cr);
	draw_arc({70,0},radius/4,radius,0.93*M_PI,cr);
	draw_arc({140,0},radius/4,radius,1.4*M_PI,cr);
	draw_arc({210,0},radius/4,radius,1.90*M_PI,cr);
	return true;
}
Coordinate Canvas::convert_coordinate(Coordinate const& pos){
	return {center.x + pos.x , center.y - pos.y };
}

void Canvas::draw_background(const Cairo::RefPtr<Cairo::Context>& cr,
							 Color const& background_color){
	cr->save();
	cr->set_source_rgb(background_color.r, background_color.g, background_color.b);
	cr->paint();
	cr->restore();					 
}

void Canvas::draw_border(const Cairo::RefPtr<Cairo::Context>& cr,
						 Length thickness,
					     Color const& border_color){
	cr->save();
	cr->set_line_width(thickness);
	cr->set_source_rgb(border_color.r, border_color.g, border_color.b);
	Gtk::Allocation alloc(get_allocation());
	const int x(alloc.get_width());
	const int y(alloc.get_height());
	cr->rectangle(0, 0, x, y );
	cr->stroke();
	cr->restore();
}
void Canvas::draw_disk(Circle const& original,const Cairo::RefPtr<Cairo::Context>& cr, 
					   Color const& color){
	cr->save();
	Coordinate converted(convert_coordinate(original.center()));
	cr->set_source_rgb(color.r,color.g,color.b);
	cr->arc(converted.x, converted.y, original.radius(), 0, M_PI * 2);
	cr->stroke_preserve();
	cr->fill();		   
	cr->restore();
}
void Canvas::draw_arc(Coordinate const& original,Length thickness,Length outer_radius, 
					  Angle alpha, const Cairo::RefPtr<Cairo::Context>& cr,
					  Color const& color){
	cr->save();
	Coordinate converted(convert_coordinate(original));
	cr->set_line_width(thickness);
	cr->set_source_rgb(color.r, color.g, color.b);
	cr->arc(converted.x, converted.y,
			(outer_radius-thickness/2.0)+1, 3*M_PI_2 , alpha + 3*M_PI_2);
	cr->stroke();	   
	cr->restore();
}
void Canvas::draw_rectangle(Rectangle const& original, 
							const Cairo::RefPtr<Cairo::Context>& cr, bool fill,
							Color const& color){
	cr->save();
	Coordinate converted(convert_coordinate(original.top_left()));
	cr->set_source_rgb(color.r, color.g, color.b);
	cr->rectangle(converted.x,converted.y,original.base(),original.height());
	
	if(fill)
		cr->fill();	
	else
		cr->stroke();
	cr->restore();				 
}

void Canvas::draw_all_player_bodies(const Cairo::RefPtr<Cairo::Context>& cr) {
	const vec_player_graphics& disks_and_arcs(Simulator::get_player_graphics());
	for (auto const& circled_arc : Simulator::get_player_graphics()) {
		std::cout << "girdi" << std::endl;
		//get the circle from the tuple and draw its disk
		*(std::get<0>(circled_arc));
		std::cout << "çıktı" << std::endl;
		draw_disk(*(std::get<0>(circled_arc)), cr, 
				  predefined_color_chooser((std::get<2>(circled_arc))));//get the color
		
		//get the arc angle from the tuple and draw the arc

	}
	
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





