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
		
	protected:
		//Overridden draw method for the canvas.
		bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
  
	private:
		
		Coordinate center;//the center coordinates of the canvas in gui's coord. system
		
		/**
		 * Takes a cartesian coordinate and returns a new coordinate in gui's
		 * coordinate system.
		 */ 
		Coordinate convert_coordinate(Coordinate const&);
		
		
		
		// ===== Graphic-Draw Methods =====
		void draw_background(const Cairo::RefPtr<Cairo::Context>& cr,
							 Color const& background_color = Tools::COLOR_WHITE);						 
		void draw_border(const Cairo::RefPtr<Cairo::Context>& cr, Length thicnkess,
						 Color const& border_color = Tools::COLOR_BLACK);
		void draw_all_player_graphics(const Cairo::RefPtr<Cairo::Context>& cr);
		void draw_all_rectangle_graphics(const Cairo::RefPtr<Cairo::Context>& cr);
		void draw_all_ball_graphics(const Cairo::RefPtr<Cairo::Context>& cr);
		/**
		 * All the arguments given to the functions below must be the original shapes
		 * in the simulation. (these functions also converts coordinates).
		 */ 
		void draw_disk(Circle const& original, const Cairo::RefPtr<Cairo::Context>& cr, 
					   Color const& color = Tools::COLOR_BLUE);
		void draw_arc(Coordinate const& original, Length thickness, Angle alpha, 
					  Length outer_radius, const Cairo::RefPtr<Cairo::Context>& cr,
					  Color const& color = Tools::COLOR_BLUE);
		void draw_rectangle(Rectangle const& original, 
							const Cairo::RefPtr<Cairo::Context>& cr, bool fill = true,
							Color const & color = Tools::COLOR_BROWN);
		/**
		 * The refresh method for the animation window. This function will also call 
		 * the update method of simulation in further iplementations.
		 */
		void refresh();
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
		
		Gtk::VBox the_big_box; //the main container
		
		Gtk::HButtonBox interaction_box;
		Gtk::Box		sim_arena;
		
		Canvas 			canvas;
		
		Gtk::Button 	button_exit;
		Gtk::Button 	button_open;
		Gtk::Button		button_save;
		Gtk::Button		button_start_stop;
		Gtk::Button 	button_step;
		Gtk::Label		label_message;
	protected:
		// ===== Event Handlers =====
		void on_button_clicked_exit();
		void on_button_clicked_open();
		void on_button_clicked_save();
		void on_button_clicked_start_stop();
		void on_button_clicked_step();
		
		void refresh();
		
		
		
	
		void add_button_panel_components();
		void connect_buttons_to_handlers();
		
};



#endif
