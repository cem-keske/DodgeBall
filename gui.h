#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include "simulation.h"
#include "tools.h"
#include <memory>
#include <gtkmm.h>

/// DRAWABLE ///

/**
 * Pure virtual class for drawable gui objects.
 */ 
class Drawable {
	protected:
		virtual void draw(const Cairo::RefPtr<Cairo::Context>& cr) = 0;
};

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
		std::vector<std::unique_ptr<Drawable>> objects;
		Coordinate center;
		
		Coordinate convert_coordinate(Coordinate const&);
		void refresh();
};

/// GUI WINDOW ///
/**
 * 
 */ 
class Gui_Window : public Gtk::Window
{
	public:
	
		Gui_Window();
		virtual ~Gui_Window();

	protected:
		
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


// ===== Drawables ===== //

class Gui_Player : public Drawable {
	private:
		std::shared_ptr<Player> player;
		void draw(const Cairo::RefPtr<Cairo::Context>& cr);
};

class Gui_Ball : public Drawable {
	private:
		std::shared_ptr<Ball> ball;
		void draw(const Cairo::RefPtr<Cairo::Context>& cr);
};

class Gui_Obstacle : public Drawable {
	private:
		std::shared_ptr<Rectangle> rectangle;
		void draw(const Cairo::RefPtr<Cairo::Context>& cr);
};


#endif
