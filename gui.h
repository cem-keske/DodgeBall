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
  //Override default signal handler:
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
  //Button Signal handlers:
  void on_button_clicked_clear();
  void on_button_clicked_draw();

  Gtk::Box m_Box, m_Box_Buttons, m_Box_Sim_Area;
  MyArea              m_Area;
  Gtk::Button 		  m_Button_Exit;
  Gtk::Button 		  m_Button_Open;
  
private:
	void draw();
};

#endif
