/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * cathy
 * Copyright (C) 2013 Francis (Grizzly) Smit <grizzlysmit@smit.id.au>
 * 
cathy is free software: you can redistribute it and/or modify it
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

#ifndef _MAINWIN_H_
#define _MAINWIN_H_

#include <gtkmm.h>
#include <xmmsclient/xmmsclient++.h>
#include <xmmsclient/xmmsclient++/list.h>
#include <xmmsclient/xmmsclient++/coll.h>
#include "list-view-format-text.h"
#include "matrix-box.h"


#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>


class Main_win: public Gtk::Window 
{
	public:
		Main_win(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder);
		~Main_win();

	protected:
		virtual bool on_button_Pressed(GdkEventButton* event);

	private:
		const Glib::RefPtr<Gtk::Builder>& m_builder;
		Gtk::Button *m_buttonConnect;
		ListViewFormatText *m_listviewformatTextPlaylists;
		MatrixBox *m_listviewformatTextCurrentPlaylist;
		Gtk::Button *m_buttonExit;
		Gtk::ImageMenuItem *m_imagemenuitemQuit;
		Gtk::ScrolledWindow *m_scrolledwindowPlaylists;
		Gtk::ScrolledWindow *m_scrolledwindowCurrentPlaylist;
		Gtk::Paned *m_panedBody;
		Xmms::Client xmms2_client;
		Glib::RefPtr<Gtk::TextTag> m_ref_bold_tag;
		int connect_cout;
		unsigned m_connect_retrys;
		std::string m_currentPlaylistName;

		void on_button_Connect();
		void refresh_playlists();
		std::vector<Glib::ustring> get_mediainfo(int id);
		void refresh_playlist();
		void on_button_Exit();
		void on_Playlists_clicked(Glib::ustring cp);
		void on_Playlists_dblclicked(Glib::ustring cp);
		void on_Playlist_clicked(int id);
		void on_Playlist_dblclicked(int id);
};

#endif // _MAINWIN_H_

