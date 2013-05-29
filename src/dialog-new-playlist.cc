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

#include "dialog-new-playlist.h"
#include <boost/format.hpp>

DialogNewPlaylist::DialogNewPlaylist(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder)
  : Gtk::Dialog(cobject), m_builder(builder), 
    m_entryNewPlayList(0), m_filechooserbutton1(0), m_entryUrl(0),
    m_comboboxtextCollection(0), m_checkbuttonAddRecursive(0), 
    m_radiobuttonDirectory(0), m_radiobuttonUrl(0), m_radiobuttonCollection(0), 
    m_comboboxtextOrderBy(0), m_scrolledwindowOrderBy(0), m_listviewtextOrderBy(0), 
    m_scrolledwindowAvailable(0), m_listviewtextAvailable(0), 
    m_scrolledwindowColContent(0), m_toolbuttonMoveLeft(0), 
    m_toolbuttonMoveRight(0), m_toolbuttonUp(0), m_toolbuttonDown(0), 
    m_toolbuttonRevese(0), m_toolbuttonRefresh(0), /*m_filefilterSound(0), */
    m_addrecursive(true), m_radioselected(directory), m_orderby(atoz)
{
	//////////////////////////////////////////////////////////
	// ********** stuff inside dialogNewPlaylist ********** //
	//////////////////////////////////////////////////////////
	//*
	m_builder->get_widget("entryNewPlayList", m_entryNewPlayList);
	if(m_entryNewPlayList){
		//m_entryNewPlayList->signal_activate().connect( sigc::mem_fun(*this, &DialogNewPlaylist::on_button_Exit) );
	}
	m_builder->get_widget("filechooserbutton1", m_filechooserbutton1);
	if(m_filechooserbutton1){
		//m_filechooserbutton1->signal_activate().connect( sigc::mem_fun(*this, &DialogNewPlaylist::on_button_Exit) );
	}
	m_builder->get_widget("entryUrl", m_entryUrl);
	if(m_entryUrl){
		//m_entryUrl->signal_activate().connect( sigc::mem_fun(*this, &DialogNewPlaylist::on_button_Exit) );
	}
	m_builder->get_widget("comboboxtextCollection", m_comboboxtextCollection);
	if(m_comboboxtextCollection){
		m_comboboxtextCollection->signal_changed().connect( sigc::mem_fun(*this, &DialogNewPlaylist::on_combo_Coll_changed) );
	}
	m_builder->get_widget("comboboxtextOrderBy", m_comboboxtextOrderBy);
	if(m_comboboxtextOrderBy){
		m_comboboxtextOrderBy->signal_changed().connect( sigc::mem_fun(*this, &DialogNewPlaylist::on_comboOrderBy_changed) );
		m_comboboxtextOrderBy->set_active(0);
	}
	// checkbox //
	m_builder->get_widget("checkbuttonAddRecursive", m_checkbuttonAddRecursive);
	if(m_checkbuttonAddRecursive){
		m_checkbuttonAddRecursive->signal_toggled().connect( sigc::mem_fun(*this, &DialogNewPlaylist::on_checkbutton_AddRecursive) );
	}
	// radiobuttons //
	m_builder->get_widget("radiobuttonDirectory", m_radiobuttonDirectory);
	if(m_radiobuttonDirectory){
		m_radiobuttonDirectory->signal_toggled().connect( sigc::bind<RadioSelected>( sigc::mem_fun(*this, &DialogNewPlaylist::on_radiobutton), directory) );
	}
	m_builder->get_widget("radiobuttonUrl", m_radiobuttonUrl);
	if(m_radiobuttonUrl){
		m_radiobuttonUrl->signal_toggled().connect( sigc::bind<RadioSelected>( sigc::mem_fun(*this, &DialogNewPlaylist::on_radiobutton), url) );
	}
	m_builder->get_widget("radiobuttonCollection", m_radiobuttonCollection);
	if(m_radiobuttonCollection){
		m_radiobuttonCollection->signal_toggled().connect( sigc::bind<RadioSelected>( sigc::mem_fun(*this, &DialogNewPlaylist::on_radiobutton), collection) );
	}
    //*/
	m_builder->get_widget("scrolledwindowOrderBy", m_scrolledwindowOrderBy);
	if(m_scrolledwindowOrderBy){
		//m_scrolledwindowOrderBy->signal_toggled().connect( sigc::bind<RadioSelected>( sigc::mem_fun(*this, &DialogNewPlaylist::on_radiobutton), collection) );
		m_listviewtextOrderBy = new LVT(1);
		m_scrolledwindowOrderBy->add(*m_listviewtextOrderBy);
		m_listviewtextOrderBy->show();
		m_listviewtextOrderBy->set_hexpand();
		m_listviewtextOrderBy->set_vexpand();
		m_listviewtextOrderBy->set_column_title(0, "OrderBY");
		m_listviewtextOrderBy->signal_selection_changed().connect( sigc::mem_fun(*this, &DialogNewPlaylist::on_selection_changed_OrderBy) );
		m_listviewtextOrderBy->signal_dblclicked().connect( sigc::mem_fun(*this, &DialogNewPlaylist::on_toolbutton_Revese) );
/*		m_listviewtextOrderBy->append("title");
		m_listviewtextOrderBy->append("album");
		m_listviewtextOrderBy->append("id");
		m_listviewtextOrderBy->append("tracknr");
		m_listviewtextOrderBy->append("artist");
		m_listviewtextOrderBy->append("mime");*/
	}
	// m_scrolledwindowAvailable //
	m_builder->get_widget("scrolledwindowAvailable", m_scrolledwindowAvailable);
	if(m_scrolledwindowAvailable){
		//m_scrolledwindowAvailable->signal_toggled().connect( sigc::bind<RadioSelected>( sigc::mem_fun(*this, &DialogNewPlaylist::on_radiobutton), collection) );
		m_listviewtextAvailable = new LVT(1);
		m_scrolledwindowAvailable->add(*m_listviewtextAvailable);
		m_listviewtextAvailable->show();
		m_listviewtextAvailable->set_hexpand();
		m_listviewtextAvailable->set_vexpand();
		m_listviewtextAvailable->set_column_title(0, "Available");
		m_listviewtextAvailable->append("title");
		m_listviewtextAvailable->append("album");
		m_listviewtextAvailable->append("id");
		m_listviewtextAvailable->append("tracknr");
		m_listviewtextAvailable->append("artist");
		m_listviewtextAvailable->append("mime");
		m_listviewtextAvailable->signal_selection_changed().connect( sigc::mem_fun(*this, &DialogNewPlaylist::on_selection_changed_Available) );
	}
	// m_scrolledwindowColContent //
	m_builder->get_widget("scrolledwindowColContent", m_scrolledwindowColContent);
	if(m_scrolledwindowColContent){
		//m_scrolledwindowAvailable->signal_toggled().connect( sigc::bind<RadioSelected>( sigc::mem_fun(*this, &DialogNewPlaylist::on_radiobutton), collection) );
		m_listviewtextColContent = new LVT(5);
		m_scrolledwindowColContent->add(*m_listviewtextColContent);
		m_listviewtextColContent->show();
		//m_listviewtextColContent->set_hexpand();
		//m_listviewtextColContent->set_vexpand();
		m_listviewtextColContent->set_column_title(0, "tracknr");
		m_listviewtextColContent->set_column_title(1, "Title");
		m_listviewtextColContent->set_column_title(2, "Artist");
		m_listviewtextColContent->set_column_title(3, "Album");
		m_listviewtextColContent->set_column_title(4, "Duration");
		//m_listviewtextColContent->signal_selection_changed().connect( sigc::mem_fun(*this, &DialogNewPlaylist::on_selection_changed_Available) );
	}
	// m_toolbuttonMoveLeft //
	m_builder->get_widget("toolbuttonMoveLeft", m_toolbuttonMoveLeft);
	if(m_toolbuttonMoveLeft){
		m_toolbuttonMoveLeft->signal_clicked().connect( sigc::mem_fun(*this, &DialogNewPlaylist::on_toolbutton_MoveLeft) );
	}
	// m_toolbuttonMoveRight //
	m_builder->get_widget("toolbuttonMoveRight", m_toolbuttonMoveRight);
	if(m_toolbuttonMoveRight){
		m_toolbuttonMoveRight->signal_clicked().connect( sigc::mem_fun(*this, &DialogNewPlaylist::on_toolbutton_MoveRight) );
	}
	// m_toolbuttonUp //
	m_builder->get_widget("toolbuttonUp", m_toolbuttonUp);
	if(m_toolbuttonUp){
		m_toolbuttonUp->signal_clicked().connect( sigc::mem_fun(*this, &DialogNewPlaylist::on_toolbutton_Up) );
	}
	// m_toolbuttonDown //
	m_builder->get_widget("toolbuttonDown", m_toolbuttonDown);
	if(m_toolbuttonDown){
		m_toolbuttonDown->signal_clicked().connect( sigc::mem_fun(*this, &DialogNewPlaylist::on_toolbutton_Down) );
	}
	// m_toolbuttonRevese //
	m_builder->get_widget("toolbuttonRevese", m_toolbuttonRevese);
	if(m_toolbuttonRevese){
		m_toolbuttonRevese->signal_clicked().connect( sigc::mem_fun(*this, &DialogNewPlaylist::on_toolbutton_Revese) );
	}
	// m_toolbuttonRefresh //
	m_builder->get_widget("toolbuttonRefresh", m_toolbuttonRefresh);
	if(m_toolbuttonRefresh){
		m_toolbuttonRefresh->signal_clicked().connect( sigc::mem_fun(*this, &DialogNewPlaylist::on_toolbutton_Refresh) );
	}

	// m_filefilterSound //
	m_filefilterSound = Gtk::FileFilter::create();
	if(m_filefilterSound){
		//m_filefilterSound->signal_clicked().connect( sigc::mem_fun(*this, &DialogNewPlaylist::on_toolbutton_Refresh) );
		m_filefilterSound->set_name("sound files");
		m_filefilterSound->add_pattern("*.ogg");
		m_filefilterSound->add_pattern("*.mp3");
	}
}

