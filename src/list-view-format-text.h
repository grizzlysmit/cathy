/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * list-view-format-text.h
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

#ifndef _LIST_VIEW_FORMAT_TEXT_H_
#define _LIST_VIEW_FORMAT_TEXT_H_
#include <gtkmm.h>

#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>

class ListViewFormatText : public Gtk::EventBox 
{
	public:
		ListViewFormatText();
		virtual ~ListViewFormatText();
		
        typedef sigc::signal<void, Glib::ustring> type_signal_clicked;

		/** Add a new row at the end of the list
		 * @return the number of the row added
		 */
		guint append(const Glib::ustring& column_one_value = Glib::ustring());

		/** Insert a new row at the beginning of the list
		 * @param column_one_value the new text for the new row, column 0
		 */
		void prepend(const Glib::ustring& column_one_value = Glib::ustring());


	    /// Discard all rows:
		void clear_items();

		/** Obtain the value of an existing cell from the list.
		 * @param row the number of the row in the listbox.
		 * @param column the number of the column in the row.
		 * @return the value of that cell, if it exists.
		 */
		Glib::ustring get_text(guint row) const;

		/** Change an existing value of cell of the list.
		 * @param row the number of the row in the list.
		 * @param column the number of the column in the row.	 
		 * @param value the new contents of that row and column.
		 */
		void set_text(guint row, const Glib::ustring& value);

		void set_tooltip_markup(const Glib::ustring markup);

		/// @return the number of rows in the listbox
		guint size() const;


		Glib::ustring SelectionString;

		/** Returns a vector of the indexes of the selected rows
		 * @return a SelectionList with the selection results
		 */
		Glib::ustring get_selected();

		type_signal_clicked signal_clicked();
		type_signal_clicked signal_dblclicked();
		
	protected:		
		Gtk::Box vbox;
		std::vector<Gtk::Label*> Rows;
		Glib::ustring m_tooltip_markup;
		type_signal_clicked m_signal_clicked;
		type_signal_clicked m_signal_dblclicked;
	
		bool on_button_Pressed(GdkEventButton* event, Gtk::Label *label);
	private:
		
};

#endif // _LIST_VIEW_FORMAT_TEXT_H_

