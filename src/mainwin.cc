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

#include "mainwin.h"
#include <vector>
#include <exception>
#include <boost/format.hpp>




Main_win::Main_win(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder)
  : Gtk::Window(cobject), m_builder(builder), m_buttonConnect(0), m_buttonPrevious(0),
    m_buttonRewind(0), m_buttonStop(0), m_buttonPlayPause(0), m_buttonForward(0), 
    m_buttonNext(0), m_buttonDelete(0), m_buttonNew(0), m_actionNewCollection(0), 
    m_buttonExit(0), m_buttonHelp(0), 
    m_volumebuttonMaster(0), m_volumebuttonLeft(0), m_volumebuttonRight(0), m_progressbar1(0), 
    m_labelPlayed(0), m_labelLeft(0), m_labelTitle(0), m_labelArtist(0), m_labelAlbum(0), 
    m_labelDuration(0), 
    m_aboutdialog1(0), m_dialogNewPlaylist(0), 
    m_listviewformatTextPlaylists(0), m_MatrixBoxCurrentPlaylist(0), m_imagemenuitemQuit(0), 
    m_imagemenuitemConnect(0), m_imagemenuitemNew(0), 
    m_scrolledwindowPlaylists(0), m_scrolledwindowCurrentPlaylist(0), m_panedBody(0), 
    m_statusbar1(0), 
    //m_eventboxPlaylists(0), m_eventboxCurrentPlaylist(0), 
    xmms2_client("cathy"), xmms2_sync_client("cathy_sync"), connect_cout(0), 
    m_connect_retrys(2), m_setting_volume_left(false), m_setting_volume_right(false), 
    m_setting_volume_master(false), m_duration(0)
{
	m_status = Xmms::Playback::STOPPED;

	// button stuff //
	m_builder->get_widget("buttonConnect", m_buttonConnect);
	if(m_buttonConnect){
		m_buttonConnect->signal_clicked().connect( sigc::mem_fun(*this, &Main_win::on_button_Connect) );
	}
	m_builder->get_widget("buttonPrevious", m_buttonPrevious);
	if(m_buttonPrevious){
		m_buttonPrevious->signal_clicked().connect( sigc::mem_fun(*this, &Main_win::on_button_Previous) );
	}
	m_builder->get_widget("buttonRewind", m_buttonRewind);
	if(m_buttonRewind){
		m_buttonRewind->signal_clicked().connect( sigc::mem_fun(*this, &Main_win::on_button_Rewind) );
	}
	m_builder->get_widget("buttonStop", m_buttonStop);
	if(m_buttonStop){
		m_buttonStop->signal_clicked().connect( sigc::mem_fun(*this, &Main_win::on_button_Stop) );
	}
	m_builder->get_widget("buttonPlayPause", m_buttonPlayPause);
	if(m_buttonPlayPause){
		m_buttonPlayPause->signal_clicked().connect( sigc::mem_fun(*this, &Main_win::on_button_PlayPause) );
	}
	m_builder->get_widget("buttonForward", m_buttonForward);
	if(m_buttonForward){
		m_buttonForward->signal_clicked().connect( sigc::mem_fun(*this, &Main_win::on_button_Forward) );
	}
	m_builder->get_widget("buttonNext", m_buttonNext);
	if(m_buttonNext){
		m_buttonNext->signal_clicked().connect( sigc::mem_fun(*this, &Main_win::on_button_Next) );
	}
	m_builder->get_widget("buttonDelete", m_buttonDelete);
	if(m_buttonDelete){
		m_buttonDelete->signal_clicked().connect( sigc::mem_fun(*this, &Main_win::on_button_Delete) );
	}
	m_builder->get_widget("buttonNew", m_buttonNew);
	if(m_buttonNew){
		m_buttonNew->signal_clicked().connect( sigc::mem_fun(*this, &Main_win::on_button_New) );
	}
	// m_actionNewCollection //
	m_builder->get_widget("actionNewCollection", m_actionNewCollection);
	if(m_actionNewCollection){
		m_actionNewCollection->signal_activate().connect( sigc::mem_fun(*this, &Main_win::on_action_NewCollection) );
	}
	m_builder->get_widget("buttonExit", m_buttonExit);
	if(m_buttonExit){
		m_buttonExit->signal_clicked().connect( sigc::mem_fun(*this, &Main_win::on_button_Exit) );
	}
	m_builder->get_widget("buttonHelp", m_buttonHelp);
	if(m_buttonHelp){
		m_buttonHelp->signal_clicked().connect( sigc::mem_fun(*this, &Main_win::on_button_Help) );
	}

	// volume control  //
	m_builder->get_widget("volumebuttonMaster", m_volumebuttonMaster);
	if(m_volumebuttonMaster){
		m_volumebuttonMaster->signal_value_changed().connect( sigc::mem_fun(*this, &Main_win::on_button_VolumeMasterChange) );
	}
	m_builder->get_widget("volumebuttonLeft", m_volumebuttonLeft);
	if(m_volumebuttonLeft){
		m_volumebuttonLeft->signal_value_changed().connect( sigc::mem_fun(*this, &Main_win::on_button_VolumeLeftChange) );
		m_volumebuttonLeft->set_orientation(Gtk::ORIENTATION_HORIZONTAL);
	}
	m_builder->get_widget("volumebuttonRight", m_volumebuttonRight);
	if(m_volumebuttonRight){
		m_volumebuttonRight->signal_value_changed().connect( sigc::mem_fun(*this, &Main_win::on_button_VolumeRightChange) );
		m_volumebuttonRight->set_orientation(Gtk::ORIENTATION_HORIZONTAL);
	}

	// progressbar //
	m_builder->get_widget("progressbar1", m_progressbar1);
	if(m_progressbar1){
		//m_progressbar1->signal_value_changed().connect( sigc::mem_fun(*this, &Main_win::on_button_VolumeMasterChange) );
	}

	// Label stuff //
	m_builder->get_widget("labelPlayed", m_labelPlayed);
	if(m_labelPlayed){
		//m_progressbar1->signal_value_changed().connect( sigc::mem_fun(*this, &Main_win::on_button_VolumeMasterChange) );
	}
	m_builder->get_widget("labelLeft", m_labelLeft);
	if(m_labelLeft){
		//m_progressbar1->signal_value_changed().connect( sigc::mem_fun(*this, &Main_win::on_button_VolumeMasterChange) );
	}
	m_builder->get_widget("labelTitle", m_labelTitle);
	if(m_labelTitle){
		//m_progressbar1->signal_value_changed().connect( sigc::mem_fun(*this, &Main_win::on_button_VolumeMasterChange) );
	}
	m_builder->get_widget("labelArtist", m_labelArtist);
	if(m_labelArtist){
		//m_progressbar1->signal_value_changed().connect( sigc::mem_fun(*this, &Main_win::on_button_VolumeMasterChange) );
	}
	m_builder->get_widget("labelAlbum", m_labelAlbum);
	if(m_labelAlbum){
		//m_progressbar1->signal_value_changed().connect( sigc::mem_fun(*this, &Main_win::on_button_VolumeMasterChange) );
	}
	m_builder->get_widget("labelDuration", m_labelDuration);
	if(m_labelDuration){
		//m_progressbar1->signal_value_changed().connect( sigc::mem_fun(*this, &Main_win::on_button_VolumeMasterChange) );
	}

	// Menu Stuff //
	m_builder->get_widget("imagemenuitemQuit", m_imagemenuitemQuit);
	if(m_imagemenuitemQuit){
		m_imagemenuitemQuit->signal_activate().connect( sigc::mem_fun(*this, &Main_win::on_button_Exit) );
	}
	m_builder->get_widget("imagemenuitemConnect", m_imagemenuitemConnect);
	if(m_imagemenuitemConnect){
		m_imagemenuitemConnect->signal_activate().connect( sigc::mem_fun(*this, &Main_win::on_button_Connect) );
	}
	m_builder->get_widget("imagemenuitemNew", m_imagemenuitemNew);
	if(m_imagemenuitemNew){
		m_imagemenuitemNew->signal_activate().connect( sigc::mem_fun(*this, &Main_win::on_button_New) );
	}

	// The Paned comopnet and custom sub componets //
	m_builder->get_widget("panedBody", m_panedBody);
	if(m_panedBody){
		m_scrolledwindowPlaylists = new Gtk::ScrolledWindow();
		m_scrolledwindowCurrentPlaylist = new Gtk::ScrolledWindow();
		m_panedBody->pack1(*m_scrolledwindowPlaylists, true, true);
		m_panedBody->pack2(*m_scrolledwindowCurrentPlaylist, true, true);
		m_listviewformatTextPlaylists = new ListViewFormatText();
		m_MatrixBoxCurrentPlaylist = new MatrixBox(5);
		//m_eventboxPlaylists = new Gtk::EventBox();
		//m_eventboxCurrentPlaylist = new Gtk::EventBox();
		m_scrolledwindowCurrentPlaylist->add(*m_MatrixBoxCurrentPlaylist);
		//m_eventboxCurrentPlaylist->add(*m_MatrixBoxCurrentPlaylist);
		m_listviewformatTextPlaylists->set_tooltip_text("Playlists\nclick to select\ndouble click to make default");
		std::cout << "got here[" << __LINE__ << ']' << std::endl;
		//m_eventboxPlaylists->add(*m_listviewformatTextPlaylists);
		m_scrolledwindowPlaylists->add(*m_listviewformatTextPlaylists);

		// connect the signals //
		m_listviewformatTextPlaylists->signal_clicked().connect(sigc::mem_fun(*this, &Main_win::on_Playlists_clicked));
		m_listviewformatTextPlaylists->signal_dblclicked().connect(sigc::mem_fun(*this, &Main_win::on_Playlists_dblclicked));

		m_MatrixBoxCurrentPlaylist->signal_clicked().connect(sigc::mem_fun(*this, &Main_win::on_Playlist_clicked));
		m_MatrixBoxCurrentPlaylist->signal_dblclicked().connect(sigc::mem_fun(*this, &Main_win::on_Playlist_dblclicked));
		

		// tooltip stuff //
		m_listviewformatTextPlaylists->set_tooltip_markup("<b>Playlists</b>");
		m_listviewformatTextPlaylists->set_has_tooltip(true);
		
		m_MatrixBoxCurrentPlaylist->set_tooltip_markup("<b>Current Playlist's contents</b>");
		m_MatrixBoxCurrentPlaylist->set_has_tooltip(true);
		
		std::vector<Glib::ustring> headers;
		headers.insert(headers.end(), "<b>Title</b>");
		headers.insert(headers.end(), "<b>Artist</b>");
		headers.insert(headers.end(), "<b>Album</b>");
		headers.insert(headers.end(), "<b>Duration</b>");
		headers.insert(headers.end(), "<b>Current</b>");
		m_MatrixBoxCurrentPlaylist->set_headings(headers);
		// show the widgits //
		//m_listviewformatTextPlaylists->show();
		//m_scrolledwindowPlaylists->show_all();
		//m_panedBody->show_all_children();
		show_all_children();
		std::cout << "got here[" << __LINE__ << ']' << std::endl;
	}
	m_builder->get_widget("statusbar1", m_statusbar1);
	if(m_statusbar1){
		//m_statusbar1->signal_activate().connect( sigc::mem_fun(*this, &Main_win::on_button_Exit) );
		m_ContextId = m_statusbar1->get_context_id("Cathy");
	}

	// Dialogs //
	m_builder->get_widget("aboutdialog1", m_aboutdialog1);
	if(m_aboutdialog1){
		//m_aboutdialog1->signal_activate().connect( sigc::mem_fun(*this, &Main_win::on_button_Exit) );
	}
	m_builder->get_widget_derived("dialogNewPlaylist", m_dialogNewPlaylist);
	if(m_dialogNewPlaylist){
		m_dialogNewPlaylist->signal_coll_changed().connect( sigc::mem_fun(*this, &Main_win::on_coll_changed) );
	}
}

