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
		
		Gtk::Button 	m_Button_Exit;
		Gtk::Button 	m_Button_Open;
		Gtk::Button		m_Button_Save;
		Gtk::Button		start_stop_;
		Gtk::Button 	m_Button_Step;
		Gtk::Label		m_Label_Message;
		
	private:
		void draw();
		void connect_buttons_to_handlers();
		void add_buttons();
};

#endif