DialogNewPlaylist::~DialogNewPlaylist()
{
}

void DialogNewPlaylist::on_checkbutton_AddRecursive()
{
	m_addrecursive = m_checkbuttonAddRecursive->get_active();
	if(m_addrecursive){
		std::cout << __FILE__ << '[' << __LINE__ << "]  checked" << std::endl;
		m_filechooserbutton1->set_action(Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
		m_filechooserbutton1->remove_filter(m_filefilterSound);
		m_filechooserbutton1->set_title("Select a Folder");
	}else{
		std::cout << __FILE__ << '[' << __LINE__ << "]  unchecked" << std::endl;
		m_filechooserbutton1->set_action(Gtk::FILE_CHOOSER_ACTION_OPEN);
		m_filechooserbutton1->add_filter(m_filefilterSound);
		m_filechooserbutton1->set_filter(m_filefilterSound);
		m_filechooserbutton1->set_title("Select a File");
	}
}

void DialogNewPlaylist::on_radiobutton(RadioSelected rs)
{
	m_radioselected = rs;
	switch(m_radioselected){
		case(directory):
			m_filechooserbutton1->set_sensitive(true);
			m_filechooserbutton1->set_can_focus(true);
			m_entryUrl->set_sensitive(false);
			m_entryUrl->set_can_focus(false);
			m_comboboxtextCollection->set_sensitive(false);
			m_comboboxtextCollection->set_can_focus(false);
			m_comboboxtextOrderBy->set_sensitive(false);
			m_comboboxtextOrderBy->set_can_focus(false);
			break;
		case(url):
			m_filechooserbutton1->set_sensitive(false);
			m_filechooserbutton1->set_can_focus(false);
			m_entryUrl->set_sensitive(true);
			m_entryUrl->set_can_focus(true);
			m_comboboxtextCollection->set_sensitive(false);
			m_comboboxtextCollection->set_can_focus(false);
			m_comboboxtextOrderBy->set_sensitive(false);
			m_comboboxtextOrderBy->set_can_focus(false);
			break;
		case(collection):
			m_filechooserbutton1->set_sensitive(false);
			m_filechooserbutton1->set_can_focus(false);
			m_entryUrl->set_sensitive(false);
			m_entryUrl->set_can_focus(false);
			m_comboboxtextCollection->set_sensitive(true);
			m_comboboxtextCollection->set_can_focus(true);
			m_comboboxtextOrderBy->set_sensitive(true);
			m_comboboxtextOrderBy->set_can_focus(true);
			break;
	}
}

void DialogNewPlaylist::on_selection_changed_OrderBy()
{
	std::cout << __FILE__ << '[' << __LINE__ << "]  selection changed OrderBy" << std::endl;
}

void DialogNewPlaylist::on_selection_changed_Available()
{
	std::cout << __FILE__ << '[' << __LINE__ << "]  selection changed Available" << std::endl;
}


void DialogNewPlaylist::on_comboOrderBy_changed()
{
	Glib::ustring item = m_comboboxtextOrderBy->get_active_text();
	if(!item.empty()){
		if(item == "A-Z"){
			m_orderby = atoz;
			std::cout << __FILE__ << '[' << __LINE__ << "]  A-Z" << std::endl;
		}else if(item == "Z-A"){
			m_orderby = ztoa;
			std::cout << __FILE__ << '[' << __LINE__ << "]  Z-A" << std::endl;
		}else if(item == "Edit Manually"){
			m_orderby = manual;
			std::cout << __FILE__ << '[' << __LINE__ << "]  Edit Manually" << std::endl;
		}
	}
}

void DialogNewPlaylist::on_combo_Coll_changed()
{
	std::cout << __FILE__ << '[' << __LINE__ << "] on_combo_Coll_changed" << std::endl;
	m_listviewtextColContent->remove_all_rows();
	Glib::ustring item = m_comboboxtextCollection->get_active_text();
	std::vector<Glib::ustring> orderby = get_order();
	if(!item.empty()){
		std::vector<Xmms::Dict> lst = m_signal_coll_changed.emit(item, orderby);
		std::cout << __FILE__ << '[' << __LINE__ << "] list got: lst.size() == " << lst.size() << std::endl;
		for(std::vector<Xmms::Dict>::iterator i = lst.begin(), i_end = lst.end(); i != i_end; ++i){
			Xmms::Dict info = *i;
			int tracknr;
			std::string tracknr_str;
			try{
				tracknr = boost::get<int>(info["tracknr"]);
				boost::format bf("%02d");
				bf % tracknr;
				tracknr_str = bf.str();
			}
			catch( Xmms::no_such_key_error& err ) {
				tracknr_str = "tracknr unknown";
				//std::cout << "tracknr unknown" << std::endl;
			}
			int row = m_listviewtextColContent->append(tracknr_str);
			std::string title, artist, album;
			try{
				title = boost::get<std::string>(info["title"]);
			}
			catch( Xmms::no_such_key_error& err ) {
				title = "Title unknown";
				//std::cout << "Title unknown" << std::endl;
			}
			m_listviewtextColContent->set_text(row, 1, title);
			
			try{
				artist = boost::get<std::string>(info["artist"]);
			}
			catch( Xmms::no_such_key_error& err ) {
				artist = "Artist unknown";
				//std::cout << "Artist unknown" << std::endl;
			}			
			m_listviewtextColContent->set_text(row, 2, artist);
			
			try{
				album = boost::get<std::string>(info["album"]);
			}
			catch( Xmms::no_such_key_error& err ) {
				album = "No Album";
				//std::cout << "No Album" << std::endl;
			}			
			m_listviewtextColContent->set_text(row, 3, album);

			try{
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
				m_listviewtextColContent->set_text(row, 4, s);
			}
			catch( Xmms::no_such_key_error& err ) {
				m_listviewtextColContent->set_text(row, 4, "00:00:00");
				//std::cout << "00:00:00" << std::endl;
			}			
		}
	}
}

DialogNewPlaylist::type_signal_coll_changed DialogNewPlaylist::signal_coll_changed()
{
	return m_signal_coll_changed;
}

void DialogNewPlaylist::on_toolbutton_MoveLeft()
{
	Gtk::ListViewText::SelectionList sellst = m_listviewtextAvailable->get_selected();
	if(sellst.empty()) return;
	Glib::RefPtr<Gtk::TreeSelection> ts = m_listviewtextAvailable->get_selection();
	Gtk::TreeModel::iterator sel_it = ts->get_selected();
	int sel = sellst[0];
	Glib::ustring selrow = m_listviewtextAvailable->get_text(sel);
	m_listviewtextOrderBy->append(selrow);
	//erase(sel_it);
	Glib::RefPtr<Gtk::TreeModel> reftm = m_listviewtextAvailable->get_model();
	Glib::RefPtr<Gtk::TreeStore> refStore = Glib::RefPtr<Gtk::TreeStore>::cast_dynamic(reftm);
	if(refStore){
		std::cout << __FILE__ << '[' << __LINE__ << "]  it's a Gtk::TreeStore" << std::endl;
		refStore->erase(sel_it);
		return;
	}
	Glib::RefPtr<Gtk::ListStore> refLStore = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(reftm);
	if(refLStore){
		std::cout << __FILE__ << '[' << __LINE__ << "]  it's a Gtk::ListStore" << std::endl;
		refLStore->erase(sel_it);
	}
}

void DialogNewPlaylist::on_toolbutton_MoveRight()
{
	Gtk::ListViewText::SelectionList sellst = m_listviewtextOrderBy->get_selected();
	if(sellst.empty()) return;
	Glib::RefPtr<Gtk::TreeSelection> ts = m_listviewtextOrderBy->get_selection();
	Gtk::TreeModel::iterator sel_it = ts->get_selected();
	int sel = sellst[0];
	Glib::ustring selrow = m_listviewtextOrderBy->get_text(sel);
	m_listviewtextAvailable->append(makeAsc(selrow));
	//erase(sel_it);
	Glib::RefPtr<Gtk::TreeModel> reftm = m_listviewtextOrderBy->get_model();
	Glib::RefPtr<Gtk::TreeStore> refStore = Glib::RefPtr<Gtk::TreeStore>::cast_dynamic(reftm);
	if(refStore){
		std::cout << __FILE__ << '[' << __LINE__ << "]  it's a Gtk::TreeStore" << std::endl;
		refStore->erase(sel_it);
		return;
	}
	Glib::RefPtr<Gtk::ListStore> refLStore = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(reftm);
	if(refLStore){
		std::cout << __FILE__ << '[' << __LINE__ << "]  it's a Gtk::ListStore" << std::endl;
		refLStore->erase(sel_it);
	}
}

void DialogNewPlaylist::on_toolbutton_Up()
{
	Gtk::ListViewText::SelectionList sellst = m_listviewtextOrderBy->get_selected();
	if(sellst.empty()) return;
	int sel = sellst[0];
	if(sel == 0) return; // already at top //
	Glib::RefPtr<Gtk::TreeSelection> ts = m_listviewtextOrderBy->get_selection();
	Gtk::TreeModel::iterator sel_it = ts->get_selected();
	Gtk::TreeModel::iterator new_it = sel_it;
	--new_it;
	Glib::RefPtr<Gtk::TreeModel> reftm = m_listviewtextOrderBy->get_model();
	Glib::RefPtr<Gtk::ListStore> refLStore = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(reftm);
	if(refLStore){
		std::cout << __FILE__ << '[' << __LINE__ << "]  it's a Gtk::ListStore" << std::endl;
		refLStore->iter_swap(sel_it, new_it);
	}
/*	Glib::ustring selrow = m_listviewtextOrderBy->get_text(sel);
	Glib::ustring newspot = m_listviewtextOrderBy->get_text(sel - 1);
	m_listviewtextOrderBy->set_text(sel - 1, 0, selrow);
	m_listviewtextOrderBy->set_text(sel, 0, newspot);*/
	//m_listviewtextOrderBy->set_selection(sel - 1);
}

void DialogNewPlaylist::on_toolbutton_Down()
{
	Gtk::ListViewText::SelectionList sellst = m_listviewtextOrderBy->get_selected();
	if(sellst.empty()) return;
	int sel = sellst[0];
	if(sel == m_listviewtextOrderBy->size() - 1) return; // already at bottom //
	Glib::RefPtr<Gtk::TreeSelection> ts = m_listviewtextOrderBy->get_selection();
	Gtk::TreeModel::iterator sel_it = ts->get_selected();
	Gtk::TreeModel::iterator new_it = sel_it;
	++new_it;
	Glib::RefPtr<Gtk::TreeModel> reftm = m_listviewtextOrderBy->get_model();
	Glib::RefPtr<Gtk::ListStore> refLStore = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(reftm);
	if(refLStore){
		std::cout << __FILE__ << '[' << __LINE__ << "]  it's a Gtk::ListStore" << std::endl;
		refLStore->iter_swap(sel_it, new_it);
	}
/*	Glib::ustring selrow = m_listviewtextOrderBy->get_text(sel);
	Glib::ustring newspot = m_listviewtextOrderBy->get_text(sel + 1);
	m_listviewtextOrderBy->set_text(sel + 1, 0, selrow);
	m_listviewtextOrderBy->set_text(sel, 0, newspot);*/
	//m_listviewtextOrderBy->set_selection(sel + 1);
}

void DialogNewPlaylist::on_toolbutton_Revese()
{
	Gtk::ListViewText::SelectionList sellst = m_listviewtextOrderBy->get_selected();
	if(sellst.empty()) return;
	int sel = sellst[0];
	Glib::ustring selrow = m_listviewtextOrderBy->get_text(sel);
	m_listviewtextOrderBy->set_text(sel, 0, toggleAscDesc(selrow));
}

void DialogNewPlaylist::on_toolbutton_Refresh()
{
	on_combo_Coll_changed();
}

Glib::ustring DialogNewPlaylist::makeAsc(Glib::ustring s)
{
	if(s[0] == '-'){
		s = s.substr(1);
	}
	return s;
}

Glib::ustring DialogNewPlaylist::toggleAscDesc(Glib::ustring s)
{
	if(s[0] == '-'){
		s = s.substr(1);
	}else{
		s = "-" + s;
	}
	return s;
}

void DialogNewPlaylist::set_collections(std::vector<Glib::ustring> colls)
{
	m_comboboxtextCollection->remove_all();
	for(std::vector<Glib::ustring>::iterator i = colls.begin(), i_end = colls.end(); i != i_end; ++i){
		m_comboboxtextCollection->append(*i);
	}
}

std::vector<Glib::ustring> DialogNewPlaylist::get_order()
{
	guint n = m_listviewtextOrderBy->size();
	std::vector<Glib::ustring> result;
	for(guint i = 0; i < n; ++i){
		result.insert(result.end(), m_listviewtextOrderBy->get_text(i));
	}
	return result;
}

DialogNewPlaylist::RadioSelected DialogNewPlaylist::get_radio_selected()
{
	return m_radioselected;
}

bool DialogNewPlaylist::get_addrecursive()
{
	return m_addrecursive;
}

Glib::ustring DialogNewPlaylist::get_playlist_name()
{
	return m_entryNewPlayList->get_text();
}

Glib::ustring DialogNewPlaylist::get_filename()
{
	return m_filechooserbutton1->get_filename();
}

Glib::ustring DialogNewPlaylist::get_url()
{
	return m_entryUrl->get_text();
}

Glib::ustring DialogNewPlaylist::get_collection_name()
{
	return m_comboboxtextCollection->get_active_text();
}








