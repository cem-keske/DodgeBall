/**
 * file: gui.h
 * 
 * authors:	Cem Keske
 * 			Emre Yazici
 */
#include "define.h"
#include "gui.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <cairomm/context.h>

// ===== Constants for the module gui =====

static constexpr int default_border_thickness(3);
static constexpr double circle_arc_ratio(0.35);	//between radius and arc's thickness
static constexpr int delta_t_ms((int)(DELTA_T * 1000)); //DELTA_T converted to ms
static constexpr double starting_angle(M_PI_2*3); 	//the angle where the arcs start



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

/**
 * Returns the label string to show corresponding to the given simulation state.
 * 
 */
std::string state_to_string(Simulation_State active_state){
	switch(active_state){		
		case NO_GAME 		: return "No game to run";
		case GAME_READY 	: return "Game ready to run";
		case GAME_OVER		: return "Game's over !";
		case PLAYER_TRAPPED : return "Cannot complete the game!";
		default 			: return "Error! Cannot resolve the state!";
	}
	
}

bool file_exists(const std::string& file_path){
	std::ifstream file_to_check(file_path);
	return file_to_check.good();
}




/// ===== CANVAS ===== ///



Canvas::Canvas() : center(DIM_MAX,DIM_MAX) {
	set_size_request(DIM_MAX*2,DIM_MAX*2);
}


bool Canvas::on_draw(const Cairo::RefPtr<Cairo::Context>& cr){
		
	draw_background(cr); //this gets onto te old display
	if(Simulator::empty() == false) {
		draw_all_player_graphics(cr);
		draw_all_rectangle_graphics(cr);
		draw_all_ball_graphics(cr);
	}
	draw_border(cr, default_border_thickness);	
	return true;
}

Coordinate Canvas::convert_coordinate(Coordinate const& pos){
	return {center.x + pos.x , center.y - pos.y };
}

/**
 * Default value for color is white.
 */
void Canvas::draw_background(const Cairo::RefPtr<Cairo::Context>& cr, 
						Color const& background_color){
	cr->save();
	cr->set_source_rgb(background_color.r, background_color.g, background_color.b);
	cr->paint();
	cr->restore();					 
}

/**
 * Default value for color is black.
 */
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

/**
 * Default value for color is blue.
 */
void Canvas::draw_disk(Circle const& original,const Cairo::RefPtr<Cairo::Context>& cr, 
					   Color const& color){
	cr->save();
	Coordinate converted(convert_coordinate(original.center()));
	cr->set_source_rgb(color.r,color.g,color.b);
	cr->arc(converted.x, converted.y, original.radius(), 0, M_PI * 2);
	cr->fill();		   
	cr->restore();
}

/**
 * Default value for color is blue.
 */
void Canvas::draw_arc(Coordinate const& original,Length thickness,Length outer_radius, 
					  Angle alpha, const Cairo::RefPtr<Cairo::Context>& cr,
					  Color const& color){
	cr->save();
	Coordinate converted(convert_coordinate(original));
	cr->set_line_width(thickness);
	cr->set_source_rgb(color.r, color.g, color.b);
	cr->arc(converted.x, converted.y,
			(outer_radius-thickness/2.0), starting_angle , starting_angle + alpha);
	cr->stroke();	   
	cr->restore();
}

/**
 * Default value for color is blue.
 * Default value for 'fill' is true.
 */
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

/**
 * Draws graphics for players requesting the shapes to draw from the simulation module. 
 */
void Canvas::draw_all_player_graphics(const Cairo::RefPtr<Cairo::Context>& cr) {
	
	for (auto const& circled_arc : Simulator::fetch_player_graphics()) {
		//get the circle, color and angle from the tuple
		Circle const& circ(*(std::get<0>(circled_arc))); 	//circle at '0'
		Angle arc_angle(std::get<1>(circled_arc)); 			//angle at '1'
		Color const& color(predefined_color_chooser(std::get<2>(circled_arc)));
		
		draw_disk(circ, cr, color);

		draw_arc(circ.center(), circle_arc_ratio * circ.radius(), circ.radius(), 
				 arc_angle,cr);
	}
}

