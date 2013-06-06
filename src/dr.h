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

#ifndef _DR_H_
#define _DR_H_
#include <gtkmm.h>
#include <gtkmm/filefilter.h>
#include <xmmsclient/xmmsclient++.h>
#include <xmmsclient/xmmsclient++/list.h>
#include <xmmsclient/xmmsclient++/coll.h>

#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <vector>
#include "lvt.h"

class DR: public Gtk::Dialog 
{
	public:
		enum NameSpace { rb_coll, rb_playlist };

		//                      return,                  namespace     //
		typedef sigc::signal<std::vector<Glib::ustring>, NameSpace> type_signal_namespace_changed;
		//                      return,                  collection,    orderby                  namespace //
		typedef sigc::signal<std::vector<Xmms::Dict>, Glib::ustring, std::vector<Glib::ustring>, NameSpace> type_signal_playlist_coll_changed;

		DR(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder);
		~DR();

		void set_namespace(NameSpace ns);
		NameSpace get_namespace();
		void set_coll(Glib::ustring coll_playlst);
		Glib::ustring get_coll();
		Glib::ustring get_newname();

		type_signal_namespace_changed signal_namespace_changed();
		type_signal_playlist_coll_changed signal_playlist_coll_changed();
	protected:
		const Glib::RefPtr<Gtk::Builder>& m_builder;
		Gtk::ScrolledWindow *m_scrolledwindowRenameContents;
		LVT *m_listviewtextRenameContents;
		Gtk::RadioButton *m_radiobuttonRenameNSCollection;
		Gtk::RadioButton *m_radiobuttonRenameNSPlaqylist;
		Gtk::ComboBoxText *m_comboboxtextRenameCollection;
		Gtk::Button *m_buttonRenameOK;
		Gtk::Entry *m_entryRenameNewName;
		Gtk::ButtonBox *m_action_area;

		NameSpace m_NameSpace;

		// signals //
		type_signal_namespace_changed m_signal_namespace_changed;
		type_signal_playlist_coll_changed m_signal_playlist_coll_changed;

		void on_radiobutton(NameSpace ns);
		void on_comboboxtextRenameCollection_changed();
		void on_entry_RenameNewName_changed(const Glib::ustring& s,int *i);

		// utility functions //
		int get_index(Gtk::ComboBoxText *combo, Glib::ustring text);
		bool valid_collection_name(Glib::ustring name);
	private:
		
};

#endif // _DR_H_

