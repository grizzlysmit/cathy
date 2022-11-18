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

#include "dr.h"
#include <fmt/core.h>
//#include <boost/format.hpp>
#include <fmt/format.h>
//#include <fmt/format-inl.h>

DR::DR(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder)
  : Gtk::Dialog(cobject), m_builder(builder), 
    m_scrolledwindowRenameContents(nullptr), m_listviewtextRenameContents(nullptr), 
    m_radiobuttonRenameNSCollection(nullptr), m_radiobuttonRenameNSPlaqylist(nullptr), 
    m_comboboxtextRenameCollection(nullptr), m_buttonRenameOK(nullptr), m_entryRenameNewName(nullptr), 
    m_action_area(nullptr), 
    m_NameSpace(rb_playlist)
{
	// m_scrolledwindowRenameContents //
	m_builder->get_widget("scrolledwindowRenameContents", m_scrolledwindowRenameContents);
	if(m_scrolledwindowRenameContents){
		//m_scrolledwindowRenameContents->signal_toggled().connect( sigc::bind<RadioSelected>( sigc::mem_fun(*this, &DialogNewPlaylist::on_radiobutton), collection) );
		m_listviewtextRenameContents = new LVT(6);
		m_scrolledwindowRenameContents->add(*m_listviewtextRenameContents);
		m_listviewtextRenameContents->show();
		//m_listviewtextRenameContents->set_hexpand();
		//m_listviewtextRenameContents->set_vexpand();
		m_listviewtextRenameContents->set_column_title(0, "media id");
		m_listviewtextRenameContents->set_column_title(1, "tracknr");
		m_listviewtextRenameContents->set_column_title(2, "Title");
		m_listviewtextRenameContents->set_column_title(3, "Artist");
		m_listviewtextRenameContents->set_column_title(4, "Album");
		m_listviewtextRenameContents->set_column_title(5, "Duration");
		//m_listviewtextRenameContents->signal_selection_changed().connect( sigc::mem_fun(*this, &DialogNewPlaylist::on_selection_changed_Available) );
	}
	// m_radiobuttonRenameNSCollection //
	m_builder->get_widget("radiobuttonRenameNSCollection", m_radiobuttonRenameNSCollection);
	if(m_radiobuttonRenameNSCollection){
		m_radiobuttonRenameNSCollection->signal_toggled().connect( sigc::bind<NameSpace>( sigc::mem_fun(*this, &DR::on_radiobutton), rb_coll) );
	}
	// m_radiobuttonRenameNSPlaqylist //
	m_builder->get_widget("radiobuttonRenameNSPlaqylist", m_radiobuttonRenameNSPlaqylist);
	if(m_radiobuttonRenameNSPlaqylist){
		m_radiobuttonRenameNSPlaqylist->signal_toggled().connect( sigc::bind<NameSpace>( sigc::mem_fun(*this, &DR::on_radiobutton), rb_playlist) );
	}
	// m_comboboxtextRenameCollection //
	m_builder->get_widget("comboboxtextRenameCollection", m_comboboxtextRenameCollection);
	if(m_comboboxtextRenameCollection){
		m_comboboxtextRenameCollection->signal_changed().connect( sigc::mem_fun(*this, &DR::on_comboboxtextRenameCollection_changed) );
	}
	// m_buttonRenameOK //
	m_builder->get_widget("buttonRenameOK", m_buttonRenameOK);
	if(m_buttonRenameOK){
		//m_buttonRenameOK->signal_changed().connect( sigc::mem_fun(*this, &DR::on_comboboxtextRenameCollection_changed) );
	}

	// m_entryRenameNewName //
	m_builder->get_widget("entryRenameNewName", m_entryRenameNewName);
	if(m_entryRenameNewName){
		m_entryRenameNewName->signal_insert_text().connect( sigc::mem_fun(*this, &DR::on_entry_RenameNewName_changed) );
	}

	// m_action_area //
	m_builder->get_widget("dialogrename-action_area", m_action_area);
	if(m_action_area){
		//m_action_area->signal_insert_text().connect( sigc::mem_fun(*this, &DR::on_entry_RenameNewName_changed) );
		m_action_area->set_layout(Gtk::BUTTONBOX_EDGE);
	}

	set_default_size(640, 480);
}

DR::~DR()
{
	delete m_listviewtextRenameContents;
}

void DR::on_radiobutton(NameSpace ns)
{
	m_NameSpace = ns;
	m_comboboxtextRenameCollection->remove_all();
	std::vector<Glib::ustring> lst = m_signal_namespace_changed.emit(m_NameSpace);
	std::cout << __FILE__ << "[" << __LINE__ 
		      << "] " << __PRETTY_FUNCTION__ << ": lst.size() == " 
		      << lst.size() << std::endl;
	for(std::vector<Glib::ustring>::iterator i = lst.begin(), i_end = lst.end(); i != i_end; ++i){
		m_comboboxtextRenameCollection->append(*i);
	}
	on_comboboxtextRenameCollection_changed();
}

