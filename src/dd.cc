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

#include "dd.h"
#include <fmt/core.h>
//#include <boost/format.hpp>
#include <fmt/format.h>
//#include <fmt/format-inl.h>

DD::DD(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder)
  : Gtk::Dialog(cobject), m_builder(builder), 
    m_scrolledwindowDeleteContents(nullptr), m_listviewtextDeleteContents(nullptr), 
    m_radiobuttonDeleteNSCollection(nullptr), m_radiobuttonDeleteNSPlaqylist(nullptr), 
    m_comboboxtextDeleteCollection(nullptr), m_buttonDeleteIt(nullptr), m_action_area(nullptr), 
    m_NameSpace(rb_playlist)
{
	// m_scrolledwindowDeleteContents //
	m_builder->get_widget("scrolledwindowDeleteContents", m_scrolledwindowDeleteContents);
	if(m_scrolledwindowDeleteContents){
		//m_scrolledwindowDeleteContents->signal_toggled().connect( sigc::bind<RadioSelected>( sigc::mem_fun(*this, &DialogNewPlaylist::on_radiobutton), collection) );
		m_listviewtextDeleteContents = new LVT(6);
		m_scrolledwindowDeleteContents->add(*m_listviewtextDeleteContents);
		m_listviewtextDeleteContents->show();
		//m_listviewtextDeleteContents->set_hexpand();
		//m_listviewtextDeleteContents->set_vexpand();
		m_listviewtextDeleteContents->set_column_title(0, "media id");
		m_listviewtextDeleteContents->set_column_title(1, "tracknr");
		m_listviewtextDeleteContents->set_column_title(2, "Title");
		m_listviewtextDeleteContents->set_column_title(3, "Artist");
		m_listviewtextDeleteContents->set_column_title(4, "Album");
		m_listviewtextDeleteContents->set_column_title(5, "Duration");
		//m_listviewtextDeleteContents->signal_selection_changed().connect( sigc::mem_fun(*this, &DialogNewPlaylist::on_selection_changed_Available) );
	}
	// m_radiobuttonDeleteNSCollection //
	m_builder->get_widget("radiobuttonDeleteNSCollection", m_radiobuttonDeleteNSCollection);
	if(m_radiobuttonDeleteNSCollection){
		m_radiobuttonDeleteNSCollection->signal_toggled().connect( sigc::bind<NameSpace>( sigc::mem_fun(*this, &DD::on_radiobutton), rb_coll) );
	}
	// m_radiobuttonDeleteNSPlaqylist //
	m_builder->get_widget("radiobuttonDeleteNSPlaqylist", m_radiobuttonDeleteNSPlaqylist);
	if(m_radiobuttonDeleteNSPlaqylist){
		m_radiobuttonDeleteNSPlaqylist->signal_toggled().connect( sigc::bind<NameSpace>( sigc::mem_fun(*this, &DD::on_radiobutton), rb_playlist) );
	}
	// m_comboboxtextDeleteCollection //
	m_builder->get_widget("comboboxtextDeleteCollection", m_comboboxtextDeleteCollection);
	if(m_comboboxtextDeleteCollection){
		m_comboboxtextDeleteCollection->signal_changed().connect( sigc::mem_fun(*this, &DD::on_comboboxtextDeleteCollection_changed) );
	}
	// m_buttonDeleteIt //
	m_builder->get_widget("buttonDeleteIt", m_buttonDeleteIt);
	if(m_buttonDeleteIt){
		//m_buttonDeleteIt->signal_changed().connect( sigc::mem_fun(*this, &DD::on_comboboxtextDeleteCollection_changed) );
	}

	// m_action_area //
	m_builder->get_widget("dialogdelete-action_area", m_action_area);
	if(m_action_area){
		//m_action_area->signal_insert_text().connect( sigc::mem_fun(*this, &DR::on_entry_RenameNewName_changed) );
		m_action_area->set_layout(Gtk::BUTTONBOX_EDGE);
	}

	set_default_size(640, 480);
}

DD::~DD()
{
	delete m_listviewtextDeleteContents;
}

void DD::on_radiobutton(NameSpace ns)
{
	m_NameSpace = ns;
	m_comboboxtextDeleteCollection->remove_all();
	std::vector<Glib::ustring> lst = m_signal_namespace_changed.emit(m_NameSpace);
	std::cout << __FILE__ << "[" << __LINE__ 
		      << "] " << __PRETTY_FUNCTION__ << ": lst.size() == " 
		      << lst.size() << std::endl;
	for(std::vector<Glib::ustring>::iterator i = lst.begin(), i_end = lst.end(); i != i_end; ++i){
		m_comboboxtextDeleteCollection->append(*i);
	}
	on_comboboxtextDeleteCollection_changed();
}