Main_win::~Main_win(){
	delete m_listviewformatTextPlaylists;
	delete m_MatrixBoxCurrentPlaylist;
	delete m_scrolledwindowPlaylists;
	delete m_scrolledwindowCurrentPlaylist;
}


void Main_win::on_button_Connect()
{
	std::cout << __FILE__ << " got here[" << __LINE__ << ']' << std::endl;
    for(int i = 0; i < m_connect_retrys;i++){
		std::cout << __FILE__ << " got here[" << __LINE__ << ']' << std::endl;
		try{
			xmms2_client.connect( std::getenv("XMMS_PATH") );
			m_conn = Glib::signal_timeout().connect(sigc::mem_fun(*this, &Main_win::on_timeout), 100);
			if(xmms2_client.playback.getStatus() == Xmms::Playback::PLAYING){
				Xmms::Dict d = xmms2_client.playback.volumeGet();
				std::cout << __FILE__ << " got here[" << __LINE__ << ']' << std::endl;
				if(d.contains("master")){
					m_setting_volume_master = true;
					m_volumebuttonMaster->set_value(static_cast<double>(d.get<int32_t>("master"))/100.0);
					m_setting_volume_master = false;
				}
				if(d.contains("left")){
					m_setting_volume_left = true;
					m_volumebuttonLeft->set_value(static_cast<double>(d.get<int32_t>("left"))/100.0);
					m_setting_volume_left = false;
				}else if(d.contains("master")){
					m_setting_volume_left = true;
					m_volumebuttonLeft->set_value(static_cast<double>(d.get<int32_t>("master"))/100.0);
					m_setting_volume_left = false;
				}
				if(d.contains("right")){
					m_setting_volume_right = true;
					m_volumebuttonRight->set_value(static_cast<double>(d.get<int32_t>("right"))/100.0);
					m_setting_volume_right = false;
				}else if(d.contains("master")){
					m_setting_volume_right = true;
					m_volumebuttonRight->set_value(static_cast<double>(d.get<int32_t>("master"))/100.0);
					m_setting_volume_right = false;
				}
				std::cout << __FILE__ << " got here[" << __LINE__ << ']' << std::endl;
				unsigned tme = xmms2_client.playback.getPlaytime();
				int id = xmms2_client.playback.currentID();
				Xmms::PropDict info = xmms2_client.medialib.getInfo( id );
				if(info.contains("duration")){
					m_duration = boost::get< int >( info["duration"] );
					m_progressbar1->set_fraction(tme/m_duration);
				}else{
					m_progressbar1->pulse();
				}
				update_labels(info);
			}
			
			// thread stuff //
			m_sync_thread = Glib::Threads::Thread::create(sigc::mem_fun(this, &Main_win::run_sync));
			
			std::cout << __FILE__ << " got here[" << __LINE__ << ']' << std::endl;
			refresh_playlists();
			std::cout << __FILE__ << " got here[" << __LINE__ << ']' << std::endl;
			refresh_playlist();
			std::cout << __FILE__ << " got here[" << __LINE__ << ']' << std::endl;
			
			m_statusbar1->push("Cathy is connected to xmms2d", m_ContextId);

			std::cout << "connect count == " << ++connect_cout << std::endl;
			break;
		}
		catch(std::exception &e){
			std::cout << e.what() << std::endl;
			system("/usr/bin/xmms2-launcher"); // force xmms2d to start //
		}
	}
}