void DR::on_comboboxtextRenameCollection_changed()
{
	std::cout << __FILE__ << '[' << __LINE__ << "] " << __PRETTY_FUNCTION__ << std::endl;
	m_listviewtextRenameContents->remove_all_rows();
	Glib::ustring item = m_comboboxtextRenameCollection->get_active_text();
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
			    id_str = fmt::format("{:d}", id);
			}
			catch( Xmms::no_such_key_error& err ) {
				id_str = "tracknr unknown";
				//std::cout << "tracknr unknown" << std::endl;
			}
			catch(...){
				id_str = "tracknr unknown";
			}
			int row = m_listviewtextRenameContents->append(id_str);
			try{
				tracknr = boost::get<int>(info["tracknr"]);
				//boost::format bf("%02d");
				//bf % tracknr;
				//tracknr_str = bf.str();
				tracknr_str = fmt::format("{:02d}", tracknr);
			}
			catch( Xmms::no_such_key_error& err ) {
				tracknr_str = "tracknr unknown";
				//std::cout << "tracknr unknown" << std::endl;
			}
			catch(...){
				tracknr_str = "tracknr unknown";
			}
			m_listviewtextRenameContents->set_text(row, 1, tracknr_str);
			std::string title, artist, album;
			try{
				title = boost::get<std::string>(info["title"]);
			}
			catch( Xmms::no_such_key_error& err ) {
				title = "Title unknown";
				//std::cout << "Title unknown" << std::endl;
			}
			m_listviewtextRenameContents->set_text(row, 2, title);
			
			try{
				artist = boost::get<std::string>(info["artist"]);
			}
			catch( Xmms::no_such_key_error& err ) {
				artist = "Artist unknown";
				//std::cout << "Artist unknown" << std::endl;
			}
			catch(...){
				artist = "Artist unknown";
			}
			m_listviewtextRenameContents->set_text(row, 3, artist);
			
			try{
				album = boost::get<std::string>(info["album"]);
			}
			catch( Xmms::no_such_key_error& err ) {
				album = "No Album";
				//std::cout << "No Album" << std::endl;
			}
			catch(...){
				album = "No Album";
			}
			m_listviewtextRenameContents->set_text(row, 4, album);

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
				std::string s = fmt::format("{0:02d}:{1:02d}:{2:06.3f}", hours, mins, secs);
				m_listviewtextRenameContents->set_text(row, 5, s);
			}
			catch( Xmms::no_such_key_error& err ) {
				m_listviewtextRenameContents->set_text(row, 5, "00:00:00");
				//std::cout << "00:00:00" << std::endl;
			}
			catch(...){
				m_listviewtextRenameContents->set_text(row, 5, "00:00:00");
			}
		}
	}
}

DR::type_signal_namespace_changed DR::signal_namespace_changed()
{
	return m_signal_namespace_changed;
}

DR::type_signal_playlist_coll_changed DR::signal_playlist_coll_changed()
{
	return m_signal_playlist_coll_changed;
}

void DR::set_namespace(NameSpace ns)
{
	switch(ns){
		case(rb_coll):
		{
			m_radiobuttonRenameNSCollection->toggled();
			m_radiobuttonRenameNSCollection->set_active(true);
			break;
		}
		case(rb_playlist):
		{
			m_radiobuttonRenameNSPlaqylist->toggled();
			m_radiobuttonRenameNSPlaqylist->set_active(true);
			break;
		}
	}
}

DR::NameSpace DR::get_namespace()
{
	return m_NameSpace;
}

void DR::set_coll(Glib::ustring coll_playlst)
{
	int index = get_index(m_comboboxtextRenameCollection, coll_playlst);
	m_comboboxtextRenameCollection->set_active(index);
}

Glib::ustring DR::get_coll()
{
	return m_comboboxtextRenameCollection->get_active_text();
}

int DR::get_index(Gtk::ComboBoxText *combo, Glib::ustring text)
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

Glib::ustring DR::get_newname()
{
	return m_entryRenameNewName->get_text();
}


void DR::on_entry_RenameNewName_changed(const Glib::ustring& s,int *i)
{
	std::cout << __FILE__ << '[' << __LINE__ << "] " << __PRETTY_FUNCTION__ << " s == " << s << std::endl;
	Glib::ustring text = m_entryRenameNewName->get_text();
	if(valid_collection_name(text)){
		m_buttonRenameOK->set_sensitive(true);
	}else{
		m_buttonRenameOK->set_sensitive(false);
	}
}

bool DR::valid_collection_name(Glib::ustring name)
{
	if(name.empty()) return false;
	Glib::ustring invalid(" \n\t\r");
	size_t pos = name.find_first_of(invalid);
	return (pos == Glib::ustring::npos);
}








