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
#include <gtkmm/volumebutton.h>
#include <xmmsclient/xmmsclient++.h>
#include <xmmsclient/xmmsclient++/list.h>
#include <xmmsclient/xmmsclient++/coll.h>
#include "list-view-format-text.h"
#include "matrix-box.h"
#include "mesage.h"
#include <queue>
#include "dialog-new-playlist.h"
#include "dnc.h" // dialogNewCollection //
#include "dd.h"  // dialogDelete  //
#include "dr.h"  // dialogrename //



#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>


class Main_win: public Gtk::Window 
{
	public:
		Main_win(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder);
		~Main_win();

		// xmms2_sync_client's run stuff 
		void run_sync();

	protected:
		//virtual bool on_button_Pressed(GdkEventButton* event);

	private:
		const Glib::RefPtr<Gtk::Builder>& m_builder;

		// the buttons //
		Gtk::Button *m_buttonConnect;
		Gtk::Button *m_buttonPrevious;
		Gtk::Button *m_buttonRewind;
		Gtk::Button *m_buttonStop;
		Gtk::Button *m_buttonPlayPause;
		Gtk::Button *m_buttonForward;
		Gtk::Button *m_buttonNext;
		Gtk::Button *m_buttonDelete;
		Gtk::Button *m_buttonNewPlayList;
		Gtk::Button *m_buttonNewCollection;
		Gtk::Button *m_buttonRenamePlayList;
		Gtk::Button *m_buttonExit;
		Gtk::Button *m_buttonAbout;
		Gtk::VolumeButton *m_volumebuttonMaster;
		Gtk::VolumeButton *m_volumebuttonLeft;
		Gtk::VolumeButton *m_volumebuttonRight;
		Gtk::ProgressBar *m_progressbar1;

		// Label stuff 
		Gtk::Label *m_labelPlayed;
		Gtk::Label *m_labelLeft;
		Gtk::Label *m_labelTitle;
		Gtk::Label *m_labelArtist;
		Gtk::Label *m_labelAlbum;
		Gtk::Label *m_labelDuration;

		// Dialogs //
		Gtk::AboutDialog *m_aboutdialog1;
		DialogNewPlaylist *m_dialogNewPlaylist;
		DNC *m_dialogNewCollection;
		DD *m_dialogDelete;
		DR *m_dialogrename;

		//custom components I wrote //
		ListViewFormatText *m_listviewformatTextPlaylists;
		MatrixBox *m_MatrixBoxCurrentPlaylist;

		// Menu Stuff //
		Gtk::ImageMenuItem *m_imagemenuitemQuit;
		Gtk::ImageMenuItem *m_imagemenuitemConnect;
		Gtk::ImageMenuItem *m_imagemenuitemNewPlayList;
		Gtk::ImageMenuItem *m_imagemenuitemNewCollection;
		Gtk::ImageMenuItem *m_imagemenuitemPlayPause;
		Gtk::ImageMenuItem *m_imagemenuitemPrevious;
		Gtk::ImageMenuItem *m_imagemenuitemRewind;
		Gtk::ImageMenuItem *m_imagemenuitemStop;
		Gtk::ImageMenuItem *m_imagemenuitemForward;
		Gtk::ImageMenuItem *m_imagemenuitemNext;
		Gtk::ImageMenuItem *m_imagemenuitemRefresh;
		Gtk::ImageMenuItem *m_imagemenuitemAbout;
		
		Gtk::ScrolledWindow *m_scrolledwindowPlaylists;
		Gtk::ScrolledWindow *m_scrolledwindowCurrentPlaylist;
		Gtk::Paned *m_panedBody;
		Gtk::Statusbar *m_statusbar1;
		
		Xmms::Client *xmms2_client;
		Xmms::Client *xmms2_sync_client;
		Glib::RefPtr<Gtk::TextTag> m_ref_bold_tag;
		Glib::RefPtr<Gtk::Action> m_action_PlayPause;
		Glib::RefPtr<Gtk::Action> m_action_Connection;
		//Glib::RefPtr<Gtk::ActionGroup> m_refActionGroupPlayBack;
		int connect_cout;
		unsigned m_connect_retrys;
		std::string m_currentPlaylistName;
		unsigned m_ContextId;
		bool m_isactive;
		unsigned m_current_id;
		bool m_setting_volume_left;
		bool m_setting_volume_right;
		bool m_setting_volume_master;
		double m_duration;
		bool m_dont_disconect;
		bool m_auto_connect;
		bool m_adding_palylist;