void Main_win::run_sync()
{

	xmms2_sync_client.connect(std::getenv("XMMS_PATH"));
	xmms2_sync_client.playback.getStatus()(Xmms::bind(&Main_win::handle_status, this));
	xmms2_sync_client.playback.broadcastStatus()(Xmms::bind(&Main_win::handle_status, this));

	xmms2_sync_client.setDisconnectCallback(boost::bind(&Main_win::handle_disconnect, this));
	m_status = xmms2_sync_client.playback.getStatus();
	xmms2_sync_client.playlist.broadcastChanged()(Xmms::bind(&Main_win::handle_change, this));
	xmms2_sync_client.playlist.broadcastCurrentPos()(Xmms::bind(&Main_win::handle_update_pos, this));

	xmms2_sync_client.playlist.broadcastLoaded()(Xmms::bind(&Main_win::handle_pls_loaded, this));
	xmms2_sync_client.playback.broadcastCurrentID()(Xmms::bind(&Main_win::handle_current_id, this));
	xmms2_sync_client.playback.currentID()(Xmms::bind(&Main_win::handle_current_id, this));
	xmms2_sync_client.playback.broadcastVolumeChanged()(Xmms::bind (&Main_win::handle_volume, this));
	xmms2_sync_client.playback.signalPlaytime()(Xmms::bind(&Main_win::handle_playtime, this));
	xmms2_sync_client.playback.getPlaytime()(Xmms::bind(&Main_win::handle_playtime, this));
	xmms2_sync_client.medialib.broadcastEntryChanged()(Xmms::bind(&Main_win::handle_mlib_entry_changed, this));
	for(int i = 0; i < 10; i++){
		try{
			xmms2_sync_client.getMainLoop().run();
		}
		catch(std::exception &e){
			std::cout << __FILE__ << "[" << __LINE__ 
				<< "] Error in " << __PRETTY_FUNCTION__ << ": " 
				<< e.what() << std::endl;
		}
	}
}

