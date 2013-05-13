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
  : Gtk::Window(cobject), m_builder(builder), m_buttonConnect(0), m_listviewformatTextPlaylists(0), 
    m_listviewformatTextCurrentPlaylist(0), m_buttonExit(0), m_imagemenuitemQuit(0), 
    m_scrolledwindowPlaylists(0), m_scrolledwindowCurrentPlaylist(0), m_panedBody(0), 
    //m_eventboxPlaylists(0), m_eventboxCurrentPlaylist(0), 
    xmms2_client("cathy"), connect_cout(0), m_connect_retrys(2)
{
	//Gtk::Window::property_default_width() = 2000;
	m_builder->get_widget("buttonConnect", m_buttonConnect);
	if(m_buttonConnect){
		m_buttonConnect->signal_clicked().connect( sigc::mem_fun(*this, &Main_win::on_button_Connect) );
	}
	m_builder->get_widget("buttonExit", m_buttonExit);
	if(m_buttonExit){
		m_buttonExit->signal_clicked().connect( sigc::mem_fun(*this, &Main_win::on_button_Exit) );
	}
	m_builder->get_widget("imagemenuitemQuit", m_imagemenuitemQuit);
	if(m_imagemenuitemQuit){
		m_imagemenuitemQuit->signal_activate().connect( sigc::mem_fun(*this, &Main_win::on_button_Exit) );
	}
	m_builder->get_widget("panedBody", m_panedBody);
	if(m_panedBody){
		m_scrolledwindowPlaylists = new Gtk::ScrolledWindow();
		m_scrolledwindowCurrentPlaylist = new Gtk::ScrolledWindow();
		m_panedBody->pack1(*m_scrolledwindowPlaylists, true, true);
		m_panedBody->pack2(*m_scrolledwindowCurrentPlaylist, true, true);
		m_listviewformatTextPlaylists = new ListViewFormatText();
		m_listviewformatTextCurrentPlaylist = new MatrixBox(4);
		//m_eventboxPlaylists = new Gtk::EventBox();
		//m_eventboxCurrentPlaylist = new Gtk::EventBox();
		m_scrolledwindowCurrentPlaylist->add(*m_listviewformatTextCurrentPlaylist);
		//m_eventboxCurrentPlaylist->add(*m_listviewformatTextCurrentPlaylist);
		m_listviewformatTextPlaylists->set_tooltip_text("Playlists\nclick to select\ndouble click to make default");
		std::cout << "got here[" << __LINE__ << ']' << std::endl;
		//m_eventboxPlaylists->add(*m_listviewformatTextPlaylists);
		m_scrolledwindowPlaylists->add(*m_listviewformatTextPlaylists);

		m_listviewformatTextPlaylists->signal_clicked().connect(sigc::mem_fun(*this, &Main_win::on_Playlists_clicked));
		m_listviewformatTextPlaylists->signal_dblclicked().connect(sigc::mem_fun(*this, &Main_win::on_Playlists_dblclicked));

		m_listviewformatTextCurrentPlaylist->signal_clicked().connect(sigc::mem_fun(*this, &Main_win::on_Playlist_clicked));
		m_listviewformatTextCurrentPlaylist->signal_dblclicked().connect(sigc::mem_fun(*this, &Main_win::on_Playlist_dblclicked));
		

		std::vector<Glib::ustring> headers;
		headers.insert(headers.end(), "<b>Title</b>");
		headers.insert(headers.end(), "<b>Artist</b>");
		headers.insert(headers.end(), "<b>Album</b>");
		headers.insert(headers.end(), "<b>Duration</b>");
		m_listviewformatTextCurrentPlaylist->set_headings(headers);
		// show the widgits //
		//m_listviewformatTextPlaylists->show();
		//m_scrolledwindowPlaylists->show_all();
		//m_panedBody->show_all_children();
		show_all_children();
		std::cout << "got here[" << __LINE__ << ']' << std::endl;
	}
}

Main_win::~Main_win(){
	delete m_listviewformatTextPlaylists;
	delete m_listviewformatTextCurrentPlaylist;
	//delete m_eventboxPlaylists;
	//delete m_eventboxCurrentPlaylist;
	delete m_scrolledwindowPlaylists;
	delete m_scrolledwindowCurrentPlaylist;
}


void Main_win::on_button_Connect(){
	std::cout << "got here[" << __LINE__ << ']' << std::endl;
    for(int i = 0; i < m_connect_retrys;i++){
		std::cout << "got here[" << __LINE__ << ']' << std::endl;
		try{
			xmms2_client.connect( std::getenv("XMMS_PATH") );
			std::cout << "got here[" << __LINE__ << ']' << std::endl;
			refresh_playlists();
			refresh_playlist();
			std::cout << "connect count == " << ++connect_cout << std::endl;
			break;
		}
		catch(std::exception &e){
			std::cout << e.what() << std::endl;
			system("/usr/bin/xmms2-launcher"); // force xmms2d to start //
		}
	}
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

std::vector<Glib::ustring> Main_win::get_mediainfo(int id)
{

    std::vector<Glib::ustring> result;
	
	Xmms::Dict info = xmms2_client.medialib.getInfo( id );

	std::cout << "title = ";
	try {
        result.insert(result.end(), boost::get<std::string>(info["title"]));
		std::cout << info["title"] << std::endl;
	}
	catch( Xmms::no_such_key_error& err ) {
		result.insert(result.end(), "Title unknown");
		std::cout << "Title unknown" << std::endl;
	}
	
	std::cout << "artist = ";
	try {
        result.insert(result.end(), boost::get<std::string>(info["artist"]));
		std::cout << info["artist"] << std::endl;
	}
	catch( Xmms::no_such_key_error& err ) {
		result.insert(result.end(), "No artist");
		std::cout << "No artist" << std::endl;
	}

	std::cout << "album = ";
	try {
        result.insert(result.end(), boost::get<std::string>(info["album"]));
		std::cout << info["album"] << std::endl;
	}
	catch( Xmms::no_such_key_error& err ) {
		result.insert(result.end(), "No album");
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
        result.insert(result.end(), s);
	}
	catch( Xmms::no_such_key_error& err ) {
		result.insert(result.end(), "unknown duration");
		std::cout << "unknown duration" << std::endl;
	}

    std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
	return result;
}

void Main_win::refresh_playlist()
{
    std::cout << __FILE__ << '[' << __LINE__ << "] m_currentPlaylistName == " << m_currentPlaylistName << std::endl;
	Xmms::List< int > list = xmms2_client.playlist.listEntries(m_currentPlaylistName);
    std::cout << __FILE__ << '[' << __LINE__ << "] list.size() == " << list.size() << std::endl;

	m_listviewformatTextCurrentPlaylist->clear_items();

	for( Xmms::List< int >::const_iterator i(list.begin()), i_end(list.end()); i != i_end; ++i ){
		std::vector<Glib::ustring> result = get_mediainfo(*i);
		std::cout << __FILE__ << '[' << __LINE__ << "] result.size() == " << result.size() << std::endl;
		guint res = m_listviewformatTextCurrentPlaylist->append(result, *i);
		std::cout << __FILE__ << '[' << __LINE__ << "] res == " << res << std::endl;
	}
}


void Main_win::on_Playlists_clicked(Glib::ustring cp)
{
    m_currentPlaylistName = cp;
	std::cout << __FILE__ << '[' << __LINE__ << "] m_currentPlaylistName == " << m_currentPlaylistName << std::endl;
	refresh_playlist();
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


