/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * cathy
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

#ifndef _DIALOG_NEW_PLAYLIST_H_
#define _DIALOG_NEW_PLAYLIST_H_
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


class DialogNewPlaylist : public Gtk::Dialog 
{
	public:
		enum RadioSelected { directory, url, collection};
		enum OrderBy { atoz, ztoa, manual};
		
		typedef sigc::signal<std::vector<Xmms::Dict>, Glib::ustring, std::vector<Glib::ustring> > type_signal_coll_changed;
		
		DialogNewPlaylist(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder);
		~DialogNewPlaylist();

		void set_collections(std::vector<Glib::ustring> colls);
		std::vector<Glib::ustring> get_order();
		RadioSelected get_radio_selected();
		Glib::ustring get_playlist_name();
		Glib::ustring get_filename();
		Glib::ustring get_url();
		Glib::ustring get_collection_name();
		bool get_addrecursive();

		type_signal_coll_changed signal_coll_changed();
	protected:

	private:
		const Glib::RefPtr<Gtk::Builder>& m_builder;

		// stuff inside dialogNewPlaylist //
		Gtk::Entry *m_entryNewPlayList;
		Gtk::FileChooserButton *m_filechooserbutton1;
		Gtk::Entry *m_entryUrl;
		Gtk::ComboBoxText *m_comboboxtextCollection;
		Gtk::CheckButton *m_checkbuttonAddRecursive;
		Gtk::RadioButton *m_radiobuttonDirectory;
		Gtk::RadioButton *m_radiobuttonUrl;
		Gtk::RadioButton *m_radiobuttonCollection;
		Gtk::ComboBoxText *m_comboboxtextOrderBy;
		Gtk::ScrolledWindow *m_scrolledwindowOrderBy;
		LVT *m_listviewtextOrderBy;
		Gtk::ScrolledWindow *m_scrolledwindowAvailable;
		LVT *m_listviewtextAvailable;
		Gtk::ScrolledWindow *m_scrolledwindowColContent;
		LVT *m_listviewtextColContent;
		Gtk::ToolButton *m_toolbuttonMoveLeft;
		Gtk::ToolButton *m_toolbuttonMoveRight;
		Gtk::ToolButton *m_toolbuttonUp;
		Gtk::ToolButton *m_toolbuttonDown;
		Gtk::ToolButton *m_toolbuttonRevese;
		Gtk::ToolButton *m_toolbuttonRefresh;
		Glib::RefPtr<Gtk::FileFilter> m_filefilterSound;
		bool m_addrecursive;
		RadioSelected m_radioselected;
		OrderBy m_orderby;
		type_signal_coll_changed m_signal_coll_changed;

		void on_checkbutton_AddRecursive();
		void on_radiobutton(RadioSelected rs);
		void on_comboOrderBy_changed();
		void on_combo_Coll_changed();
		void on_selection_changed_OrderBy();
		void on_selection_changed_Available();
		void on_toolbutton_MoveLeft();
		void on_toolbutton_MoveRight();
		void on_toolbutton_Revese();
		void on_toolbutton_Refresh();
		void on_toolbutton_Up();
		void on_toolbutton_Down();
		Glib::ustring makeAsc(Glib::ustring s);
		Glib::ustring toggleAscDesc(Glib::ustring s);
};

#endif // _DIALOG_NEW_PLAYLIST_H_