bool Main_win::on_timeout()
{
	// code goes here //
	try{
		Glib::Threads::Mutex::Lock lock(m_mutex);
		if(m_duration <= 0 && xmms2_client.playback.getStatus() == Xmms::Playback::PLAYING){
			std::cout << __FILE__ << '[' << __LINE__ 
				<< "] m_duration <= 0 && xmms2_client.playback.getStatus()"
				<< " == Xmms::Playback::PLAYING; m_duration == " 
				<< m_duration << std::endl;
			int id = xmms2_client.playback.currentID();
			Xmms::PropDict info = xmms2_client.medialib.getInfo( id );
			update_labels(info);
		}
		if(m_queue.empty()) return true; // nothing to do //
		while(!m_queue.empty()){
			basemesage *msg = m_queue.front();
			switch(msg->get_message_type()){
				case(basemesage::disconnect):
					m_statusbar1->push("Cathy is disconnected from xmms2d", m_ContextId);
					//on_button_Connect();
					break;
				case(basemesage::status):
				{
					std::cout << __FILE__ << "[" << __LINE__ << "] got here case(basemesage::status): " << std::endl;
					mesage<Xmms::Playback::Status> *the_mesage;
					Xmms::Playback::Status st;
					try{
						refresh_playlists();
						
						the_mesage = dynamic_cast<mesage<Xmms::Playback::Status>*>(msg);
						st = the_mesage->get_arg();
					}
					catch(std::exception &e){
						std::cout << __FILE__ << "[" << __LINE__ 
							<< "] Error in dynamic_cast<mesage<Xmms::Playback::Status>*>(msg): " 
							<< e.what() << std::endl;
						m_queue.pop();
						delete msg;
						return true;
					}
					//mesage<Xmms::Playback::Status> *the_mesage = dynamic_cast<mesage<Xmms::Playback::Status>*>(msg);
					m_status = st;

					if (st == Xmms::Playback::STOPPED ||
					    st == Xmms::Playback::PAUSED){
						m_buttonPlayPause->set_label("gtk-media-play");
					}else{
						m_buttonPlayPause->set_label("gtk-media-pause");
						std::cout << __FILE__ << " got here[" << __LINE__ << ']' << std::endl;
						Xmms::Dict d = xmms2_client.playback.volumeGet();
						std::cout << __FILE__ << '[' << __LINE__ 
							<< "]  m_duration == " << m_duration << std::endl;
						int id = xmms2_client.playback.currentID();
						Xmms::PropDict info = xmms2_client.medialib.getInfo( id );
						update_labels(info);
						if(d.contains("left")){
							m_setting_volume_left = true;
							m_volumebuttonLeft->set_value(static_cast<double>(d.get<int32_t>("left"))/100.0);
							m_setting_volume_left = false;
						}else if(d.contains("master")){
							m_setting_volume_left = true;
							m_volumebuttonLeft->set_value(static_cast<double>(d.get<int32_t>("master"))/100.0);
							m_setting_volume_left = false;
						}
						if(d.contains("right")){
							m_setting_volume_right = true;
							m_volumebuttonRight->set_value(static_cast<double>(d.get<int32_t>("right"))/100.0);
							m_setting_volume_right = false;
						}else if(d.contains("master")){
							m_setting_volume_right = true;
							m_volumebuttonRight->set_value(static_cast<double>(d.get<int32_t>("master"))/100.0);
							m_setting_volume_right = false;
						}
						if(d.contains("master")){
							m_setting_volume_master = true;
							m_volumebuttonMaster->set_value(static_cast<double>(d.get<int32_t>("master"))/100.0);
							m_setting_volume_master = false;
						}
						std::cout << __FILE__ << " got here[" << __LINE__ << ']' << std::endl;
						//xmms2_sync_client.playback.broadcastVolumeChanged()(Xmms::bind (&Main_win::handle_volume, this));
						// TODO: code for monitoring song play //
					}
					break;
				}
				case(basemesage::plchange):
				{
					mesage<Xmms::Dict> *the_mesage;
					Xmms::Dict chg;
					int32_t change;
					try{
						refresh_playlists();
						
						the_mesage = dynamic_cast<mesage<Xmms::Dict>*>(msg);
						chg = the_mesage->get_arg();
						change = chg.get<int32_t>("type");
					}
					catch(std::exception &e){
						std::cout << __FILE__ << "[" << __LINE__ 
							<< "] Error in dynamic_cast<mesage<Xmms::Dict>*>(msg): " 
							<< e.what() << std::endl;
						m_queue.pop();
						delete msg;
						return true;
					}
					int32_t pos = 0, npos = 0;
					uint32_t id = 0;
					Glib::ustring s;

					if(chg.contains("position")){
						pos = chg.get<int32_t>("position");
					}

					if(chg.contains("id")){
						id = chg.get<int32_t>("id");
						//int id = xmms2_client.playback.currentID();
						Xmms::PropDict info = xmms2_client.medialib.getInfo( id );
						update_labels(info);
					}
					std::cout << __FILE__ << '[' << __LINE__ << "] id == " << id << std::endl;

					if (chg.contains("name")) {
						s = Glib::ustring(chg.get<std::string>("name"));
					}

					if(s != m_currentPlaylistName){
						std::cout << __FILE__ << '[' << __LINE__ 
							<< "] s != m_currentPlaylistName; s == " << s << std::endl;
						std::cout << __FILE__ << '[' << __LINE__ << "] m_currentPlaylistName == " 
							<< m_currentPlaylistName << std::endl;
						return true;
					}

					switch(change){
						case XMMS_PLAYLIST_CHANGED_ADD:
							std::cout << __FILE__ << '[' << __LINE__ << "] XMMS_PLAYLIST_CHANGED_ADD id == " << id << std::endl;
							refresh_playlist();
							break;
						case XMMS_PLAYLIST_CHANGED_INSERT:
							std::cout << __FILE__ << '[' << __LINE__ << "] XMMS_PLAYLIST_CHANGED_INSERT id == " << id << std::endl;
							refresh_playlist();
							break;
						case XMMS_PLAYLIST_CHANGED_MOVE:
							npos = chg.get<int32_t>("newposition");

							std::cout << __FILE__ << '[' << __LINE__ << "] XMMS_PLAYLIST_CHANGED_MOVE id == " << id << std::endl;
							refresh_playlist();

							if (pos < npos && pos)
								pos --;

							break;
						case XMMS_PLAYLIST_CHANGED_REMOVE:
							std::cout << __FILE__ << '[' << __LINE__ << "] XMMS_PLAYLIST_CHANGED_MOVE id == " << id << std::endl;
							refresh_playlist();
							break;
						case XMMS_PLAYLIST_CHANGED_SHUFFLE:
						case XMMS_PLAYLIST_CHANGED_SORT:
						case XMMS_PLAYLIST_CHANGED_CLEAR:
							std::cout << __FILE__ << '[' << __LINE__ << "] XMMS_PLAYLIST_CHANGED_SHUFFLE id == " << id << std::endl;
							refresh_playlist();
							if(xmms2_client.playback.getStatus() == Xmms::Playback::PLAYING){
								xmms2_sync_client.playlist.listEntries(m_currentPlaylistName)(Xmms::bind(&Main_win::handle_list, this));
							}
							break;
					}
					break;
				}
				case(basemesage::update_pos):
				{
					mesage<Xmms::Dict> *the_mesage;
					Xmms::Dict posdict;
					Glib::ustring changed_pl;
					try{
						the_mesage = dynamic_cast<mesage<Xmms::Dict>*>(msg);
						//mesage<Xmms::Dict> *the_mesage = dynamic_cast<mesage<Xmms::Dict>*>(msg);
						posdict = the_mesage->get_arg();
						changed_pl = posdict.get<std::string>("name");
					}
					catch(std::exception &e){
						std::cout << __FILE__ << "[" << __LINE__ 
							<< "] Error in dynamic_cast<mesage<Xmms::Dict>*>(msg): " 
							<< e.what() << std::endl;
						m_queue.pop();
						delete msg;
						return true;
					}
					if(changed_pl == m_currentPlaylistName){
						int32_t pos = posdict.get<int32_t>("position");
						//m_current_pos = pos;
						refresh_playlist();
					}
					/*				int id = xmms2_client.playback.currentID();
					Xmms::PropDict info = xmms2_client.medialib.getInfo( id );
					update_labels(info);*/
					break;
				}
				case(basemesage::pls_loaded):
				{
					mesage<std::string> *the_mesage;
					std::string name;
					try{
						the_mesage = dynamic_cast<mesage<std::string>*>(msg);
						name = the_mesage->get_arg();
					}
					catch(std::exception &e){
						std::cout << __FILE__ << "[" << __LINE__ 
							<< "] Error in dynamic_cast<mesage<std::string>*>(msg): " 
							<< e.what() << std::endl;
						m_queue.pop();
						delete msg;
						return true;
					}
					if(name == m_currentPlaylistName && xmms2_client.playback.getStatus() == Xmms::Playback::PLAYING){
						xmms2_sync_client.playlist.listEntries(name)(Xmms::bind (&Main_win::handle_list, this));

						refresh_playlist();
					}

					break;
				}
				case(basemesage::list):
				{
					std::cout << __FILE__ << "[" << __LINE__ << "] got here case(basemesage::list): " << std::endl;
					mesage<Xmms::List<int> > *the_mesage;
					std::vector< int >  list;
					try{
						the_mesage = dynamic_cast<mesage<Xmms::List<int> >*>(msg);
						Xmms::List< int >  tlist = the_mesage->get_arg();
						list.insert(list.end(), tlist.begin(), tlist.end());
					}
					catch(std::exception &e){
						std::cout << __FILE__ << "[" << __LINE__ 
							<< "] Error in dynamic_cast<mesage<Xmms::List<int> >*>(msg): " 
							<< e.what() << std::endl;
						m_queue.pop();
						delete msg;
						return true;
					}
					std::cout << __FILE__ << "[" << __LINE__ << "] got here case(basemesage::list): " << std::endl;
					refresh_playlist();
					std::cout << __FILE__ << "[" << __LINE__ << "] got here case(basemesage::list): " << std::endl;
					break;
				}
				case(basemesage::current_id):
				{
					mesage<int> *the_mesage;
					int  id;
					try{
						the_mesage = dynamic_cast<mesage<int>*>(msg);
						id = the_mesage->get_arg();
					}
					catch(std::exception &e){
						std::cout << __FILE__ << "[" << __LINE__ 
							<< "] Error in dynamic_cast<mesage<int>*>(msg): " 
							<< e.what() << std::endl;
						m_queue.pop();
						delete msg;
						return true;
					}
					//mesage<int> *the_mesage = dynamic_cast<mesage<int>*>(msg);
					std::cout << __FILE__ << "[" << __LINE__ << "] got here case(basemesage::current_id): " << std::endl;
					m_current_id = id;
					refresh_playlist();
					std::cout << __FILE__ << "[" << __LINE__ << "] got here case(basemesage::current_id): " << std::endl;
					break;
				}
				case(basemesage::handle_volume):
				{
					mesage<Xmms::Dict> *the_mesage;
					Xmms::Dict  d;
					try{
						the_mesage = dynamic_cast<mesage<Xmms::Dict>*>(msg);
						d = the_mesage->get_arg();
					}
					catch(std::exception &e){
						std::cout << __FILE__ << "[" << __LINE__ 
							<< "] Error in dynamic_cast<mesage<Xmms::Dict>*>(msg): " 
							<< e.what() << std::endl;
						m_queue.pop();
						delete msg;
						return true;
					}
					//mesage<Xmms::Dict> *the_mesage = dynamic_cast<mesage<Xmms::Dict>*>(msg);
					if(m_setting_volume_left || m_setting_volume_right || m_setting_volume_master){
						std::cout << __FILE__ << "[" << __LINE__ << "] got here case(basemesage::handle_volume): " << std::endl;
						return true;
					}
					if(d.contains("master")){
						m_setting_volume_master = true;
						double master = d.get<int32_t>("master");
						std::cout << __FILE__ << "[" << __LINE__ << "] got here case(basemesage::handle_volume): master == " << master << std::endl;
						m_volumebuttonMaster->set_value(master/100.0);
						m_setting_volume_master = false;
						//m_channels = 1;
					}
					if(d.contains("left")){
						m_setting_volume_left = true;
						double left = d.get<int32_t>("left");
						std::cout << __FILE__ << "[" << __LINE__ << "] got here case(basemesage::handle_volume): left == " << left << std::endl;
						m_volumebuttonLeft->set_value(left/100.0);
						m_setting_volume_left = false;
					}
					if(d.contains("right")){
						m_setting_volume_right = true;
						double right = d.get<int32_t>("right");
						std::cout << __FILE__ << "[" << __LINE__ << "] got here case(basemesage::handle_volume): right == " << right << std::endl;
						m_volumebuttonRight->set_value(right/100.0);
						m_setting_volume_right = false;
					}
					//m_channels = 2;
					break;
				}
				case(basemesage::handle_playtime):
				{
					mesage<unsigned> *the_mesage;
					unsigned  tme;
					try{
						the_mesage = dynamic_cast<mesage<unsigned>*>(msg);
						tme = the_mesage->get_arg();
					}
					catch(std::exception &e){
						std::cout << __FILE__ << "[" << __LINE__ 
							<< "] Error in dynamic_cast<mesage<unsigned>*>(msg): " 
							<< e.what() << std::endl;
						m_queue.pop();
						delete msg;
						return true;
					}
					//mesage<unsigned> *the_mesage = dynamic_cast<mesage<unsigned>*>(msg);
					// TODO: fill this in %%%%%%%%%%%%%%% //
					if(m_duration > 0){
						m_progressbar1->set_fraction(tme/m_duration);
						long long pt = tme, tleft = m_duration - tme;
						if(tleft < 0) tleft = 0;
						{
							double secs = (pt % 60000)/1000.0;
							pt /= 60000;
							int mins = pt % 60;
							pt /= 60;
							int hours = pt;
							boost::format bf("%02d:%02d:%06.3f");
							bf % hours % mins % secs;
							//std::cout << bf << ')' << std::endl;
							std::string s = bf.str();
							m_labelPlayed->set_text(s);
						}
						{
							double secs = (tleft % 60000)/1000.0;
							tleft /= 60000;
							int mins = tleft % 60;
							tleft /= 60;
							int hours = tleft;
							boost::format bf("-%02d:%02d:%06.3f");
							bf % hours % mins % secs;
							//std::cout << bf << ')' << std::endl;
							std::string s = bf.str();
							m_labelLeft->set_text(s);
						}
					}else{
						m_progressbar1->pulse();
					}
					break;
				}
				case(basemesage::handle_medialib_info):
				{
					mesage<Xmms::PropDict> *the_mesage;
					try{
						the_mesage = dynamic_cast<mesage<Xmms::PropDict>*>(msg);
						Xmms::PropDict  info = the_mesage->get_arg();
						update_labels(info);
					}
					catch(std::exception &e){
						std::cout << __FILE__ << "[" << __LINE__ 
							<< "] Error in dynamic_cast<mesage<Xmms::PropDict>*>(msg): " 
							<< e.what() << std::endl;
						m_queue.pop();
						delete msg;
						return true;
					}
					//mesage<Xmms::PropDict> *the_mesage = dynamic_cast<mesage<Xmms::PropDict>*>(msg);
					break;
				}
				case(basemesage::handle_mlib_entry_changed):
				{
					mesage<uint32_t> *the_mesage;
					uint32_t  id;
					try{
						the_mesage = dynamic_cast<mesage<uint32_t>*>(msg);
						id = the_mesage->get_arg();
					}
					catch(std::exception &e){
						std::cout << __FILE__ << "[" << __LINE__ 
							<< "] Error in dynamic_cast<mesage<uint32_t>*>(msg): " 
							<< e.what() << std::endl;
						m_queue.pop();
						delete msg;
						return true;
					}
					//mesage<uint32_t> *the_mesage = dynamic_cast<mesage<uint32_t>*>(msg);
					break;
				}
			}
			m_queue.pop();
			delete msg;
		}
	}
	catch(std::exception &e){
		std::cout << __FILE__ << "[" << __LINE__ 
			<< "] Error in on_timeout: " 
			<< e.what() << std::endl;
	}
	return true;
}

