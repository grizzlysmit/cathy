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

#ifndef _LVT_H_
#define _LVT_H_
#include <gtkmm.h>

#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <algorithm>    
#include <vector>

class LVT: public Gtk::ListViewText 
{
	public:
		typedef sigc::signal<void> type_signal_clicked;
		
		LVT(guint columns_count, bool editable=false, Gtk::SelectionMode mode=Gtk::SELECTION_SINGLE);
		~LVT();

		void remove_all_rows();
		type_signal_clicked signal_clicked();
		type_signal_clicked signal_dblclicked();
		type_signal_clicked signal_selection_changed();
	protected:
		Gtk::ListViewText::SelectionList m_selected;
		type_signal_clicked m_signal_clicked;
		type_signal_clicked m_signal_dblclicked;
		type_signal_clicked m_signal_selection_changed;
		
		virtual bool on_button_press_event(GdkEventButton *event);
		virtual bool notequal(Gtk::ListViewText::SelectionList lst1, Gtk::ListViewText::SelectionList lst2);
	private:
		
};

#endif // _LVT_H_

