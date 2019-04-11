#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include "simulation.h"
#include "tools.h"
#include <gtkmm.h>
//cem
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
	
		bool empty;
		void refresh();
};


class MyEvent : public Gtk::Window
{
	public:
	
		MyEvent();
		virtual ~MyEvent();

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

#endif