void Main_win::update_labels(const Xmms::PropDict &info)
{
	try{
		if(info.contains("title")){
			m_labelTitle->set_text(boost::get<std::string>(info["title"]));
		}
		if(info.contains("album")){
			m_labelAlbum->set_text(boost::get<std::string>(info["album"]));
		}
		if(info.contains("artist")){
			m_labelArtist->set_text(boost::get<std::string>(info["artist"]));
		}
		if(info.contains("duration")){
			int duration = boost::get< int >( info["duration"] );
			m_duration = duration;
			//std::cout << duration << "  (" << std::flush;
			double secs = (duration % 60000)/1000.0;
			duration /= 60000;
			int mins = duration % 60;
			duration /= 60;
			int hours = duration;
			boost::format bf("%02d:%02d:%06.3f");
			bf % hours % mins % secs;
			//std::cout << bf << ')' << std::endl;
			std::string s = bf.str();
			m_labelDuration->set_text(s);
		}
	}
	catch(std::exception &e){
		std::cout << __FILE__ << "[" << __LINE__ 
			<< "] Error in " << __PRETTY_FUNCTION__ << ": " 
			<< e.what() << std::endl;
	}
}

void Main_win::handle_disconnect()
{
	Glib::Threads::Mutex::Lock lock (m_mutex);
	try{
		basemesage *msg = new mesage<int>(basemesage::disconnect, 0);
		m_queue.push(msg);
	}
	catch(std::exception &e){
		std::cout << __FILE__ << "[" << __LINE__ 
			<< "] Error in handle_disconnect: " 
			<< e.what() << std::endl;
	}
}


void Main_win::on_button_Previous()
{
	try{
		m_status = xmms2_client.playback.getStatus();
		if(m_status == Xmms::Playback::PLAYING ||
		   m_status == Xmms::Playback::PAUSED){
			xmms2_client.playback.stop();
			xmms2_client.playlist.setNextRel(-1);
			xmms2_client.playback.start();
		}else{
			xmms2_client.playlist.setNextRel(-1);
		}
		//refresh_playlist(); taken care off by xmms2_sync_client's callbacks //
	}
	catch(std::exception &e){
		std::cout << __FILE__ << "[" << __LINE__ 
			<< "] Error in " << __PRETTY_FUNCTION__ << ": " 
			<< e.what() << std::endl;
	}
}

void Main_win::on_button_Rewind()
{
	try{
		std::cout << __FILE__ << "[" << __LINE__ << "] got here on_button_Rewind()" << std::endl;
		m_status = xmms2_client.playback.getStatus();
		if(m_status == Xmms::Playback::PLAYING ||
		   m_status == Xmms::Playback::PAUSED){
			//unsigned p = xmms2_client.playback.getPlaytime();
			//xmms2_client.playback.stop();
			//xmms2_client.playback.seekMs(p - 10000);
			xmms2_client.playback.seekSamplesRel(-10000);
			//xmms2_client.playback.start();
		}else{
			//unsigned p = xmms2_client.playback.getPlaytime();
			//xmms2_client.playback.seekMs(p - 10000);
			xmms2_client.playback.seekSamplesRel(-10000);
		}
	}
	catch(std::exception &e){
		std::cout << __FILE__ << "[" << __LINE__ 
			<< "] Error in " << __PRETTY_FUNCTION__ << ": " 
			<< e.what() << std::endl;
	}
}

void Main_win::on_button_Stop()
{
	try{
		xmms2_client.playback.stop();
	}
	catch(std::exception &e){
		std::cout << __FILE__ << "[" << __LINE__ 
			<< "] Error in " << __PRETTY_FUNCTION__ << ": " 
			<< e.what() << std::endl;
	}
}

void Main_win::on_button_PlayPause()
{
	try{
		m_status = xmms2_client.playback.getStatus();
		if(m_status == Xmms::Playback::PLAYING){
			xmms2_client.playback.pause();
		}else{
			xmms2_client.playback.start();
		}
	}
	catch(std::exception &e){
		std::cout << __FILE__ << "[" << __LINE__ 
			<< "] Error in " << __PRETTY_FUNCTION__ << ": " 
			<< e.what() << std::endl;
	}
}

void Main_win::on_button_Forward()
{
	try{
		std::cout << __FILE__ << "[" << __LINE__ << "] got here on_button_Forward()" << std::endl;
		m_status = xmms2_client.playback.getStatus();
		if(m_status == Xmms::Playback::PLAYING ||
		   m_status == Xmms::Playback::PAUSED){
			//unsigned p = xmms2_client.playback.getPlaytime();
			//xmms2_client.playback.stop();
			//xmms2_client.playback.seekMs(p + 10000);
			xmms2_client.playback.seekSamplesRel(10000);
			//xmms2_client.playback.start();
		}else{
			//unsigned p = xmms2_client.playback.getPlaytime();
			//xmms2_client.playback.seekMs(p + 10000);
			xmms2_client.playback.seekSamplesRel(10000);
		}
	}
	catch(std::exception &e){
		std::cout << __FILE__ << "[" << __LINE__ 
			<< "] Error in " << __PRETTY_FUNCTION__ << ": " 
			<< e.what() << std::endl;
	}
}

void Main_win::on_button_Next()
{
	try{
		m_status = xmms2_client.playback.getStatus();
		if(m_status == Xmms::Playback::PLAYING ||
		   m_status == Xmms::Playback::PAUSED){
			xmms2_client.playback.stop();
			xmms2_client.playlist.setNextRel(1);
			xmms2_client.playback.start();
		}else{
			xmms2_client.playlist.setNextRel(1);
		}
		//refresh_playlist(); // taken care of in callbacks from xmms2_sync_client //
	}
	catch(std::exception &e){
		std::cout << __FILE__ << "[" << __LINE__ 
			<< "] Error in " << __PRETTY_FUNCTION__ << ": " 
			<< e.what() << std::endl;
	}
}

void Main_win::on_button_Delete()
{
	Gtk::MessageDialog dialogGtk::MessageDialog(*this, "Delete PlayList <b>" + m_currentPlaylistName 
	                                              + "?</b>\nThis cannot be undone!",
	                          true /* use_markup */, Gtk::MESSAGE_QUESTION,
	                          Gtk::BUTTONS_OK_CANCEL);
	dialog.set_secondary_text("you are about to delete a play "
	                          "list from xmms2's database!!");

	int result = dialog.run();

	//Handle the response:
	switch(result)
	{
		case(Gtk::RESPONSE_OK):
		{
			try{
				xmms2_client.playlist.remove(m_currentPlaylistName);
				//refresh_playlists(); // taken care of in callbacks from xmms2_sync_client //
			}
			catch(std::exception &e){
				Gtk::MessageDialog dialog(*this, "Failed to delete PlayList <b>" + m_currentPlaylistName 
				                          + "?</b>\nyour error message was <b><i>" + e.what() + "!</i></b>",
				                          true /* use_markup */, Gtk::MESSAGE_ERROR,
				                          Gtk::BUTTONS_OK);
				std::string currentplst = xmms2_client.playlist.currentActive();
				if(currentplst == m_currentPlaylistName){
					dialog.set_secondary_text("you cannot delete the currently "
					                          "active playlist!!");
				}else{
					dialog.set_secondary_text("I don't know what happend!!");
				}

				int result = dialog.run();
				std::cout << e.what() << std::endl;
			}
			std::cout << "OK clicked." << std::endl;
			break;
		}
		case(Gtk::RESPONSE_CANCEL):
		{
			std::cout << "Cancel clicked." << std::endl;
			break;
		}
		default:
		{
			std::cout << "Unexpected button clicked." << std::endl;
			break;
		}
	}
}

