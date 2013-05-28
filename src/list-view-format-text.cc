/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * list-view-format-text.cc
 * Copyright (C) 2013 Francis (Grizzly) Smit <grizzlysmit@smit.id.au>
 *
 * cathy is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * cathy is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "list-view-format-text.h"



ListViewFormatText::ListViewFormatText()
   : vbox(Gtk::ORIENTATION_VERTICAL)
{
    add(vbox); // add the vbox to the ListViewFormatText Gtk::EventBox dirived widget 
	//  set the background colour of the ListViewFormatText to white //
	std::string style_sheet = ".white_bg {background: #FFFFFF; color: #000000; } ";
	//style_sheet += ".green_bg {background: #33FF33; color: #000000; }";
	Glib::RefPtr<Gtk::StyleContext> stylecontext = get_style_context();
	Glib::RefPtr<Gtk::CssProvider> cssprov = Gtk::CssProvider::create();
	cssprov->load_from_data(style_sheet);
	stylecontext->add_provider(cssprov, GTK_STYLE_PROVIDER_PRIORITY_USER);
	stylecontext->add_class("white_bg");
	//stylecontext->add_class("green_bg");
	stylecontext->context_save();
}

ListViewFormatText::~ListViewFormatText()
{
    clear_items();
}

guint ListViewFormatText::append(const Glib::ustring& column_one_value)
{
	try{
		Gtk::Label *tmp = new Gtk::Label(column_one_value, Gtk::ALIGN_START);
		tmp->set_use_markup();
		tmp->set_selectable();
		//tmp->set_opacity(1);
		tmp->set_tooltip_markup(m_tooltip_markup);
		tmp->set_has_tooltip(true);
		//std::cout << __FILE__ << "[" << __LINE__ << "] got here" << std::endl;


		Rows.insert(Rows.end(), tmp);
		tmp->set_events(Gdk::BUTTON_PRESS_MASK);
		tmp->signal_button_press_event().connect( sigc::bind<Gtk::Label*>( sigc::mem_fun(*this, &ListViewFormatText::on_button_Pressed), tmp), false );
		//std::cout << __FILE__ << "[" << __LINE__ << "] got here" << std::endl;


		vbox.pack_start(*tmp, true, true, 0);
		//std::cout << __FILE__ << "[" << __LINE__ << "] got here" << std::endl;
		vbox.show_all();
		tmp->show();
		//std::cout << __FILE__ << "[" << __LINE__ << "] got here" << std::endl;

		return size() - 1;
	}
	catch(std::exception &e){
		std::cout << __FILE__ << "[" << __LINE__ 
			<< "] Error in " << __PRETTY_FUNCTION__ << ": " 
			<< e.what() << std::endl;
		return 0;
	}
	
}

void ListViewFormatText::prepend(const Glib::ustring& column_one_value)
{
	try{
		Gtk::Label *tmp = new Gtk::Label(column_one_value, Gtk::ALIGN_START);
		tmp->set_use_markup();
		tmp->set_selectable();
		tmp->set_tooltip_markup(m_tooltip_markup);
		tmp->set_has_tooltip(true);
		//tmp->set_opacity(1);

		Rows.insert(Rows.begin(), tmp);
		tmp->set_events(Gdk::BUTTON_PRESS_MASK);
		tmp->signal_button_press_event().connect( sigc::bind<Gtk::Label*>( sigc::mem_fun(*this, &ListViewFormatText::on_button_Pressed), tmp), false );

		vbox.pack_end(*tmp, true, true, 0);
		tmp->show();
		vbox.show_all();
	}
	catch(std::exception &e){
		std::cout << __FILE__ << "[" << __LINE__ 
			<< "] Error in " << __PRETTY_FUNCTION__ << ": " 
			<< e.what() << std::endl;
	}
}

void ListViewFormatText::clear_items()
{
	try{
		Gtk::Label *tmp;
		for(std::vector<Gtk::Label*>::iterator i(Rows.begin()), i_end(Rows.end()); i != i_end; ++i){
			tmp = *i;
			vbox.remove(*tmp);
			delete tmp;
		}
		Rows.clear();
	}
	catch(std::exception &e){
		std::cout << __FILE__ << "[" << __LINE__ 
			<< "] Error in " << __PRETTY_FUNCTION__ << ": " 
			<< e.what() << std::endl;
	}
}

Glib::ustring ListViewFormatText::get_text(guint row) const
{
	Glib::ustring result;

	g_return_val_if_fail( row < size(), result );

	result = Rows[row]->get_text();

	return result;
}

void ListViewFormatText::set_text(guint row, const Glib::ustring& value)
{
	g_return_if_fail( row < size() );

	Rows[row]->set_label(value);
}

guint ListViewFormatText::size() const
{
	return Rows.size();
}

Glib::ustring ListViewFormatText::get_selected()
{
	return SelectionString;
}

bool ListViewFormatText::on_button_Pressed(GdkEventButton* event, Gtk::Label *label){
	try{
		//std::cout << "entering on_button_Pressed" << std::endl;
		label->select_region(0);
		if(event->type == GDK_2BUTTON_PRESS){ //double click //
			SelectionString = label->get_text();
			m_signal_dblclicked.emit(SelectionString);
			//std::cout << "double click: " << SelectionString << std::endl;
			return false;
		}else if(event->type == GDK_BUTTON_PRESS){ // single click //
			SelectionString = label->get_text();
			m_signal_clicked.emit(SelectionString);
			//std::cout << "single click: " << SelectionString << std::endl;
			return true;
		}else if(event->type == GDK_3BUTTON_PRESS){ // single click //
			SelectionString = label->get_text();
			//std::cout << "triple click: " << SelectionString << std::endl;
			return false;
		}else if(event->type == GDK_BUTTON_RELEASE){
			SelectionString = label->get_text();
			//std::cout << "button release: " << SelectionString << std::endl;
			return false;
		}
	}
	catch(std::exception &e){
		std::cout << __FILE__ << "[" << __LINE__ 
			<< "] Error in " << __PRETTY_FUNCTION__ << ": " 
			<< e.what() << std::endl;
	}
	return false;
}

ListViewFormatText::type_signal_clicked ListViewFormatText::signal_clicked()
{
	return m_signal_clicked;
}

ListViewFormatText::type_signal_clicked ListViewFormatText::signal_dblclicked()
{
	return m_signal_dblclicked;
}

void ListViewFormatText::set_tooltip_markup(const Glib::ustring markup)
{
	Gtk::EventBox::set_tooltip_markup(markup);
	m_tooltip_markup = markup;
}

