/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * matrix-box.h
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

#ifndef _MATRIX_BOX_H_
#define _MATRIX_BOX_H_
#include <gtkmm.h>

#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>

class MatrixBox: public Gtk::EventBox 
{
	public:
		MatrixBox(guint num_cols);
		virtual ~MatrixBox();
		typedef sigc::signal<void, int> type_signal_clicked;

		type_signal_clicked signal_clicked();
		type_signal_clicked signal_dblclicked();
		void  set_headings(const std::vector<Glib::ustring>& heads);
		guint append(const std::vector<Glib::ustring>& row, int _id);
		void clear_items();
	protected:
		class Row {
			public:
				Row();
				Row(guint num_cols);
				Row(guint num_cols, const std::vector<Gtk::Label*>& r, int _id);
				~Row();
				std::vector<Gtk::Label*>::iterator begin();
				std::vector<Gtk::Label*>::iterator end();
				void add(const std::vector<Gtk::Label*>& r, int _id);
			protected:
				std::vector<Gtk::Label*> row;
				guint num_columns;
				int id;
			private:
		};
		
		guint num_columns;
		Gtk::Grid m_grid;
		std::vector<Row*> rows;
		int currentid;
		type_signal_clicked m_signal_clicked;
		type_signal_clicked m_signal_dblclicked;
		
		bool on_button_Pressed(GdkEventButton* event, int _id, Row* row);
	private:

};

#endif // _MATRIX_BOX_H_