bool Main_win::handle_status(const Xmms::Playback::Status &st)
{
	std::cout << __FILE__ << '[' << __LINE__ << "] handle_status " << std::endl;
	Glib::Threads::Mutex::Lock lock (m_mutex);
	try{
		basemesage *msg = new mesage<Xmms::Playback::Status>(basemesage::status, st);
		m_queue.push(msg);
	}
	catch(std::exception &e){
		std::cout << __FILE__ << "[" << __LINE__ 
			<< "] Error in " << __PRETTY_FUNCTION__ << ": " 
			<< e.what() << std::endl;
	}

	return true;
}



void Main_win::refresh_playlists(){
	try{
		//std::cout << __FILE__ << " got here[" << __LINE__ << ']' << std::endl;
		std::string currentplst = xmms2_client.playlist.currentActive();
		//Xmms::List< std::string > lst = xmms2_client.collection.list("Playlists");
		Xmms::List< std::string > lst = xmms2_client.collection.list(Xmms::Collection::PLAYLISTS);


		//Glib::RefPtr<Gtk::TextBuffer> buffer = m_listviewformatTextPlaylists->get_buffer();

		//buffer->set_text("");
		//std::cout << "got here[" << __LINE__ << ']' << std::endl;
		m_listviewformatTextPlaylists->clear_items();
		//std::cout << "got here[" << __LINE__ << ']' << std::endl;

		for( Xmms::List< std::string  >::const_iterator i(lst.begin()), i_end(lst.end()); i != i_end; ++i ){
			//for(auto playlst: lst)
			if(*i == currentplst){
				//Gtk::TextBuffer::Tag tag = Pango::Weight::Bold;
				//std::vector< Glib::RefPtr<Gtk::TextTag> > tags;

				//tags.push_back(m_ref_bold_tag);

				//buffer->insert_with_tags(buffer->end(), *i + "\n", tags);
				m_listviewformatTextPlaylists->append("<b>" + *i + "</b>");
				//std::cout << __FILE__ << "[" << __LINE__ << "] got here" << std::endl;
				//buffer->insert(buffer->end(), "* " + *i + "\n");
				//std::cout << "* ";
			}else{
				//buffer->insert(buffer->end(), *i + "\n");
				m_listviewformatTextPlaylists->append(*i);
				//std::cout << __FILE__ << "[" << __LINE__ << "] got here" << std::endl;
				//std::cout << "  ";
			}
			//std::cout << *i << std::endl;
		}
		std::cout << "=============================================================================" << std::endl;
		//std::cout << "damm how do I add libs in this anjuta"  << std::endl;
/*		for(auto playlist: lst){
			if(playlist == currentplst){
				std::cout << "* ";
	        }else{
		        std::cout << "  ";
	        }
	        std::cout << playlist << std::endl;
	    }*/
		m_currentPlaylistName = currentplst;
	}
	catch(std::exception &e){
		std::cout << __FILE__ << "[" << __LINE__ 
			<< "] Error in " << __PRETTY_FUNCTION__ << ": " 
			<< e.what() << std::endl;
	}
}

void Main_win::on_button_Exit(){
	hide();
}

void Main_win::on_button_Help(){
	int res = m_aboutdialog1->run();
	m_aboutdialog1->hide();
}

void Main_win::on_button_VolumeLeftChange(double value)
{
	std::cout << __FILE__ << "[" << __LINE__ << "] got here on_button_VolumeLeftChange: value " << value << std::endl;

	//Glib::Threads::Mutex::Lock lock (m_mutex);
	try{
		if(!m_setting_volume_left){
			xmms2_client.playback.volumeSet("left", round(value*100));
		}
	}
	catch(std::exception &e){
		std::cout << __FILE__ << "[" << __LINE__ 
			<< "] Error in " << __PRETTY_FUNCTION__ << ": " 
			<< e.what() << std::endl;
	}
}

void Main_win::on_button_VolumeRightChange(double value)
{
	std::cout << __FILE__ << "[" << __LINE__ << "] got here on_button_VolumeRightChange: value " << value << std::endl;

	//Glib::Threads::Mutex::Lock lock (m_mutex);
	try{
		if(!m_setting_volume_right){
			xmms2_client.playback.volumeSet("right", round(value*100));
		}
	}
	catch(std::exception &e){
		std::cout << __FILE__ << "[" << __LINE__ 
			<< "] Error in " << __PRETTY_FUNCTION__ << ": " 
			<< e.what() << std::endl;
	}
}

void Main_win::on_button_VolumeMasterChange(double value)
{
	std::cout << __FILE__ << "[" << __LINE__ << "] got here on_button_VolumeMasterChange: value " << value << std::endl;

	//Glib::Threads::Mutex::Lock lock (m_mutex);
	try{
		if(!m_setting_volume_master){
			xmms2_client.playback.volumeSet("master", round(value*100));
		}
	}
	catch(std::exception &e){
		std::cout << __FILE__ << "[" << __LINE__ 
			<< "] Error in " << __PRETTY_FUNCTION__ << ": " 
			<< e.what() << std::endl;
	}
}

/*bool Main_win::on_button_Pressed(GdkEventButton* event){
	std::cout << "entering on_button_Pressed" << std::endl;
	//Glib::RefPtr<Gtk::TextBuffer> buffer = m_listviewformatTextPlaylists->get_buffer();
	if(event->type == GDK_2BUTTON_PRESS){ //double click //
		std::cout << "double click: " << m_currentPlaylistName << std::endl;
		return false;
	}else if(event->type == GDK_BUTTON_PRESS){ // single click //
		std::cout << __FILE__ << '[' << __LINE__ << "] m_connect_retrys == " 
		                           << m_connect_retrys << std::endl;
		for(int i = 0; i < m_connect_retrys;i++){
			std::cout << "got here[" << __LINE__ << ']' << std::endl;
			try{
				xmms2_client.connect( std::getenv("XMMS_PATH") );
				std::cout << "got here[" << __LINE__ << ']' << std::endl;
				//refresh_playlists();
				refresh_playlist();
				std::cout << "connect count == " << ++connect_cout << std::endl;
				break;
			}
			catch(std::exception &e){
				std::cout << e.what() << std::endl;
				system("/usr/bin/xmms2-launcher"); // force xmms2d to start //
			}
		}
		std::cout << "single click: " << m_currentPlaylistName << std::endl;
		
		return false;
	}else if(event->type == GDK_3BUTTON_PRESS){ // single click //
		std::cout << "triple click: " << m_currentPlaylistName << std::endl;
		return false;
	}else if(event->type == GDK_BUTTON_RELEASE){
		std::cout << "button release: " << m_currentPlaylistName << std::endl;
		return true;
	}
	return false;
}*/

