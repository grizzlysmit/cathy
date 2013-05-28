/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * cathy
 * Copyright (C) 2013 Francis (Grizzly) Smit <grizzlysmit@rakbat>
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

#include "lvt.h"

LVT::LVT(guint columns_count, bool editable, Gtk::SelectionMode mode)
  : Gtk::ListViewText(columns_count, editable, mode)
{
}

LVT::~LVT()
{
}

void LVT::remove_all_rows()
{
	Glib::RefPtr<Gtk::TreeModel> reftm = get_model();
	Glib::RefPtr<Gtk::TreeStore> refStore = Glib::RefPtr<Gtk::TreeStore>::cast_dynamic(reftm);
	if(refStore){
		std::cout << __FILE__ << '[' << __LINE__ << "]  it's a Gtk::TreeStore" << std::endl;
		Gtk::TreeIter i;
		while((i = refStore->children().begin()) != refStore->children().end()){
			refStore->erase(i);
		}
		return;
	}
	Glib::RefPtr<Gtk::ListStore> refLStore = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(reftm);
	if(refLStore){
		std::cout << __FILE__ << '[' << __LINE__ << "]  it's a Gtk::ListStore" << std::endl;
		Gtk::TreeIter i;
		while((i = refLStore->children().begin()) != refLStore->children().end()){
			refLStore->erase(i);
		}
	}
}

bool LVT::on_button_press_event(GdkEventButton *event)
{
	bool result = Gtk::ListViewText::on_button_press_event(event);
	Gtk::ListViewText::SelectionList lst = get_selected();
	if(notequal(lst, m_selected)){
		m_signal_selection_changed.emit();
	}
	m_selected = lst;
	if(event->type == GDK_2BUTTON_PRESS){ //double click //
		m_signal_dblclicked.emit();
		std::cout << "double click: " << std::endl;
		return result;
	}else if(event->type == GDK_BUTTON_PRESS){ // single click //
		m_signal_clicked.emit();
		std::cout << "single click: " << std::endl;
		return result;
	}
	return result;
}


LVT::type_signal_clicked LVT::signal_clicked()
{
	return m_signal_clicked;
}

LVT::type_signal_clicked LVT::signal_dblclicked()
{
	return m_signal_dblclicked;
}

LVT::type_signal_clicked LVT::signal_selection_changed()
{
	return m_signal_selection_changed;
}

bool sortcompare(int n, int m) //not a member used to sort //
{ 
	return (n < m); 
}

bool equalcompare(int n, int m)
{
	std::cout << __FILE__ << '[' << __LINE__ << "] equalcompare() n == " << n << std::endl;
	std::cout << __FILE__ << '[' << __LINE__ << "] equalcompare() m == " << m << std::endl;
	return (n == m);
}

bool LVT::notequal(Gtk::ListViewText::SelectionList lst1, Gtk::ListViewText::SelectionList lst2)
{
	std::cout << __FILE__ << '[' << __LINE__ << "]  inside notequal" << std::endl;
	if(lst1.size() != lst2.size()){
		std::cout << __FILE__ << '[' << __LINE__ << "]  lst1.size() != list2.size()" << std::endl;
		std::cout << __FILE__ << '[' << __LINE__ << "]  lst1.size() == " << lst1.size() << std::endl;
		std::cout << __FILE__ << '[' << __LINE__ << "]  lst2.size() == " << lst2.size() << std::endl;
		return false;
	}
	std::sort(lst1.begin(), lst1.end(), sortcompare);
	std::cout << __FILE__ << '[' << __LINE__ << "]  lst1 sorted" << std::endl;
	std::sort(lst2.begin(), lst2.end(), sortcompare);
	std::cout << __FILE__ << '[' << __LINE__ << "]  lst2 sorted" << std::endl;
	return !std::equal(lst1.begin(), lst1.end(), lst2.begin(), equalcompare);
}