void DD::on_comboboxtextDeleteCollection_changed()
{
	std::cout << __FILE__ << '[' << __LINE__ << "] " << __PRETTY_FUNCTION__ << std::endl;
	m_listviewtextDeleteContents->remove_all_rows();
	Glib::ustring item = m_comboboxtextDeleteCollection->get_active_text();
	std::vector<Glib::ustring> orderby;
	orderby.insert(orderby.end(), "album");
	orderby.insert(orderby.end(), "tracknr");
	orderby.insert(orderby.end(), "artist");
	//orderby.insert(orderby.end(), "title");
	//orderby.insert(orderby.end(), "duration");
	if(!item.empty()){
		std::vector<Xmms::Dict> lst = m_signal_playlist_coll_changed.emit(item, orderby, m_NameSpace);
		std::cout << __FILE__ << '[' << __LINE__ << "] list got: lst.size() == " << lst.size() << std::endl;
		for(std::vector<Xmms::Dict>::iterator i = lst.begin(), i_end = lst.end(); i != i_end; ++i){
			Xmms::Dict info = *i;
			int tracknr, id;
			std::string tracknr_str, id_str;
			try{
				id = boost::get<int>(info["id"]);
				//boost::format bf("%010d");
				//boost::format bf("%d");
				//bf % id;
				//id_str = bf.str();
			    id_str = fmt::format("{:d}"s, id);
			}
			catch( Xmms::no_such_key_error& err ) {
				id_str = "tracknr unknown";
				//std::cout << "tracknr unknown" << std::endl;
			}
			int row = m_listviewtextDeleteContents->append(id_str);
			try{
				tracknr = boost::get<int>(info["tracknr"]);
				//boost::format bf("%02d");
				//bf % tracknr;
				//tracknr_str = bf.str();
				tracknr_str = fmt::format("{:02d}"s, tracknr);
			}
			catch( Xmms::no_such_key_error& err ) {
				tracknr_str = "tracknr unknown";
				//std::cout << "tracknr unknown" << std::endl;
			}
			m_listviewtextDeleteContents->set_text(row, 1, tracknr_str);
			std::string title, artist, album;
			try{
				title = boost::get<std::string>(info["title"]);
			}
			catch( Xmms::no_such_key_error& err ) {
				title = "Title unknown";
				//std::cout << "Title unknown" << std::endl;
			}
			m_listviewtextDeleteContents->set_text(row, 2, title);
			
			try{
				artist = boost::get<std::string>(info["artist"]);
			}
			catch( Xmms::no_such_key_error& err ) {
				artist = "Artist unknown";
				//std::cout << "Artist unknown" << std::endl;
			}			
			m_listviewtextDeleteContents->set_text(row, 3, artist);
			
			try{
				album = boost::get<std::string>(info["album"]);
			}
			catch( Xmms::no_such_key_error& err ) {
				album = "No Album";
				//std::cout << "No Album" << std::endl;
			}			
			m_listviewtextDeleteContents->set_text(row, 4, album);

			try{
				int duration = boost::get< int >( info["duration"] );
				//std::cout << duration << "  (" << std::flush;
				double secs = (duration % 60000)/1000.0;
				duration /= 60000;
				int mins = duration % 60;
				duration /= 60;
				int hours = duration;
				//boost::format bf("%02d:%02d:%06.3f");
				//bf % hours % mins % secs;
				//std::cout << bf << ')' << std::endl;
				//std::string s = bf.str();
				std::string s = fmt::format("{0:02d}:{1:02d}:{2:06.3f}"s, hours, mins, secs);
				m_listviewtextDeleteContents->set_text(row, 5, s);
			}
			catch( Xmms::no_such_key_error& err ) {
				m_listviewtextDeleteContents->set_text(row, 5, "00:00:00");
				//std::cout << "00:00:00" << std::endl;
			}			
		}
	}
}

DD::type_signal_namespace_changed DD::signal_namespace_changed()
{
	return m_signal_namespace_changed;
}

DD::type_signal_playlist_coll_changed DD::signal_playlist_coll_changed()
{
	return m_signal_playlist_coll_changed;
}

void DD::set_namespace(NameSpace ns)
{
	switch(ns){
		case(rb_coll):
		{
			m_radiobuttonDeleteNSCollection->toggled();
			m_radiobuttonDeleteNSCollection->set_active(true);
			break;
		}
		case(rb_playlist):
		{
			m_radiobuttonDeleteNSPlaqylist->toggled();
			m_radiobuttonDeleteNSPlaqylist->set_active(true);
			break;
		}
	}
}

DD::NameSpace DD::get_namespace()
{
	return m_NameSpace;
}

void DD::set_coll(Glib::ustring coll_playlst)
{
	int index = get_index(m_comboboxtextDeleteCollection, coll_playlst);
	m_comboboxtextDeleteCollection->set_active(index);
}

Glib::ustring DD::get_coll()
{
	return m_comboboxtextDeleteCollection->get_active_text();
}

int DD::get_index(Gtk::ComboBoxText *combo, Glib::ustring text)
{
	Glib::RefPtr<Gtk::TreeModel> reftm = combo->get_model();
	Glib::RefPtr<Gtk::TreeStore> refStore = Glib::RefPtr<Gtk::TreeStore>::cast_dynamic(reftm);
	if(refStore){
		std::cout << __FILE__ << '[' << __LINE__ << "]  it's a Gtk::TreeStore" << std::endl;
		int indx = 0;
		for(Gtk::TreeIter i = refStore->children().begin(), i_end = refStore->children().end();
		    i = i_end; ++i, ++indx){
			Glib::ustring tmp;
			i->get_value(0, tmp);
			if(text == tmp) return indx;
		}
		return -1;
	}
	Glib::RefPtr<Gtk::ListStore> refLStore = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(reftm);
	if(refLStore){
		std::cout << __FILE__ << '[' << __LINE__ << "]  it's a Gtk::ListStore" << std::endl;
		int indx = 0;
		for(Gtk::TreeIter i = refLStore->children().begin(), i_end = refLStore->children().end();
		    i != i_end; ++i, ++indx){
			Glib::ustring tmp;
			i->get_value(0, tmp);
			if(text == tmp) return indx;
		}
	}
	return -1;
}