std::vector<Glib::ustring> Main_win::get_mediainfo(int id, int highlight)
{
    try{
		std::vector<Glib::ustring> result;

		Xmms::Dict info = xmms2_client.medialib.getInfo( id );

		std::string pre, post;
		if(highlight == 1){ // bold //
			pre  = "<b>";
			post = "</b>";
		}else if(highlight == 2){  // italic //
			pre  = "<i>";
			post = "</i>";
		}else{ // none //
		}

		//std::cout << "title = ";
		try {
			result.insert(result.end(), pre + boost::get<std::string>(info["title"]) + post);
			//std::cout << info["title"] << std::endl;
		}
		catch( Xmms::no_such_key_error& err ) {
			result.insert(result.end(), pre + "Title unknown" + post);
			//std::cout << "Title unknown" << std::endl;
		}

		//std::cout << "artist = ";
		try {
			result.insert(result.end(), pre + boost::get<std::string>(info["artist"]) + post);
			//std::cout << info["artist"] << std::endl;
		}
		catch( Xmms::no_such_key_error& err ) {
			result.insert(result.end(), pre + "No artist" + post);
			//std::cout << "No artist" << std::endl;
		}

		//std::cout << "album = ";
		try {
			result.insert(result.end(), pre + boost::get<std::string>(info["album"]) + post);
			//std::cout << info["album"] << std::endl;
		}
		catch( Xmms::no_such_key_error& err ) {
			result.insert(result.end(), pre + "No album" + post);
			//std::cout << "No album" << std::endl;
		}

		//std::cout << "bitrate = ";

/*		try {
			std::cout << info["bitrate"] << std::endl;
	     }
	     catch( Xmms::no_such_key_error& err ) {
		     std::cout << "0" << std::endl;
	     }*/

		//std::cout << "duration = ";

		try {
			int duration = boost::get< int >( info["duration"] );
			//std::cout << duration << "  (" << std::flush;
			double secs = (duration % 60000)/1000.0;
			duration /= 60000;
			int mins = duration % 60;
			duration /= 60;
			int hours = duration;
			boost::format bf("%02d:%02d:%06.3f");
			bf % hours % mins % secs;
			//std::cout << bf << ')' << std::endl;
			std::string s = bf.str();
			result.insert(result.end(), pre + s + post);
		}
		catch( Xmms::no_such_key_error& err ) {
			result.insert(result.end(), pre + "unknown duration" + post);
			//std::cout << "unknown duration" << std::endl;
		}

		if(highlight){
			result.insert(result.end(), pre + "*>" + post);
		}else{
			result.insert(result.end(), pre + "" + post);
		}

		//std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
		return result;
	}
	catch(std::exception &e){
		std::cout << __FILE__ << "[" << __LINE__ 
			<< "] Error in " << __PRETTY_FUNCTION__ << ": " 
			<< e.what() << std::endl;
	}
}

void Main_win::refresh_playlist()
{
	try{
		//std::cout << __FILE__ << '[' << __LINE__ << "] m_currentPlaylistName == " << m_currentPlaylistName << std::endl;
		Xmms::List<int> list = xmms2_client.playlist.listEntries(m_currentPlaylistName);
		//std::cout << __FILE__ << '[' << __LINE__ << "] list.size() == " << list.size() << std::endl;

		m_MatrixBoxCurrentPlaylist->clear_items();

		//if(list.size() == 0) return;
		if(list.begin() == list.end()) return;

		Xmms::DictResult currentPos = xmms2_client.playlist.currentPos(m_currentPlaylistName);
		Xmms::Dict d = currentPos;
		std::string name = boost::get<std::string>(d["name"]);
		//std::cout << __FILE__ << '[' << __LINE__ << "] name == " << name << std::endl;
		int position = boost::get<int>(d["position"]);
		//std::cout << __FILE__ << '[' << __LINE__ << "] position == " << position << std::endl;
		std::string currentplst = xmms2_client.playlist.currentActive();
		/*std::string fred;
		 std::cout << "debug >> " <<std::flush;
		 std::cin >> fred;*/

		int p = 0, highlight;
		for( Xmms::List< int >::const_iterator i(list.begin()), i_end(list.end()); i != i_end; ++i, ++p){
			if(p == position){
				if(currentplst == name){
					highlight = 1; // bold  //
				}else{
					highlight = 2; // italic //
				}
			}else{
				highlight = 0;
			}
			std::vector<Glib::ustring> result = get_mediainfo(*i, highlight);
			//std::cout << __FILE__ << '[' << __LINE__ << "] result.size() == " << result.size() << std::endl;
			guint res = m_MatrixBoxCurrentPlaylist->append(result, *i, p);
			//std::cout << __FILE__ << '[' << __LINE__ << "] res == " << res << std::endl;
		}
	}
	catch(std::exception &e){
		std::cout << __FILE__ << "[" << __LINE__ 
			<< "] Error in " << __PRETTY_FUNCTION__ << ": " 
			<< e.what() << std::endl;
	}
}


void Main_win::on_Playlists_clicked(Glib::ustring cp)
{
	//Glib::Threads::Mutex::Lock lock (m_mutex);
    m_currentPlaylistName = cp;
	std::cout << __FILE__ << '[' << __LINE__ << "] m_currentPlaylistName == " << m_currentPlaylistName << std::endl;
	try{
		xmms2_sync_client.playlist.listEntries(m_currentPlaylistName)(Xmms::bind(&Main_win::handle_list, this));
		//refresh_playlist(); // taken care of in callbacks from xmms2_sync_client //
	}
	catch(Xmms::no_such_key_error& err){
		std::cout << __FILE__ << '[' << __LINE__ << "] refresh_playlist() failed m_currentPlaylistName == " << m_currentPlaylistName << std::endl;
		std::cout << __FILE__ << '[' << __LINE__ << "] err.what() == " << err.what() << std::endl;
	}
	catch(std::exception &e){
		std::cout << __FILE__ << '[' << __LINE__ << "] refresh_playlist() failed m_currentPlaylistName == " << m_currentPlaylistName << std::endl;
		std::cout << __FILE__ << '[' << __LINE__ << "] e.what() " << e.what() << std::endl;
	}
}

void Main_win::on_Playlists_dblclicked(Glib::ustring cp)
{
	try{
		//Glib::Threads::Mutex::Lock lock (m_mutex);
		m_currentPlaylistName = cp;
		std::cout << __FILE__ << '[' << __LINE__ << "] got here m_currentPlaylistName == " << m_currentPlaylistName << std::endl;
		//refresh_playlist();
		xmms2_client.playlist.load(m_currentPlaylistName);
		xmms2_client.playback.stop();
		xmms2_client.playback.start();
		//refresh_playlists();
		//refresh_playlist();
	}
	catch(std::exception &e){
		std::cout << __FILE__ << "[" << __LINE__ 
			<< "] Error in " << __PRETTY_FUNCTION__ << ": " 
			<< e.what() << std::endl;
	}
}

void Main_win::on_Playlist_clicked(int id)
{
	std::cout << __FILE__ << '[' << __LINE__ << "] id == " << id << std::endl;
	//Glib::Threads::Mutex::Lock lock (m_mutex);
}

void Main_win::on_Playlist_dblclicked(int pos)
{
	//Glib::Threads::Mutex::Lock lock (m_mutex);
	try{
		std::cout << __FILE__ << '[' << __LINE__ << "] got here pos == " << pos << std::endl;
		xmms2_client.playlist.load(m_currentPlaylistName);
		xmms2_client.playback.stop();
		xmms2_client.playlist.setNext(pos);
		xmms2_client.playback.start();
		//refresh_playlists();
		//refresh_playlist();
	}
	catch(std::exception &e){
		std::cout << __FILE__ << "[" << __LINE__ 
			<< "] Error in " << __PRETTY_FUNCTION__ << ": " 
			<< e.what() << std::endl;
	}
}

bool Main_win::handle_change(const Xmms::Dict &chg)
{
    std::cout << __FILE__ << "[" << __LINE__ << "] got here handle_change: " << std::endl;
	Glib::Threads::Mutex::Lock lock(m_mutex);
	try{
		basemesage *msg = new mesage<Xmms::Dict>(basemesage::plchange, chg);
		m_queue.push(msg);
	}
	catch(std::exception &e){
		std::cout << __FILE__ << "[" << __LINE__ 
			<< "] Error in handle_change: " 
			<< e.what() << std::endl;
	}

	return true;
}


bool Main_win::handle_update_pos(const Xmms::Dict &posdict)
{
	std::cout << __FILE__ << '[' << __LINE__ << "] handle_update_pos " << std::endl;
	Glib::Threads::Mutex::Lock lock(m_mutex);
	try{
		basemesage *msg = new mesage<Xmms::Dict>(basemesage::update_pos, posdict);
		m_queue.push(msg);
	}
	catch(std::exception &e){
		std::cout << __FILE__ << "[" << __LINE__ 
			<< "] Error in handle_update_pos: " 
			<< e.what() << std::endl;
	}

	return true;
}