/**
 * Draws graphics for rectangles requesting the shapes to draw from simulation. 
 */
void Canvas::draw_all_rectangle_graphics(const Cairo::RefPtr<Cairo::Context>& cr){
	for (auto rectangle : Simulator::fetch_obstacle_bodies()){
		draw_rectangle(*rectangle, cr);
	}
}

/**
 * Draws graphics for balls requesting the circles to draw from simulation.
 */
void Canvas::draw_all_ball_graphics(const Cairo::RefPtr<Cairo::Context>& cr){
	for (auto circle : Simulator::fetch_ball_bodies()){
		draw_disk(*circle, cr);
	}	
}



/// ===== GUI_WINDOW ===== ///

Gui_Window::Gui_Window() :
	button_exit("Exit"),
	button_open("Open"),
	button_save("Save"),
	button_start_stop("Start"),
	button_step("Step"),
	label_message(state_to_string(Simulator::active_simulation_state())),
	timer_running(false) {
	
	set_title("DodgeBall");
	//initialize the button panel
	interaction_box.set_layout(Gtk::ButtonBoxStyle::BUTTONBOX_START);
	add_button_panel_components();
	connect_buttons_to_handlers();
	//initialize the canvas
	sim_arena.add(canvas);
	//add components to the big box
	the_big_box.add(interaction_box);
	the_big_box.add(sim_arena);
	//add the big box to the window and show everything
	add(the_big_box);
	set_resizable(false);
	show_all_children();
}



// ===== Button Handlers =====

void Gui_Window::on_button_clicked_exit(){
	exit(0);
}

void Gui_Window::on_button_clicked_open(){
	int response(0);
	std::string file_adress("nofile");
	
	{
	Gtk::FileChooserDialog file_dialog("Please open a file", 
										Gtk::FILE_CHOOSER_ACTION_OPEN); 
	file_dialog.set_transient_for(*this);
		
	file_dialog.add_button("Open", Gtk::RESPONSE_OK);
	file_dialog.add_button("Cancel", Gtk::RESPONSE_CANCEL);
	
	response = file_dialog.run();
	file_adress = file_dialog.get_filename();
	}//dialog window destroyed, we don't want to see it on the screen anymore
	
	if(response == Gtk::RESPONSE_OK){
		if(Simulator::import_file(file_adress)){
			show_message("File succesfully imported");
			//refresh the window after importing
			refresh();
		} else {
			show_warning("Unsuccesful import");
		}
	}	
}

void Gui_Window::on_button_clicked_save(){
	if(Simulator::active_simulation_state() == NO_GAME){
		show_warning("There's no active simulation to save!");
		return;
	}
	Gtk::FileChooserDialog file_dialog("Please open a file", 
										Gtk::FILE_CHOOSER_ACTION_SAVE); 
	//Put the dialog on our window
	file_dialog.set_transient_for(*this);
	file_dialog.set_title("Please save your file");
	//Add dialog buttons
	file_dialog.add_button("Save", Gtk::RESPONSE_OK);
	file_dialog.add_button("Cancel", Gtk::RESPONSE_CANCEL);
	
	int response = file_dialog.run();
	
	if(response == Gtk::RESPONSE_OK){
		std::string file_path(file_dialog.get_filename());
		bool write(true); //go ahead and save - or don't save
		if(file_exists(file_path))
			write = ask_if_sure("This file already exists in your computer, " 
								 "are you sure you want to overwrite it?",
								 "The existing simulation data in this file"
								 " will be lost.");
		if(write){
			Simulator::save_simulation(file_path);
			show_message("Simulation saved to: " + file_path);
		}
	}
	refresh();
}

