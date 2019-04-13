#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include "simulation.h"
#include "tools.h"
#include <memory>
#include <gtkmm.h>

/// CANVAS ///



/**
 * Drawing canvas for gui.
 */
class Canvas : public Gtk::DrawingArea
{
	public:
		Canvas();
		virtual ~Canvas();
		void clear();
		void draw();

	protected:
		bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
  
	private:
		Coordinate center;
		Coordinate convert_coordinate(Coordinate const&);
		bool sim_running;
		void refresh();
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
	public:
		void draw_background(const Cairo::RefPtr<Cairo::Context>& cr,
							 Color const& background_color = Tools::COLOR_WHITE);						 
		void draw_border(const Cairo::RefPtr<Cairo::Context>& cr, Length thicnkess,
						 Color const& border_color = Tools::COLOR_BLACK);	
		
};

/// GUI WINDOW ///
/**
 * Our big window for the gui.
 */ 
class Gui_Window : public Gtk::Window
{
	public:
	
		Gui_Window();
		virtual ~Gui_Window();

	protected:
		
		virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
		void on_button_clicked_exit();
		void on_button_clicked_open();
		void on_button_clicked_save();
		void on_button_clicked_start_stop();
		void on_button_clicked_step();
		
		Gtk::VBox the_big_box;
		
		Gtk::HButtonBox interaction_box;
		Gtk::Box		sim_arena;
		Canvas 			canvas;
		
		Gtk::Button 	button_exit;
		Gtk::Button 	button_open;
		Gtk::Button		button_save;
		Gtk::Button		button_start_stop;
		Gtk::Button 	button_step;
		Gtk::Label		label_message;
		
	private:
		
		void draw();
		void connect_buttons_to_handlers();
		void add_button_panel_components();
		
};



#endif
