#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include "simulation.h"
#include "tools.h"
#include <gtkmm.h>

class MyArea : public Gtk::DrawingArea
{
	public:

		MyArea();
		virtual ~MyArea();
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
		void on_button_clicked_start();
		void on_button_clicked_step();
		
		Gtk::Box frame; 
		Gtk::Box canvas;
		Gtk::HButtonBox interaction_box;
		MyArea 			m_Area;
		Gtk::Button 	m_Button_Exit;
		Gtk::Button 	m_Button_Open;
		Gtk::Button		m_Button_Save;
		Gtk::Button		m_Button_Start;
		Gtk::Button 	m_Button_Step;
		Gtk::Label		m_Label_Message;
		
	private:
		void draw();
		void connect_buttons_to_handlers();
};

#endif
