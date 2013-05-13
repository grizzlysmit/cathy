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
    m_buttonNext(0), m_buttonDelete(0), m_buttonExit(0), m_buttonHelp(0), 
    m_aboutdialog1(0), 
    m_listviewformatTextPlaylists(0), m_MatrixBoxCurrentPlaylist(0), m_imagemenuitemQuit(0), 
    m_imagemenuitemConnect(0), 
    m_scrolledwindowPlaylists(0), m_scrolledwindowCurrentPlaylist(0), m_panedBody(0), 
    m_statusbar1(0), 
    //m_eventboxPlaylists(0), m_eventboxCurrentPlaylist(0), 
    xmms2_client("cathy"), connect_cout(0), m_connect_retrys(2)
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
	m_builder->get_widget("buttonExit", m_buttonExit);
	if(m_buttonExit){
		m_buttonExit->signal_clicked().connect( sigc::mem_fun(*this, &Main_win::on_button_Exit) );
	}
	m_builder->get_widget("buttonHelp", m_buttonHelp);
	if(m_buttonHelp){
		m_buttonHelp->signal_clicked().connect( sigc::mem_fun(*this, &Main_win::on_button_Help) );
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
	m_builder->get_widget("aboutdialog1", m_aboutdialog1);
	if(m_aboutdialog1){
		//m_aboutdialog1->signal_activate().connect( sigc::mem_fun(*this, &Main_win::on_button_Exit) );
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
	std::cout << "got here[" << __LINE__ << ']' << std::endl;
    for(int i = 0; i < m_connect_retrys;i++){
		std::cout << "got here[" << __LINE__ << ']' << std::endl;
		try{
			xmms2_client.connect( std::getenv("XMMS_PATH") );
			std::cout << "got here[" << __LINE__ << ']' << std::endl;
			refresh_playlists();
			refresh_playlist();
			xmms2_client.playback.getStatus()(Xmms::bind(&Main_win::handle_status, this));
			xmms2_client.playback.broadcastStatus()(Xmms::bind(&Main_win::handle_status, this));

			xmms2_client.setDisconnectCallback(boost::bind(&Main_win::handle_disconnect, this));
			m_status = xmms2_client.playback.getStatus();
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

void Main_win::handle_disconnect()
{
	m_statusbar1->push("Cathy is disconnected from xmms2d", m_ContextId);
	on_button_Connect();
}


void Main_win::on_button_Previous()
{
	if(m_status == Xmms::Playback::PLAYING ||
	    m_status == Xmms::Playback::PAUSED){
		xmms2_client.playback.stop();
		xmms2_client.playlist.setNextRel(-2);
		xmms2_client.playback.start();
	}else{
		xmms2_client.playlist.setNextRel(-2);
	}
	refresh_playlist();
}

void Main_win::on_button_Rewind()
{
	std::cout << __FILE__ << "[" << __LINE__ << "] got here on_button_Rewind()" << std::endl;
	if(m_status == Xmms::Playback::PLAYING ||
	    m_status == Xmms::Playback::PAUSED){
		unsigned p = xmms2_client.playback.getPlaytime();
		xmms2_client.playback.stop();
		xmms2_client.playback.seekMs(p - 10000);
		//xmms2_client.playback.seekSamplesRel(-10);
		xmms2_client.playback.start();
	}else{
		//unsigned p = xmms2_client.playback.getPlaytime();
		//xmms2_client.playback.seekMs(p - 10000);
	}
}

void Main_win::on_button_Stop()
{
	xmms2_client.playback.stop();
}

void Main_win::on_button_PlayPause()
{
	if(m_status == Xmms::Playback::PLAYING){
		xmms2_client.playback.pause();
	}else{
		xmms2_client.playback.start();
	}
}

void Main_win::on_button_Forward()
{
	std::cout << __FILE__ << "[" << __LINE__ << "] got here on_button_Forward()" << std::endl;
	if(m_status == Xmms::Playback::PLAYING ||
	    m_status == Xmms::Playback::PAUSED){
		unsigned p = xmms2_client.playback.getPlaytime();
		xmms2_client.playback.stop();
		xmms2_client.playback.seekMs(p + 10000);
		//xmms2_client.playback.seekSamplesRel(10);
		xmms2_client.playback.start();
	}else{
		unsigned p = xmms2_client.playback.getPlaytime();
		xmms2_client.playback.seekMs(p + 10000);
	}
}

void Main_win::on_button_Next()
{
	if(m_status == Xmms::Playback::PLAYING ||
	    m_status == Xmms::Playback::PAUSED){
		xmms2_client.playback.stop();
		xmms2_client.playlist.setNextRel(1);
		xmms2_client.playback.start();
	}else{
		xmms2_client.playlist.setNextRel(1);
	}
	refresh_playlist();
}

void Main_win::on_button_Delete()
{
	Gtk::MessageDialog dialog(*this, "Delete PlayList <b>" + m_currentPlaylistName 
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
				refresh_playlists();
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
	m_status = st;

	if (st == Xmms::Playback::STOPPED ||
	    st == Xmms::Playback::PAUSED){
		m_buttonPlayPause->set_label("gtk-media-play");
	}else{
		m_buttonPlayPause->set_label("gtk-media-pause");
	}

	return true;
}



void Main_win::refresh_playlists(){
	std::cout << "got here[" << __LINE__ << ']' << std::endl;
	std::string currentplst = xmms2_client.playlist.currentActive();
	Xmms::List< std::string > lst = xmms2_client.collection.list("Playlists");

	//Glib::RefPtr<Gtk::TextBuffer> buffer = m_listviewformatTextPlaylists->get_buffer();

	//buffer->set_text("");
	std::cout << "got here[" << __LINE__ << ']' << std::endl;
	m_listviewformatTextPlaylists->clear_items();
	std::cout << "got here[" << __LINE__ << ']' << std::endl;

	for( Xmms::List< std::string  >::const_iterator i(lst.begin()), i_end(lst.end()); i != i_end; ++i ){
    //for(auto playlst: lst)
        if(*i == currentplst){
			//Gtk::TextBuffer::Tag tag = Pango::Weight::Bold;
			//std::vector< Glib::RefPtr<Gtk::TextTag> > tags;

			//tags.push_back(m_ref_bold_tag);
			
			//buffer->insert_with_tags(buffer->end(), *i + "\n", tags);
			m_listviewformatTextPlaylists->append("<b>" + *i + "</b>");
			std::cout << __FILE__ << "[" << __LINE__ << "] got here" << std::endl;
		    //buffer->insert(buffer->end(), "* " + *i + "\n");
            std::cout << "* ";
        }else{
		    //buffer->insert(buffer->end(), *i + "\n");
			m_listviewformatTextPlaylists->append(*i);
			std::cout << __FILE__ << "[" << __LINE__ << "] got here" << std::endl;
            std::cout << "  ";
        }
        std::cout << *i << std::endl;
    }
	std::cout << "=============================================================================" << std::endl;
	//std::cout << "damm how do I add libs in this anjuta"  << std::endl;
/* 	for(auto playlist: lst){
		if(playlist == currentplst){
			std::cout << "* ";
		}else{
			std::cout << "  ";
		}
		std::cout << playlist << std::endl;
	}*/
	m_currentPlaylistName = currentplst;
}

void Main_win::on_button_Exit(){
	hide();
}

void Main_win::on_button_Help(){
	int res = m_aboutdialog1->run();
	m_aboutdialog1->hide();
}

bool Main_win::on_button_Pressed(GdkEventButton* event){
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
}

std::vector<Glib::ustring> Main_win::get_mediainfo(int id, int highlight)
{

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

	std::cout << "title = ";
	try {
        result.insert(result.end(), pre + boost::get<std::string>(info["title"]) + post);
		std::cout << info["title"] << std::endl;
	}
	catch( Xmms::no_such_key_error& err ) {
		result.insert(result.end(), pre + "Title unknown");
		std::cout << "Title unknown" << std::endl;
	}
	
	std::cout << "artist = ";
	try {
        result.insert(result.end(), pre + boost::get<std::string>(info["artist"]) + post);
		std::cout << info["artist"] << std::endl;
	}
	catch( Xmms::no_such_key_error& err ) {
		result.insert(result.end(), pre + "No artist" + post);
		std::cout << "No artist" << std::endl;
	}

	std::cout << "album = ";
	try {
        result.insert(result.end(), pre + boost::get<std::string>(info["album"]) + post);
		std::cout << info["album"] << std::endl;
	}
	catch( Xmms::no_such_key_error& err ) {
		result.insert(result.end(), pre + "No album" + post);
		std::cout << "No album" << std::endl;
	}

	std::cout << "bitrate = ";

	try {
		std::cout << info["bitrate"] << std::endl;
	}
	catch( Xmms::no_such_key_error& err ) {
		std::cout << "0" << std::endl;
	}

	std::cout << "duration = ";

	try {
		int duration = boost::get< int >( info["duration"] );
		std::cout << duration << "  (" << std::flush;
        double secs = (duration % 60000)/1000.0;
        duration /= 60000;
        int mins = duration % 60;
        duration /= 60;
        int hours = duration;
        boost::format bf("%02d:%02d:%06.3f");
        bf % hours % mins % secs;
		std::cout << bf << ')' << std::endl;
		std::string s = bf.str();
        result.insert(result.end(), pre + s + post);
	}
	catch( Xmms::no_such_key_error& err ) {
		result.insert(result.end(), pre + "unknown duration" + post);
		std::cout << "unknown duration" << std::endl;
	}

	if(highlight){
		result.insert(result.end(), pre + "*>" + post);
	}else{
		result.insert(result.end(), pre + "" + post);
	}

    std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
	return result;
}

void Main_win::refresh_playlist()
{
    std::cout << __FILE__ << '[' << __LINE__ << "] m_currentPlaylistName == " << m_currentPlaylistName << std::endl;
	Xmms::List< int > list = xmms2_client.playlist.listEntries(m_currentPlaylistName);
    std::cout << __FILE__ << '[' << __LINE__ << "] list.size() == " << list.size() << std::endl;

	m_MatrixBoxCurrentPlaylist->clear_items();

    if(list.size() == 0) return;
                               
    Xmms::DictResult currentPos = xmms2_client.playlist.currentPos(m_currentPlaylistName);
    Xmms::Dict d = currentPos;
    std::string name = boost::get<std::string>(d["name"]);
    std::cout << __FILE__ << '[' << __LINE__ << "] name == " << name << std::endl;
    int position = boost::get<int>(d["position"]);
    std::cout << __FILE__ << '[' << __LINE__ << "] position == " << position << std::endl;
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
		std::cout << __FILE__ << '[' << __LINE__ << "] result.size() == " << result.size() << std::endl;
		guint res = m_MatrixBoxCurrentPlaylist->append(result, *i);
		std::cout << __FILE__ << '[' << __LINE__ << "] res == " << res << std::endl;
	}
}


void Main_win::on_Playlists_clicked(Glib::ustring cp)
{
    m_currentPlaylistName = cp;
	std::cout << __FILE__ << '[' << __LINE__ << "] m_currentPlaylistName == " << m_currentPlaylistName << std::endl;
	try{
		refresh_playlist();
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
    m_currentPlaylistName = cp;
	std::cout << __FILE__ << '[' << __LINE__ << "] got here m_currentPlaylistName == " << m_currentPlaylistName << std::endl;
	//refresh_playlist();
	xmms2_client.playlist.load(m_currentPlaylistName);
	xmms2_client.playback.stop();
	refresh_playlists();
	refresh_playlist();
}

void Main_win::on_Playlist_clicked(int id)
{
	std::cout << __FILE__ << '[' << __LINE__ << "] id == " << id << std::endl;
}

void Main_win::on_Playlist_dblclicked(int id)
{
	std::cout << __FILE__ << '[' << __LINE__ << "] got here id == " << id << std::endl;
	xmms2_client.playlist.load(m_currentPlaylistName);
	xmms2_client.playback.stop();
	xmms2_client.playback.start();
	refresh_playlists();
	refresh_playlist();
}