void Gui_Window::on_button_clicked_start_stop(){
	static std::string labels[] = {"Start","Stop"};
	
	if(Simulator::active_simulation_state() == NO_GAME) {
		show_warning("No game to start or stop!");
		return;
	}
	
	//call the correct handler
	if(button_start_stop.get_label() == labels[0]) {
		if (start_timer()) 
			button_start_stop.set_label(labels[1]);
	} else {
		if (stop_timer())
			button_start_stop.set_label(labels[0]);
	}
}

void Gui_Window::on_button_clicked_step(){
	if(Simulator::active_simulation_state() == GAME_READY){
		Simulator::update_active_sim(DELTA_T);
		refresh();
	} else {
		show_warning("Nowhere to step... :/");
	}
}


// ===== Timer Utilites =====


bool Gui_Window::timer_tick(){
	if(timer_running == true){
		Simulator::update_active_sim(DELTA_T);
		refresh(); //refresh the gui window after updating the simulation
		if(Simulator::active_simulation_state() != GAME_READY) //when there's no sim to run
			stop_timer();
	}
	return timer_running;
}


bool Gui_Window::start_timer(){
	if(timer_running)
		return false;
	
	//timer was not running	, so start the action
	
	Glib::signal_timeout().connect(sigc::mem_fun(*this, &Gui_Window::timer_tick),
												  delta_t_ms);
	timer_running = true;
	return true;
}
bool Gui_Window::stop_timer(){
	if(timer_running == false)
		return false; //timer is not running
	timer_running = false;
	return true;
}


// ===== Other Utility Methods =====


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
	interaction_box.pack_start(button_exit);
	interaction_box.pack_start(button_open);
	interaction_box.pack_start(button_save);
	interaction_box.pack_start(button_start_stop);
	interaction_box.pack_start(button_step);
	interaction_box.pack_start(label_message);
}




// ===== Refresher =====


/**
 * Refreshes the gui window and all subcomponents using the current simulation.
 * 
 * !!! This function does not occupy calling the update method of the simulation !!!
 * !!! Simulation should be updated explicitly before calling this function !!!
 */
void Gui_Window::refresh(){
	
	//change the message shown if necessary
	Simulation_State state(Simulator::active_simulation_state());
	label_message.set_text(state_to_string(state));
	
	//draw all subcomponents
	auto window = get_window();
    if (window) {
        Gdk::Rectangle rectangle(0, 0, get_allocation().get_width(),
                                 get_allocation().get_height());
        window->invalidate_rect(rectangle, false);
    }
	//on_draw(get_window()->create_cairo_context());
}





// ===== Interaction Methods =====

bool Gui_Window::ask_if_sure(std::string const& message, std::string const& text){
	Gtk::MessageDialog msg_dialog(*this, message, true, Gtk::MESSAGE_WARNING,
								  Gtk::BUTTONS_NONE);
	
	//Add dialog buttons
	msg_dialog.set_title("Are you sure?");
	msg_dialog.set_secondary_text(text);
	msg_dialog.add_button("Yes, go ahead", Gtk::RESPONSE_OK);
	msg_dialog.add_button("Cancel", Gtk::RESPONSE_CANCEL);
	
	int response = msg_dialog.run();
	return (response==Gtk::RESPONSE_OK);
}

void Gui_Window::show_warning(const std::string& message, const std::string& text){
	Gtk::MessageDialog msg_dialog(*this, message, false, Gtk::MESSAGE_WARNING,
								  Gtk::BUTTONS_OK);
	msg_dialog.set_secondary_text(text);
	msg_dialog.run();
}

void Gui_Window::show_message(const std::string& message, const std::string& text){
	Gtk::MessageDialog msg_dialog(*this, message, false, Gtk::MESSAGE_INFO,
								  Gtk::BUTTONS_OK);
	msg_dialog.set_secondary_text(text);
	msg_dialog.run();
}



