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
#include <fmt/core.h>
#include <fmt/format.h>
//#include <fmt/format-inl.h>

DialogNewPlaylist::DialogNewPlaylist(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder)
  : Gtk::Dialog(cobject), m_builder(builder), 
    m_buttonCreate(nullptr), 
    m_entryNewPlayList(nullptr), m_filechooserbutton1(nullptr), m_entryUrl(nullptr),
    m_comboboxtextCollection(nullptr), m_checkbuttonAddRecursive(nullptr), 
    m_radiobuttonDirectory(nullptr), m_radiobuttonUrl(nullptr), m_radiobuttonCollection(nullptr), 
    /*m_comboboxtextOrderBy(nullptr),*/ m_scrolledwindowOrderBy(nullptr), m_listviewtextOrderBy(nullptr), 
    m_scrolledwindowAvailable(nullptr), m_listviewtextAvailable(nullptr), 
    m_scrolledwindowColContent(nullptr), m_toolbuttonMoveLeft(nullptr), 
    m_toolbuttonMoveRight(nullptr), m_toolbuttonUp(nullptr), m_toolbuttonDown(nullptr), 
    m_toolbuttonRevese(nullptr), m_toolbuttonRefresh(nullptr), m_notebookMain(nullptr), 
    m_scrolledwindowTracksChoosen(nullptr), m_listviewtextTracksChoosen(nullptr), 
    m_scrolledwindowTracksAvailble(nullptr), m_listviewtextTracksAvailble(nullptr), 
    m_comboboxtextNamespace(nullptr), m_comboboxtextPlayListCollection(nullptr), 
    m_toolbuttonTrackUP(nullptr), m_toolbuttonChooseTrack(nullptr), 
    m_toolbuttonRetractTrack(nullptr), m_toolbuttonTrackDown(nullptr), m_action_area(nullptr), 
    m_addrecursive(true), m_radioselected(directory), m_orderby(atoz)
{
	//////////////////////////////////////////////////////////
	// ********** stuff inside dialogNewPlaylist ********** //
	//////////////////////////////////////////////////////////
	//*
	m_builder->get_widget("buttonCreate", m_buttonCreate);
	if(m_buttonCreate){
		//m_buttonCreate->signal_insert_text().connect( sigc::mem_fun(*this, &DialogNewPlaylist::on_entry_NewPlayList_changed) );
	}
	m_builder->get_widget("entryNewPlayList", m_entryNewPlayList);
	if(m_entryNewPlayList){
		m_entryNewPlayList->signal_insert_text().connect( sigc::mem_fun(*this, &DialogNewPlaylist::on_entry_NewPlayList_changed) );
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
/*	m_builder->get_widget("comboboxtextOrderBy", m_comboboxtextOrderBy);
	if(m_comboboxtextOrderBy){
		m_comboboxtextOrderBy->signal_changed().connect( sigc::mem_fun(*this, &DialogNewPlaylist::on_comboOrderBy_changed) );
		m_comboboxtextOrderBy->set_active(0);
	}*/
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
		m_listviewtextAvailable->signal_dblclicked().connect( sigc::mem_fun(*this, &DialogNewPlaylist::on_toolbutton_MoveLeft) );
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
	// m_scrolledwindowTracksChoosen //
	m_builder->get_widget("scrolledwindowTracksChoosen", m_scrolledwindowTracksChoosen);
	if(m_scrolledwindowTracksChoosen){
		//m_scrolledwindowTracksChoosen->signal_toggled().connect( sigc::bind<RadioSelected>( sigc::mem_fun(*this, &DialogNewPlaylist::on_radiobutton), collection) );
		m_listviewtextTracksChoosen = new LVT(6);
		m_scrolledwindowTracksChoosen->add(*m_listviewtextTracksChoosen);
		m_listviewtextTracksChoosen->show();
		//m_listviewtextTracksChoosen->set_hexpand();
		//m_listviewtextTracksChoosen->set_vexpand();
		m_listviewtextTracksChoosen->set_column_title(0, "media id");
		m_listviewtextTracksChoosen->set_column_title(1, "tracknr");
		m_listviewtextTracksChoosen->set_column_title(2, "Title");
		m_listviewtextTracksChoosen->set_column_title(3, "Artist");
		m_listviewtextTracksChoosen->set_column_title(4, "Album");
		m_listviewtextTracksChoosen->set_column_title(5, "Duration");
		//m_listviewtextTracksChoosen->signal_selection_changed().connect( sigc::mem_fun(*this, &DialogNewPlaylist::on_selection_changed_Available) );
	}
	// m_scrolledwindowTracksAvailble //
	m_builder->get_widget("scrolledwindowTracksAvailble", m_scrolledwindowTracksAvailble);
	if(m_scrolledwindowTracksAvailble){
		//m_scrolledwindowTracksAvailble->signal_toggled().connect( sigc::bind<RadioSelected>( sigc::mem_fun(*this, &DialogNewPlaylist::on_radiobutton), collection) );
		m_listviewtextTracksAvailble = new LVT(6);
		m_scrolledwindowTracksAvailble->add(*m_listviewtextTracksAvailble);
		m_listviewtextTracksAvailble->show();
		//m_listviewtextTracksAvailble->set_hexpand();
		//m_listviewtextTracksAvailble->set_vexpand();
		m_listviewtextTracksAvailble->set_column_title(0, "media id");
		m_listviewtextTracksAvailble->set_column_title(1, "tracknr");
		m_listviewtextTracksAvailble->set_column_title(2, "Title");
		m_listviewtextTracksAvailble->set_column_title(3, "Artist");
		m_listviewtextTracksAvailble->set_column_title(4, "Album");
		m_listviewtextTracksAvailble->set_column_title(5, "Duration");
		//m_listviewtextTracksAvailble->signal_selection_changed().connect( sigc::mem_fun(*this, &DialogNewPlaylist::on_selection_changed_Available) );
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

	// m_comboboxtextNamespace //
	m_builder->get_widget("comboboxtextNamespace", m_comboboxtextNamespace);
	if(m_comboboxtextNamespace){
		m_comboboxtextNamespace->signal_changed().connect( sigc::mem_fun(*this, &DialogNewPlaylist::on_comboboxtextNamespace_changed) );
	}
	// m_comboboxtextPlayListCollection //
	m_builder->get_widget("comboboxtextPlayListCollection", m_comboboxtextPlayListCollection);
	if(m_comboboxtextPlayListCollection){
		m_comboboxtextPlayListCollection->signal_changed().connect( sigc::mem_fun(*this, &DialogNewPlaylist::on_comboboxtextPlayListCollection_changed) );
	}
	// m_toolbuttonTrackUP //
	m_builder->get_widget("toolbuttonTrackUP", m_toolbuttonTrackUP);
	if(m_toolbuttonTrackUP){
		m_toolbuttonTrackUP->signal_clicked().connect( sigc::mem_fun(*this, &DialogNewPlaylist::on_toolbutton_TrackUP) );
	}
	// m_toolbuttonChooseTrack //
	m_builder->get_widget("toolbuttonChooseTrack", m_toolbuttonChooseTrack);
	if(m_toolbuttonChooseTrack){
		m_toolbuttonChooseTrack->signal_clicked().connect( sigc::mem_fun(*this, &DialogNewPlaylist::on_toolbutton_ChooseTrack) );
	}
	// m_toolbuttonRetractTrack //
	m_builder->get_widget("toolbuttonRetractTrack", m_toolbuttonRetractTrack);
	if(m_toolbuttonRetractTrack){
		m_toolbuttonRetractTrack->signal_clicked().connect( sigc::mem_fun(*this, &DialogNewPlaylist::on_toolbutton_RetractTrack) );
	}
	// m_toolbuttonTrackDown //
	m_builder->get_widget("toolbuttonTrackDown", m_toolbuttonTrackDown);
	if(m_toolbuttonTrackDown){
		m_toolbuttonTrackDown->signal_clicked().connect( sigc::mem_fun(*this, &DialogNewPlaylist::on_toolbutton_TrackDown) );
	}

	// m_notebookMain //
	m_builder->get_widget("notebookMain", m_notebookMain);
	if(m_notebookMain){
		m_notebookMain->signal_switch_page().connect( sigc::mem_fun(*this, &DialogNewPlaylist::on_notebookMain_switch_page) );
	}
	
	// m_action_area //
	m_builder->get_widget("dialognewplaylist-action_area", m_action_area);
	if(m_action_area){
		//m_action_area->signal_insert_text().connect( sigc::mem_fun(*this, &DR::on_entry_RenameNewName_changed) );
		m_action_area->set_layout(Gtk::BUTTONBOX_EDGE);
	}

	set_default_size(800, 600);
}

DialogNewPlaylist::~DialogNewPlaylist()
{
	delete m_listviewtextOrderBy;
	delete m_listviewtextAvailable;
	delete m_listviewtextColContent;
	delete m_listviewtextTracksChoosen;
	delete m_listviewtextTracksAvailble;
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
			//m_comboboxtextOrderBy->set_sensitive(false);
			//m_comboboxtextOrderBy->set_can_focus(false);
			break;
		case(url):
			m_filechooserbutton1->set_sensitive(false);
			m_filechooserbutton1->set_can_focus(false);
			m_entryUrl->set_sensitive(true);
			m_entryUrl->set_can_focus(true);
			m_comboboxtextCollection->set_sensitive(false);
			m_comboboxtextCollection->set_can_focus(false);
			//m_comboboxtextOrderBy->set_sensitive(false);
			//m_comboboxtextOrderBy->set_can_focus(false);
			break;
		case(collection):
			m_filechooserbutton1->set_sensitive(false);
			m_filechooserbutton1->set_can_focus(false);
			m_entryUrl->set_sensitive(false);
			m_entryUrl->set_can_focus(false);
			m_comboboxtextCollection->set_sensitive(true);
			m_comboboxtextCollection->set_can_focus(true);
			//m_comboboxtextOrderBy->set_sensitive(true);
			//m_comboboxtextOrderBy->set_can_focus(true);
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


/*void DialogNewPlaylist::on_comboOrderBy_changed()
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
}*/

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
				//boost::format bf("%02d");
				//bf % tracknr;
				//tracknr_str = bf.str();
			    tracknr_str = fmt::format("{:02d}", tracknr);
			}
			catch( Xmms::no_such_key_error& err ) {
				tracknr_str = "tracknr unknown";
				//std::cout << "tracknr unknown" << std::endl;
			}
			catch(std::exception& err){
				tracknr_str = "tracknr unknown";
			}
			catch(...){
				tracknr_str = "tracknr unknown";
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
			catch(std::exception& err){
				title = "Title unkown";
			}
			catch(...){
				title = "Title unkown";
			}
			m_listviewtextColContent->set_text(row, 1, title);
			
			try{
				artist = boost::get<std::string>(info["artist"]);
			}
			catch( Xmms::no_such_key_error& err ) {
				artist = "Artist unknown";
				//std::cout << "Artist unknown" << std::endl;
			}
			catch(std::exception& err){
				artist = "Artist unkown";
			}
			catch(...){
				artist = "Artist unkown";
			}
			m_listviewtextColContent->set_text(row, 2, artist);
			
			try{
				album = boost::get<std::string>(info["album"]);
			}
			catch( Xmms::no_such_key_error& err ) {
				album = "No Album";
				//std::cout << "No Album" << std::endl;
			}			
			catch(std::exception& err){
				album = "No Album";
			}			
			catch(...){
				album = "No Album";
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
				//boost::format bf("%02d:%02d:%06.3f");
				//bf % hours % mins % secs;
				//std::cout << bf << ')' << std::endl;
				//std::string s = bf.str();
				std::string s = fmt::format("{0:02d}:{1:02d}:{2:06.3}", hours, mins, secs);
				m_listviewtextColContent->set_text(row, 4, s);
			}
			catch( Xmms::no_such_key_error& err ) {
				m_listviewtextColContent->set_text(row, 4, "00:00:00");
				//std::cout << "00:00:00" << std::endl;
			}
			catch(std::exception& err){
				m_listviewtextColContent->set_text(row, 4, "00:00:00");
			}
			catch(...){
				m_listviewtextColContent->set_text(row, 4, "00:00:00");
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
	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ //
	switch(m_Page){
		case(page0):
		{
			switch(m_radioselected){
				case(directory):
				{
					break;
				}
				case(url):
				{
					break;
				}
				case(collection):
				{
					on_combo_Coll_changed();
					break;
				}
				default:
				{
					// TODO WTF how did I get here //
				}
			}
			break;
		}
		case(pageSelect):
		{
			on_comboboxtextPlayListCollection_changed();
			break;
		}
		default:
		{
			// todo how did we get here //
		}
	}
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

bool DialogNewPlaylist::valid_playlist_name(Glib::ustring name)
{
	if(name.empty()) return false;
	Glib::ustring invalid(" \n\t\r");
	size_t pos = name.find_first_of(invalid);
	return (pos == Glib::ustring::npos);
}

void DialogNewPlaylist::on_entry_NewPlayList_changed(const Glib::ustring& s,int *i)
{
	std::cout << __FILE__ << '[' << __LINE__ << "] " << __PRETTY_FUNCTION__ << " s == " << s << std::endl;
	Glib::ustring text = m_entryNewPlayList->get_text();
	if(valid_playlist_name(text)){
		m_buttonCreate->set_sensitive(true);
	}else{
		m_buttonCreate->set_sensitive(false);
	}
}

void DialogNewPlaylist::namespaces_remove_all()
{
	m_comboboxtextNamespace->remove_all();
}

void DialogNewPlaylist::namespaces_append(Glib::ustring namespaces)
{
	m_comboboxtextNamespace->append(namespaces);
}

void DialogNewPlaylist::on_comboboxtextNamespace_changed()
{
	m_comboboxtextPlayListCollection->remove_all();
	Glib::ustring _namespace = m_comboboxtextNamespace->get_active_text();
	std::cout << __FILE__ << "[" << __LINE__ 
		      << "] " << __PRETTY_FUNCTION__ << ": _namespace == " 
			  << _namespace << std::endl;
	std::vector<Glib::ustring> lst = m_signal_namespace_changed.emit(_namespace);
	std::cout << __FILE__ << "[" << __LINE__ 
		      << "] " << __PRETTY_FUNCTION__ << ": lst.size() == " 
			  << lst.size() << std::endl;
	for(std::vector<Glib::ustring>::iterator i = lst.begin(), i_end = lst.end(); i != i_end; ++i){
		m_comboboxtextPlayListCollection->append(*i);
	}
}

DialogNewPlaylist::type_signal_namespace_changed DialogNewPlaylist::signal_namespace_changed()
{
	return m_signal_namespace_changed;
}

DialogNewPlaylist::type_signal_playlist_coll_changed DialogNewPlaylist::signal_playlist_coll_changed()
{
	return m_signal_playlist_coll_changed;
}

void DialogNewPlaylist::on_comboboxtextPlayListCollection_changed()
{
	std::cout << __FILE__ << '[' << __LINE__ << "] " << __PRETTY_FUNCTION__ << std::endl;
	m_listviewtextTracksAvailble->remove_all_rows();
	Glib::ustring item = m_comboboxtextPlayListCollection->get_active_text();
	Glib::ustring _namespace = m_comboboxtextNamespace->get_active_text();
	std::cout << __FILE__ << "[" << __LINE__ 
		      << "] " << __PRETTY_FUNCTION__ << ": _namespace == " 
			  << _namespace << std::endl;
	std::vector<Glib::ustring> orderby = get_order();
	if(!item.empty() && !_namespace.empty()){
		std::vector<Xmms::Dict> lst = m_signal_playlist_coll_changed.emit(item, orderby, _namespace);
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
			catch(std::exception& err){
				id_str = "tracknr unknown";
			}
			catch(...){
				id_str = "tracknr unknown";
			}
			int row = m_listviewtextTracksAvailble->append(id_str);
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
			catch(std::exception& err){
				tracknr_str = "tracknr unknown";
			}
			catch(...){
				tracknr_str = "tracknr unknown";
			}
			m_listviewtextTracksAvailble->set_text(row, 1, tracknr_str);
			std::string title, artist, album;
			try{
				title = boost::get<std::string>(info["title"]);
			}
			catch( Xmms::no_such_key_error& err ) {
				title = "Title unknown";
				//std::cout << "Title unknown" << std::endl;
			}
			catch(std::exception& err){
				title = "Title unknown";
			}
			catch(...){
				title = "Title unknown";
			}
			m_listviewtextTracksAvailble->set_text(row, 2, title);
			
			try{
				artist = boost::get<std::string>(info["artist"]);
			}
			catch( Xmms::no_such_key_error& err ) {
				artist = "Artist unknown";
				//std::cout << "Artist unknown" << std::endl;
			}
			catch(std::exception& err){
				artist = "Artist unknown";
			}
			catch(...){
				artist = "Artist unknown";
			}			
			m_listviewtextTracksAvailble->set_text(row, 3, artist);
			
			try{
				album = boost::get<std::string>(info["album"]);
			}
			catch( Xmms::no_such_key_error& err ) {
				album = "No Album";
				//std::cout << "No Album" << std::endl;
			}
			catch(std::exception& err){
				album = "No Album";
			}
			catch(...){
				album = "No Album";
			}			
			m_listviewtextTracksAvailble->set_text(row, 4, album);

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
				m_listviewtextTracksAvailble->set_text(row, 5, s);
			}
			catch( Xmms::no_such_key_error& err ) {
				m_listviewtextTracksAvailble->set_text(row, 5, "00:00:00");
				//std::cout << "00:00:00" << std::endl;
			}
			catch(std::exception& err){
				m_listviewtextTracksAvailble->set_text(row, 5, "00:00:00");
			}
			catch(...){
				m_listviewtextTracksAvailble->set_text(row, 5, "00:00:00");
			}			
		}
	}
}

void DialogNewPlaylist::on_notebookMain_switch_page(Gtk::Widget* page, guint page_num)
{
	// m_notebookMain //
	switch(page_num){
		case(0):
		{
			m_Page = page0;
			break;
		}
		case(1):
		{
			m_Page = pageSelect;
			break;
		}
		default:
		{
			// TODO error handling unknown page //
		}
	}
}

DialogNewPlaylist::Page_type DialogNewPlaylist::get_page()
{
	return m_Page;
}

void DialogNewPlaylist::on_toolbutton_TrackUP()
{
	Gtk::ListViewText::SelectionList sellst = m_listviewtextTracksChoosen->get_selected();
	if(sellst.empty()) return;
	int sel = sellst[0];
	if(sel == 0) return; // already at top //
	Glib::RefPtr<Gtk::TreeSelection> ts = m_listviewtextTracksChoosen->get_selection();
	Gtk::TreeModel::iterator sel_it = ts->get_selected();
	Gtk::TreeModel::iterator new_it = sel_it;
	--new_it;
	Glib::RefPtr<Gtk::TreeModel> reftm = m_listviewtextTracksChoosen->get_model();
	Glib::RefPtr<Gtk::ListStore> refLStore = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(reftm);
	if(refLStore){
		std::cout << __FILE__ << '[' << __LINE__ << "]  it's a Gtk::ListStore" << std::endl;
		refLStore->iter_swap(sel_it, new_it);
	}
}

void DialogNewPlaylist::on_toolbutton_ChooseTrack()
{
	Gtk::ListViewText::SelectionList sellst = m_listviewtextTracksAvailble->get_selected();
	if(sellst.empty()) return;
	Glib::RefPtr<Gtk::TreeSelection> ts = m_listviewtextTracksAvailble->get_selection();
	Gtk::TreeModel::iterator sel_it = ts->get_selected();
	int sel = sellst[0];
	Glib::ustring selrow = m_listviewtextTracksAvailble->get_text(sel);
	int row = m_listviewtextTracksChoosen->append(selrow);
	for(int c = 1; c < 6; c++){
		Glib::ustring tmp = m_listviewtextTracksAvailble->get_text(sel, c);
		m_listviewtextTracksChoosen->set_text(row, c, tmp);
	}
	//erase(sel_it);
	Glib::RefPtr<Gtk::TreeModel> reftm = m_listviewtextTracksAvailble->get_model();
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

void DialogNewPlaylist::on_toolbutton_RetractTrack()
{
	Gtk::ListViewText::SelectionList sellst = m_listviewtextTracksChoosen->get_selected();
	if(sellst.empty()) return;
	Glib::RefPtr<Gtk::TreeSelection> ts = m_listviewtextTracksChoosen->get_selection();
	Gtk::TreeModel::iterator sel_it = ts->get_selected();
	int sel = sellst[0];
	Glib::ustring selrow = m_listviewtextTracksChoosen->get_text(sel);
	int row = m_listviewtextTracksAvailble->append(makeAsc(selrow));
	for(int c = 1; c < 6; c++){
		Glib::ustring tmp = m_listviewtextTracksChoosen->get_text(sel, c);
		m_listviewtextTracksAvailble->set_text(row, c, tmp);
	}
	//erase(sel_it);
	Glib::RefPtr<Gtk::TreeModel> reftm = m_listviewtextTracksChoosen->get_model();
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

void DialogNewPlaylist::on_toolbutton_TrackDown()
{
	Gtk::ListViewText::SelectionList sellst = m_listviewtextTracksChoosen->get_selected();
	if(sellst.empty()) return;
	int sel = sellst[0];
	if(sel == m_listviewtextTracksChoosen->size() - 1) return; // already at bottom //
	Glib::RefPtr<Gtk::TreeSelection> ts = m_listviewtextTracksChoosen->get_selection();
	Gtk::TreeModel::iterator sel_it = ts->get_selected();
	Gtk::TreeModel::iterator new_it = sel_it;
	++new_it;
	Glib::RefPtr<Gtk::TreeModel> reftm = m_listviewtextTracksChoosen->get_model();
	Glib::RefPtr<Gtk::ListStore> refLStore = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(reftm);
	if(refLStore){
		std::cout << __FILE__ << '[' << __LINE__ << "]  it's a Gtk::ListStore" << std::endl;
		refLStore->iter_swap(sel_it, new_it);
	}
}

std::vector<int> DialogNewPlaylist::get_tracks_chosen()
{
	guint n = m_listviewtextTracksChoosen->size();
	std::vector<int> result;
	for(guint i = 0; i < n; ++i){
		char *endptr;
		Glib::ustring tmp = m_listviewtextTracksChoosen->get_text(i);
		std::cout << __FILE__ << '[' << __LINE__ << "]  tmp == " << tmp << std::endl;
		int id = strtol(tmp.c_str(), &endptr, 10);
		std::cout << __FILE__ << '[' << __LINE__ << "]  id == " << id << std::endl;
		result.insert(result.end(), id);
		if(endptr && *endptr){
			std::cout << __FILE__ << '[' << __LINE__ << "]  Error: how can an id not be all number" << std::endl;
			// TODO this is an error never should happen //
		}
	}
	return result;
}







