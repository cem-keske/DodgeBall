/**
 * file: gui.cc
 * 
 * authors:	Cem Keske
 * 			Emre Yazici
 */
#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include "simulation.h"
#include "tools.h"
#include <memory>
#include <gtkmm.h>

/// CANVAS ///


/**
 * The drawing canvas inside the gui window. 
 * Animation is realized im this component.
 */
class Canvas : public Gtk::DrawingArea{
	public:
		Canvas();
		
	public:
		//Overridden draw method for the canvas.
		bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
  
	private:
		//the center coordinates of the canvas in gui's coordinate system
		Coordinate center;
		
		/**
		 * Takes a cartesian coordinate and returns a new coordinate in gui's
		 * coordinate system.
		 */ 
		Coordinate convert_coordinate(Coordinate const&);
		
		
		
		// ===== Graphics-Draw Methods =====
		
		void draw_background(const Cairo::RefPtr<Cairo::Context>& cr,
							 Color const& background_color = Tools::COLOR_WHITE);						  
		void draw_border(const Cairo::RefPtr<Cairo::Context>& cr, Length thicnkess,
						 Color const& border_color = Tools::COLOR_BLACK);
		void draw_all_player_graphics(const Cairo::RefPtr<Cairo::Context>& cr);
		void draw_all_rectangle_graphics(const Cairo::RefPtr<Cairo::Context>& cr);
		void draw_all_ball_graphics(const Cairo::RefPtr<Cairo::Context>& cr);
		
		/**
		 * All the arguments given to the functions below must be the original shapes
		 * in the simulation (these functions occupy the coordinate conversion).
		 */ 
		void draw_disk(Circle const& original, const Cairo::RefPtr<Cairo::Context>& cr, 
					   Color const& color = Tools::COLOR_BLUE);
		void draw_arc(Coordinate const& original, Length thickness, Angle alpha, 
					  Length outer_radius, const Cairo::RefPtr<Cairo::Context>& cr,
					  Color const& color = Tools::COLOR_BLUE);
		void draw_rectangle(Rectangle const& original, 
							const Cairo::RefPtr<Cairo::Context>& cr, bool fill = true,
							Color const & color = Tools::COLOR_BROWN);
};

/// GUI WINDOW ///

/**
 * The main window for the gui.
 */ 
class Gui_Window : public Gtk::Window
{
	public:
	
		Gui_Window();
	
	private:
	
		// ===== Elements of the Window =====
		
		//The main container
		Gtk::VBox the_big_box; 
		
		
		Gtk::HButtonBox interaction_box;
		Gtk::Box		sim_arena;
		
		Canvas 			canvas;
		
		/**
		 * Interaction panel components.
		 * Utility methods must be updated after adding a new component here.
		 * 
		 */ 
		Gtk::Button 	button_exit;
		Gtk::Button 	button_open;
		Gtk::Button		button_save;
		Gtk::Button		button_start_stop;
		Gtk::Button 	button_step;
		Gtk::Label		label_message;
	
	protected:
		
		// ===== Button Handlers =====
		
		void on_button_clicked_exit();
		void on_button_clicked_open();
		void on_button_clicked_save();
		void on_button_clicked_start_stop();
		bool start_timer();
		bool stop_timer();
		void on_button_clicked_step();
		
		// ===== Timer Utilites =====
		
		bool timer_tick();
		bool timer_running;
		void toggle_simulation_running();
		
		
		// ===== Utility Methods =====
		
		void add_button_panel_components();
		void connect_buttons_to_handlers();
		
		// ===== Refresher =====
		
		void refresh();
		
		// ===== Interaction Methods =====
		
		bool ask_if_sure(std::string const& message,std::string const& title);
		void show_warning(const std::string& message,const std::string& text = "");
		void show_message(const std::string& message, const std::string& text = "");
		
		
};



#endif