bool Main_win::handle_pls_loaded(const std::string &name)
{
	std::cout << __FILE__ << '[' << __LINE__ 
	          << "] handle_pls_loaded name == " << name << std::endl;
	Glib::Threads::Mutex::Lock lock(m_mutex);
	try{
		basemesage *msg = new mesage<std::string>(basemesage::pls_loaded, name);
		m_queue.push(msg);
	}
	catch(std::exception &e){
		std::cout << __FILE__ << "[" << __LINE__ 
			<< "] Error in handle_pls_loaded: " 
			<< e.what() << std::endl;
	}

	return true;
}

bool Main_win::handle_list(const Xmms::List< int > &list)
{
	std::cout << __FILE__ << '[' << __LINE__ << "] handle_list: entering"  << std::endl;
	Glib::Threads::Mutex::Lock lock(m_mutex);

	if(list.begin() == list.end()){
		std::cout << __FILE__ << '[' << __LINE__ << "] handle_list: exiting early list empty"  << std::endl;
		return true;
	}
	
	try{
		basemesage *msg = new mesage<Xmms::List< int > >(basemesage::list, list);
		m_queue.push(msg);
	}
	catch(std::exception &e){
		std::cout << __FILE__ << "[" << __LINE__ 
			<< "] Error in handle_list: " 
			<< e.what() << std::endl;
	}
	
	std::cout << __FILE__ << '[' << __LINE__ << "] handle_list: exiting"  << std::endl;
	return true;
}


bool Main_win::handle_current_id(const unsigned int &id)
{
	std::cout << __FILE__ << '[' << __LINE__ << "] handle_current_id id == " << id << std::endl;

	Glib::Threads::Mutex::Lock lock (m_mutex);
	try{
		basemesage *msg = new mesage<int>(basemesage::current_id, id);
		m_queue.push(msg);
	}
	catch(std::exception &e){
		std::cout << __FILE__ << "[" << __LINE__ 
			<< "] Error in handle_current_id: " 
			<< e.what() << std::endl;
	}
	
	return true;
}



bool Main_win::handle_volume(const Xmms::Dict &d)
{
	/* XXX: this function really should do each and sum / split the channels */
	std::cout << __FILE__ << '[' << __LINE__ << "] handle_volume " << std::endl;

	Glib::Threads::Mutex::Lock lock (m_mutex);
	try{
		basemesage *msg = new mesage<Xmms::Dict>(basemesage::handle_volume, d);
		m_queue.push(msg);
	}
	catch(std::exception &e){
		std::cout << __FILE__ << "[" << __LINE__ 
			<< "] Error in handle_volume: " 
			<< e.what() << std::endl;
	}

	return true;
}

bool Main_win::handle_playtime(const unsigned int &tme)
{
	//std::cout << __FILE__ << '[' << __LINE__ << "] handle_playtime: tme " << tme << std::endl;

	Glib::Threads::Mutex::Lock lock (m_mutex);
	try{
		basemesage *msg = new mesage<unsigned>(basemesage::handle_playtime, tme);
		m_queue.push(msg);
	}
	catch(std::exception &e){
		std::cout << __FILE__ << "[" << __LINE__ 
			<< "] Error in handle_playtime: " 
			<< e.what() << std::endl;
	}

	return true;
}

bool Main_win::handle_mlib_entry_changed(const uint32_t &id)
{
	std::cout << __FILE__ << '[' << __LINE__ << "] handle_mlib_entry_changed: id == " << id << std::endl;

	Glib::Threads::Mutex::Lock lock(m_mutex);
	try{
		if(xmms2_client.playback.getStatus() == Xmms::Playback::PLAYING){
			xmms2_sync_client.medialib.getInfo(id)(Xmms::bind(&Main_win::handle_medialib_info, this));
		}

		basemesage *msg = new mesage<uint32_t>(basemesage::handle_mlib_entry_changed, id);
		m_queue.push(msg);
	}
	catch(std::exception &e){
		std::cout << __FILE__ << "[" << __LINE__ 
			<< "] Error in handle_mlib_entry_changed: " 
			<< e.what() << std::endl;
	}

	return true;
}

bool Main_win::handle_medialib_info(const Xmms::PropDict &info)
{
	int32_t id = info.get<int32_t>("id");
	std::cout << __FILE__ << '[' << __LINE__ << "] handle_medialib_info: id == " << id << std::endl;

	Glib::Threads::Mutex::Lock lock (m_mutex);
	try{
		basemesage *msg = new mesage<Xmms::PropDict>(basemesage::handle_medialib_info, info);
		m_queue.push(msg);
	}
	catch(std::exception &e){
		std::cout << __FILE__ << "[" << __LINE__ 
			<< "] Error in handle_medialib_info: " 
			<< e.what() << std::endl;
	}

	return true;
}

void Main_win::on_button_New()
{
	Xmms::List< std::string > lst = xmms2_client.collection.list(Xmms::Collection::COLLECTIONS);
	std::vector<Glib::ustring> colls(lst.begin(), lst.end());
	m_dialogNewPlaylist->set_collections(colls);
	int res = m_dialogNewPlaylist->run();
	m_dialogNewPlaylist->hide();
	if(res){
		std::cout << "Create palylist" << std::endl;
	}else{
		std::cout << "New Playlist Canceled" << std::endl;
	}
}

void Main_win::on_action_NewCollection()
{
}

std::vector<Xmms::Dict> Main_win::on_coll_changed(Glib::ustring collection_name, std::vector<Glib::ustring> orderby)
{
	std::cout << __FILE__ << '[' << __LINE__ << "] on_coll_changed: collection_name == " << collection_name << std::endl;
	Xmms::CollResult collresult = xmms2_client.collection.get(static_cast<std::string>(collection_name), Xmms::Collection::COLLECTIONS);
	Xmms::CollPtr coll = static_cast<Xmms::CollPtr>(collresult);
	std::cout << __FILE__ << '[' << __LINE__ << "] coll got: collection_name == " << collection_name << std::endl;
	std::list<std::string> fetch;
	fetch.insert(fetch.end(), "tracknr");
	fetch.insert(fetch.end(), "title");
	fetch.insert(fetch.end(), "artist");
	fetch.insert(fetch.end(), "album");
	fetch.insert(fetch.end(), "duration");
	std::list<std::string> order(orderby.begin(), orderby.end());
	if(order.empty()){
		order.insert(order.end(), "album");
		order.insert(order.end(), "tracknr");
		order.insert(order.end(), "artist");
		//order.insert(order.end(), "title");
		//order.insert(order.end(), "duration");
	}
	std::cout << __FILE__ << '[' << __LINE__ << "] getting list: collection_name == " << collection_name << std::endl;
	//Xmms::List<int> lst = xmms2_client.collection.queryIds(*coll);
/*	Xmms::List<int> lst = xmms2_client.collection.queryIds(*coll, order);
	std::vector<Xmms::Dict> result;
	std::cout << __FILE__ << '[' << __LINE__ << "] got list: collection_name == " << collection_name << std::endl;
	for( Xmms::List<int>::const_iterator i(lst.begin()), i_end(lst.end()); i != i_end; ++i){
		std::cout << __FILE__ << '[' << __LINE__ << "] *i == " << *i << std::endl;
		Xmms::Dict info = xmms2_client.medialib.getInfo(*i);
		result.insert(result.end(), info);
		std::cout << __FILE__ << '[' << __LINE__ << "] result.size() == " << result.size() << std::endl;
	}*/
	std::cout << __FILE__ << '[' << __LINE__ << "] list got: collection_name == " << collection_name << std::endl;
	Xmms::List<Xmms::Dict> lst = xmms2_client.collection.queryInfos(*coll, fetch, order);
	std::cout << __FILE__ << '[' << __LINE__ << "] list got: collection_name == " << collection_name << std::endl;
	std::vector<Xmms::Dict> result(lst.begin(), lst.end());
	std::cout << __FILE__ << '[' << __LINE__ << "] result made: result.size() == " << result.size() << std::endl;
	return result;
}