		// thread stuff //
		Glib::Threads::Mutex m_mutex;
		Glib::Threads::Cond m_cond_disconnect;
		//Glib::Threads::Cond m_cond_pop;
		// note message is missspelt as mesage I'm //
		// not fixing this error now  too much bother. //
		std::queue<basemesage*> m_queue;
		Glib::Threads::Thread *m_sync_thread;

		// timer connection //
		sigc::connection m_conn;

		Xmms::Playback::Status m_status;
		std::map<std::string, int> keyType;

		// button signal handlers //
		void on_button_Connect();
		void on_button_Previous();
		void on_button_Rewind();
		void on_button_Stop();
		void on_button_PlayPause();
		void on_button_Forward();
		void on_button_Next();
		void on_button_Delete();
		void on_button_Exit();
		void on_button_NewPlayList();
		void on_button_Rename();
		void on_button_NewCollection();
		void on_imagemenuitem_Refresh();
		void on_button_About();

		// VolumeButton signal handler //
		void on_button_VolumeLeftChange(double value);
		void on_button_VolumeRightChange(double value);
		void on_button_VolumeMasterChange(double value);

		// call backs to xmms2 //
		bool handle_status(const Xmms::Playback::Status &st);
		void handle_disconnect();
		bool handle_change(const Xmms::Dict &chg);
		bool handle_update_pos(const Xmms::Dict &posdict);
		bool handle_pls_loaded(const std::string &name);
		bool handle_list(const Xmms::List< int > &list);
		bool handle_current_id(const unsigned int &id);
		bool handle_volume(const Xmms::Dict &d);
		bool handle_playtime(const unsigned int &tme);
		bool handle_mlib_entry_changed(const uint32_t &id);
		bool handle_medialib_info(const Xmms::PropDict &info);
		void get_keys_thunk(std::ostream& stream, const std::string& key, const Xmms::Dict::Variant& v, const std::string& source);
		bool my_get_info( const Xmms::PropDict& propdict );
		bool error_handler(const std::string& function, const std::string& error);

		// a time out handler  //
		bool on_timeout();
		
		void on_Playlists_clicked(Glib::ustring cp);
		void on_Playlists_dblclicked(Glib::ustring cp);
		void on_Playlist_clicked(int id);
		void on_Playlist_dblclicked(int id);
		std::vector<Xmms::Dict> on_coll_changed(Glib::ustring collection_name, std::vector<Glib::ustring> orderby);
		std::vector<Glib::ustring> on_namespace_changed(Glib::ustring _namespace);
		std::vector<Xmms::Dict> on_playlist_coll_changed(Glib::ustring collection_name, std::vector<Glib::ustring> orderby, Glib::ustring _namespace);
		std::vector<std::pair<Glib::ustring, Glib::ustring> > on_newcol_getkeys();
		std::vector<Glib::ustring> on_namespace_delete_changed(DD::NameSpace _ns);
		std::vector<Xmms::Dict> on_playlist_coll_delete_changed(Glib::ustring col_playlst, std::vector<Glib::ustring> orderby, DD::NameSpace _ns);
		std::vector<Glib::ustring> on_namespace_rename_changed(DR::NameSpace _ns);
		std::vector<Xmms::Dict> on_playlist_coll_rename_changed(Glib::ustring col_playlst, std::vector<Glib::ustring> orderby, DR::NameSpace _ns);

		
		void refresh_playlists();
		std::vector<Glib::ustring> get_mediainfo(int id, int highlight = 0);
		void refresh_playlist();
		void update_labels(const Xmms::PropDict &info);
		void disconnect();
		std::string Urlencode(std::string url);
		Glib::ustring mapinttostr(int encoded);
};

#endif // _MAINWIN_H_

