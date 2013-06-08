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

#include "dnc.h"

DNC::DNC(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder)
  : Gtk::Dialog(cobject), m_builder(builder), 
    m_scrolledwindowAvailbleKeys(nullptr), m_listviewtextAvailbleKeys(nullptr), 
    m_scrolledwindowHelp(nullptr), m_listviewtextHelp(nullptr), m_entryCollectionName(nullptr), 
    m_entryPattern(nullptr), m_buttonOK(nullptr), m_dialogCollectionHelp(nullptr), 
    m_labelNewCollHelpPage0(nullptr), m_labelNewCollHelpPage1(nullptr), 
    m_toolbuttonHelp(nullptr), m_toolbuttonAddKey(nullptr), m_toolbuttonSpace(nullptr), 
    m_toolbuttonBrackets(nullptr), m_toolbuttonAND(nullptr), m_toolbuttonOR(nullptr), 
    m_toolbuttonNot(nullptr), m_toolbuttonHash(nullptr), m_toolbuttonMinus(nullptr), 
    m_toolbuttonPlus(nullptr), m_toolbuttonMatch(nullptr), m_toolbuttonFuzzyMatch(nullptr), 
    m_toolbuttonLT(nullptr), m_toolbuttonLE(nullptr), m_toolbuttonGT(nullptr), m_toolbuttonGE(nullptr), 
    m_toolbuttonGetKeys(nullptr), m_action_area(nullptr), 
    m_big_hint(false)
{
	// dialogNewCollection-scrolledwindowAvailbleKeys //
	m_builder->get_widget("dialogNewCollection-scrolledwindowAvailbleKeys", m_scrolledwindowAvailbleKeys);
	if(m_scrolledwindowAvailbleKeys){
		m_listviewtextAvailbleKeys = new LVT(2);
		m_scrolledwindowAvailbleKeys->add(*m_listviewtextAvailbleKeys);
		m_listviewtextAvailbleKeys->show();
		m_listviewtextAvailbleKeys->set_hexpand();
		m_listviewtextAvailbleKeys->set_vexpand();
		m_listviewtextAvailbleKeys->set_column_title(0, "Available Keys");
		m_listviewtextAvailbleKeys->set_column_title(1, "type");
		int row = m_listviewtextAvailbleKeys->append("title");
		m_listviewtextAvailbleKeys->set_text(row, 1, "string");
		row = m_listviewtextAvailbleKeys->append("album");
		m_listviewtextAvailbleKeys->set_text(row, 1, "string");
		row = m_listviewtextAvailbleKeys->append("id");
		m_listviewtextAvailbleKeys->set_text(row, 1, "integer");
		row = m_listviewtextAvailbleKeys->append("tracknr");
		m_listviewtextAvailbleKeys->set_text(row, 1, "integer");
		row = m_listviewtextAvailbleKeys->append("artist");
		m_listviewtextAvailbleKeys->set_text(row, 1, "string");
		row = m_listviewtextAvailbleKeys->append("mime");
		m_listviewtextAvailbleKeys->set_text(row, 1, "string");
/*		row = m_listviewtextAvailbleKeys->append("AND");
		m_listviewtextAvailbleKeys->set_text(row, 1, "Operator");
		row = m_listviewtextAvailbleKeys->append("OR");
		m_listviewtextAvailbleKeys->set_text(row, 1, "Operator");
		row = m_listviewtextAvailbleKeys->append("NOT");
		m_listviewtextAvailbleKeys->set_text(row, 1, "Operator");*/
		m_listviewtextAvailbleKeys->signal_selection_changed().connect( sigc::mem_fun(*this, &DNC::on_selection_changed_AvailableKeys) );
		m_listviewtextAvailbleKeys->signal_dblclicked().connect( sigc::mem_fun(*this, &DNC::on_toolbutton_AddKey) );
	}
	// dialogNewCollection-scrolledwindowHelp //
	m_builder->get_widget("dialogNewCollection-scrolledwindowHelp", m_scrolledwindowHelp);
	if(m_scrolledwindowHelp){
		m_listviewtextHelp = new LVT(1);
		m_scrolledwindowHelp->add(*m_listviewtextHelp);
		m_listviewtextHelp->show();
		//m_listviewtextHelp->set_hexpand();
		//m_listviewtextHelp->set_vexpand();
		m_listviewtextHelp->set_column_title(0, "Help ON Patterns");
		m_listviewtextHelp->append("PATTERN SYNTAX:");
		m_listviewtextHelp->append("       Patterns are used to search for songs in the media library, the properties ");
		m_listviewtextHelp->append("       can be found in the output of xmms2 info, or in the list box on the right.");
		m_listviewtextHelp->append("");
		m_listviewtextHelp->append("       <property>:<string>");
		m_listviewtextHelp->append("");
		m_listviewtextHelp->append("           Match  songs  whose  property matches the string. A ? in the string indicates");
		m_listviewtextHelp->append("           a single wildcard character, and a * indicates multiple wildcard characters.");
		m_listviewtextHelp->append("");
		m_listviewtextHelp->append("       <property>~<string>");
		m_listviewtextHelp->append("");
		m_listviewtextHelp->append("           Match songs whose property fuzzily matches the string. Equal to matching by ");
		m_listviewtextHelp->append("           <property>:*<string>*.");
		m_listviewtextHelp->append("");
		m_listviewtextHelp->append("       <property><operation><number>");
		m_listviewtextHelp->append("");
		m_listviewtextHelp->append("           The operation can be <, <=, > or >=, the pattern will match songs whose property");
		m_listviewtextHelp->append("           is a numerical value smaller, smaller or equal, bigger, bigger or equal in ");
		m_listviewtextHelp->append("           comparison to the number.");
		m_listviewtextHelp->append("");
		m_listviewtextHelp->append("       +<property>");
		m_listviewtextHelp->append("");
		m_listviewtextHelp->append("           Match songs which has the property.");
		m_listviewtextHelp->append("");
		m_listviewtextHelp->append("       NOT <pattern>");
		m_listviewtextHelp->append("");
		m_listviewtextHelp->append("           Match the complement of the pattern.");
		m_listviewtextHelp->append("");
		m_listviewtextHelp->append("       <pattern> <pattern>");
		m_listviewtextHelp->append("              <pattern> AND <pattern>");
		m_listviewtextHelp->append("");
		m_listviewtextHelp->append("           Match songs that are matched by both patterns.");
		m_listviewtextHelp->append("");
		m_listviewtextHelp->append("       <pattern> OR <pattern>");
		m_listviewtextHelp->append("");
		m_listviewtextHelp->append("           Match songs that are matched by at least one of the two pattern.");
		m_listviewtextHelp->append("");
		m_listviewtextHelp->append("       ( <pattern> )");
		m_listviewtextHelp->append("");
		m_listviewtextHelp->append("           Match songs matched by the pattern, used for grouping AND and OR matches.");
		m_listviewtextHelp->append("");
		m_listviewtextHelp->append("       <string>");
		m_listviewtextHelp->append("");
		m_listviewtextHelp->append("           Match songs whose artist, title or album match the string.");
		m_listviewtextHelp->append("");
		m_listviewtextHelp->append("       #<number>");
		m_listviewtextHelp->append("");
		m_listviewtextHelp->append("           Match a specific media library id.");
		m_listviewtextHelp->append("");
		m_listviewtextHelp->append("");
		set_help_hint();
		//m_listviewtextHelp->signal_selection_changed().connect( sigc::mem_fun(*this, &DNC::on_selection_changed_AvailableKeys) );
		//m_listviewtextHelp->signal_dblclicked().connect( sigc::mem_fun(*this, &DNC::on_toolbutton_AddKey) );
	}
	// m_entryCollectionName //
	m_builder->get_widget("dialogNewCollection-entryCollectionName", m_entryCollectionName);
	if(m_entryCollectionName){
		m_entryCollectionName->signal_insert_text().connect( sigc::mem_fun(*this, &DNC::on_entryCollectionName_changed) );
		//m_entryCollectionName->signal_dblclicked().connect( sigc::mem_fun(*this, &DNC::on_toolbutton_AddKey) );
	}
	// m_entryPattern //
	m_builder->get_widget("dialogNewCollection-entryPattern", m_entryPattern);
	if(m_entryPattern){
		//m_entryPattern->signal_selection_changed().connect( sigc::mem_fun(*this, &DNC::on_selection_changed_AvailableKeys) );
		//m_entryPattern->signal_dblclicked().connect( sigc::mem_fun(*this, &DNC::on_toolbutton_AddKey) );
	}
	// m_buttonOK //
	m_builder->get_widget("dialogNewCollection-buttonOK", m_buttonOK);
	if(m_buttonOK){
		//m_entryPattern->signal_selection_changed().connect( sigc::mem_fun(*this, &DNC::on_selection_changed_AvailableKeys) );
		//m_entryPattern->signal_dblclicked().connect( sigc::mem_fun(*this, &DNC::on_toolbutton_AddKey) );
	}

	// m_dialogCollectionHelp //
	m_builder->get_widget("dialogCollectionHelp", m_dialogCollectionHelp);
	if(m_dialogCollectionHelp){
		//m_dialogCollectionHelp->signal_selection_changed().connect( sigc::mem_fun(*this, &DNC::on_selection_changed_AvailableKeys) );
		//m_dialogCollectionHelp->signal_dblclicked().connect( sigc::mem_fun(*this, &DNC::on_toolbutton_AddKey) );
	}
	// m_labelNewCollHelpPage0 //
	m_builder->get_widget("labelNewCollHelpPage0", m_labelNewCollHelpPage0);
	if(m_labelNewCollHelpPage0){
		//m_labelNewCollHelpPage0->signal_selection_changed().connect( sigc::mem_fun(*this, &DNC::on_selection_changed_AvailableKeys) );
		//m_labelNewCollHelpPage0->signal_dblclicked().connect( sigc::mem_fun(*this, &DNC::on_toolbutton_AddKey) );
		m_labelNewCollHelpPage0->set_angle(90);
	}
	// m_labelNewCollHelpPage1 //
	m_builder->get_widget("labelNewCollHelpPage1", m_labelNewCollHelpPage1);
	if(m_labelNewCollHelpPage1){
		//m_labelNewCollHelpPage1->signal_selection_changed().connect( sigc::mem_fun(*this, &DNC::on_selection_changed_AvailableKeys) );
		//m_labelNewCollHelpPage1->signal_dblclicked().connect( sigc::mem_fun(*this, &DNC::on_toolbutton_AddKey) );
		m_labelNewCollHelpPage1->set_angle(90);
	}
	// m_toolbuttonHelp //
	m_builder->get_widget("dialogNewCollection-toolbuttonHelp", m_toolbuttonHelp);
	if(m_toolbuttonHelp){
		m_toolbuttonHelp->signal_clicked().connect( sigc::mem_fun(*this, &DNC::on_m_toolbuttonHelp_clicked) );
	}
	// m_toolbuttonAddKey //
	m_builder->get_widget("dialogNewCollection-toolbuttonAddKey", m_toolbuttonAddKey);
	if(m_toolbuttonAddKey){
		m_toolbuttonAddKey->signal_clicked().connect( sigc::mem_fun(*this, &DNC::on_toolbutton_AddKey) );
	}
	// m_toolbuttonSpace //
	m_builder->get_widget("toolbuttonSpace", m_toolbuttonSpace);
	if(m_toolbuttonSpace){
		m_toolbuttonSpace->signal_clicked().connect( sigc::bind<Glib::ustring>( sigc::mem_fun(*this, &DNC::on_toolbutton_x_clicked), " ") ); // passing a space char //
	}
	// m_toolbuttonBrackets //
	m_builder->get_widget("toolbuttonBrackets", m_toolbuttonBrackets);
	if(m_toolbuttonBrackets){
		m_toolbuttonBrackets->signal_clicked().connect( sigc::mem_fun(*this, &DNC::on_toolbutton_Brackets) );
	}
	// m_toolbuttonAND //
	m_builder->get_widget("toolbuttonAND", m_toolbuttonAND);
	if(m_toolbuttonAND){
		m_toolbuttonAND->signal_clicked().connect( sigc::bind<Glib::ustring>( sigc::mem_fun(*this, &DNC::on_toolbutton_x_clicked), "AND") );
	}
	// m_toolbuttonOR //
	m_builder->get_widget("toolbuttonOR", m_toolbuttonOR);
	if(m_toolbuttonOR){
		m_toolbuttonOR->signal_clicked().connect( sigc::bind<Glib::ustring>( sigc::mem_fun(*this, &DNC::on_toolbutton_x_clicked), "OR") );
	}
	// m_toolbuttonNot //
	m_builder->get_widget("toolbuttonNot", m_toolbuttonNot);
	if(m_toolbuttonNot){
		m_toolbuttonNot->signal_clicked().connect( sigc::bind<Glib::ustring>( sigc::mem_fun(*this, &DNC::on_toolbutton_x_clicked), "NOT") );
	}
	// m_toolbuttonHash //
	m_builder->get_widget("toolbuttonHash", m_toolbuttonHash);
	if(m_toolbuttonHash){
		m_toolbuttonHash->signal_clicked().connect( sigc::bind<Glib::ustring>( sigc::mem_fun(*this, &DNC::on_toolbutton_x_clicked), "#") );
	}
	// m_toolbuttonMinus //
	m_builder->get_widget("toolbuttonMinus", m_toolbuttonMinus);
	if(m_toolbuttonMinus){
		m_toolbuttonMinus->signal_clicked().connect( sigc::bind<Glib::ustring>( sigc::mem_fun(*this, &DNC::on_toolbutton_x_clicked), "-") );
	}
	// m_toolbuttonPlus //
	m_builder->get_widget("toolbuttonPlus", m_toolbuttonPlus);
	if(m_toolbuttonPlus){
		m_toolbuttonPlus->signal_clicked().connect( sigc::bind<Glib::ustring>( sigc::mem_fun(*this, &DNC::on_toolbutton_x_clicked), "+") );
	}
	// m_toolbuttonMatch //
	m_builder->get_widget("toolbuttonMatch", m_toolbuttonMatch);
	if(m_toolbuttonMatch){
		m_toolbuttonMatch->signal_clicked().connect( sigc::bind<Glib::ustring>( sigc::mem_fun(*this, &DNC::on_toolbutton_x_clicked), ":") );
	}
	// m_toolbuttonFuzzyMatch //
	m_builder->get_widget("toolbuttonFuzzyMatch", m_toolbuttonFuzzyMatch);
	if(m_toolbuttonFuzzyMatch){
		m_toolbuttonFuzzyMatch->signal_clicked().connect( sigc::bind<Glib::ustring>( sigc::mem_fun(*this, &DNC::on_toolbutton_x_clicked), "~") );
	}
	// m_toolbuttonLT //
	m_builder->get_widget("toolbuttonLT", m_toolbuttonLT);
	if(m_toolbuttonLT){
		m_toolbuttonLT->signal_clicked().connect( sigc::bind<Glib::ustring>( sigc::mem_fun(*this, &DNC::on_toolbutton_x_clicked), "<") );
	}
	// m_toolbuttonLE //
	m_builder->get_widget("toolbuttonLE", m_toolbuttonLE);
	if(m_toolbuttonLE){
		m_toolbuttonLE->signal_clicked().connect( sigc::bind<Glib::ustring>( sigc::mem_fun(*this, &DNC::on_toolbutton_x_clicked), "<=") );
	}
	// m_toolbuttonGT //
	m_builder->get_widget("toolbuttonGT", m_toolbuttonGT);
	if(m_toolbuttonGT){
		m_toolbuttonGT->signal_clicked().connect( sigc::bind<Glib::ustring>( sigc::mem_fun(*this, &DNC::on_toolbutton_x_clicked), ">") );
	}
	// m_toolbuttonGE //
	m_builder->get_widget("toolbuttonGE", m_toolbuttonGE);
	if(m_toolbuttonGE){
		m_toolbuttonGE->signal_clicked().connect( sigc::bind<Glib::ustring>( sigc::mem_fun(*this, &DNC::on_toolbutton_x_clicked), ">=") );
	}
	// m_toolbuttonGetKeys //
	m_builder->get_widget("toolbuttonGetKeys", m_toolbuttonGetKeys);
	if(m_toolbuttonGetKeys){
		m_toolbuttonGetKeys->signal_clicked().connect( sigc::mem_fun(*this, &DNC::on_toolbutton_GetKeys) );
	}
	
	// m_action_area //
	m_builder->get_widget("dialognewcolection-action_area", m_action_area);
	if(m_action_area){
		//m_action_area->signal_insert_text().connect( sigc::mem_fun(*this, &DR::on_entry_RenameNewName_changed) );
		m_action_area->set_layout(Gtk::BUTTONBOX_EDGE);
	}

	set_default_size(640, 480);
}

DNC::~DNC()
{
	delete m_listviewtextAvailbleKeys;
	delete m_listviewtextHelp;
}

void DNC::on_selection_changed_AvailableKeys()
{
}

void DNC::on_toolbutton_AddKey()
{
	Gtk::ListViewText::SelectionList sellst = m_listviewtextAvailbleKeys->get_selected();
	if(sellst.empty()) return;
	//Glib::RefPtr<Gtk::TreeSelection> ts = m_listviewtextAvailbleKeys->get_selection();
	//Gtk::TreeModel::iterator sel_it = ts->get_selected();
	int sel = sellst[0];
	Glib::ustring selrow = m_listviewtextAvailbleKeys->get_text(sel);
	int start_pos, end_pos;
	Glib::ustring text = m_entryPattern->get_text(), tmp;
	int new_pos;
	if(m_entryPattern->get_selection_bounds(start_pos, end_pos)){
		tmp = text.substr(0, start_pos);
		tmp += selrow;
		new_pos = tmp.length();
		tmp += text.substr(end_pos);
		m_entryPattern->set_text(tmp);
	}else{
		int cursor = m_entryPattern->get_position();
		tmp = text.substr(0, cursor);
		tmp += selrow;
		new_pos = tmp.length();
		tmp += text.substr(cursor);
		m_entryPattern->set_text(tmp);
	}
	if(!m_entryPattern->is_focus()){
		m_entryPattern->grab_focus();
	}
	m_entryPattern->set_position(new_pos);
}

void DNC::on_toolbutton_x_clicked(Glib::ustring ins)
{
	int start_pos, end_pos;
	Glib::ustring text = m_entryPattern->get_text(), tmp;
	int new_pos;
	if(m_entryPattern->get_selection_bounds(start_pos, end_pos)){
		tmp = text.substr(0, start_pos);
		tmp += ins;
		new_pos = tmp.length();
		tmp += text.substr(end_pos);
		m_entryPattern->set_text(tmp);
	}else{
		int cursor = m_entryPattern->get_position();
		tmp = text.substr(0, cursor);
		tmp += ins;
		new_pos = tmp.length();
		tmp += text.substr(cursor);
		m_entryPattern->set_text(tmp);
	}
	if(!m_entryPattern->is_focus()){
		m_entryPattern->grab_focus();
	}
	m_entryPattern->set_position(new_pos);
}

void DNC::on_toolbutton_Brackets()
{
	int start_pos, end_pos;
	Glib::ustring text = m_entryPattern->get_text(), tmp;
	int new_pos;
	if(m_entryPattern->get_selection_bounds(start_pos, end_pos)){
		tmp = text.substr(0, start_pos);
		tmp += "(" + text.substr(start_pos, end_pos) + ")";
		new_pos = tmp.length();
		tmp += text.substr(end_pos);
		m_entryPattern->set_text(tmp);
	}else{
		int cursor = m_entryPattern->get_position();
		tmp = text.substr(0, cursor);
		tmp += "()";
		new_pos = tmp.length();
		tmp += text.substr(cursor);
		m_entryPattern->set_text(tmp);
	}
	if(!m_entryPattern->is_focus()){
		m_entryPattern->grab_focus();
	}
	m_entryPattern->set_position(new_pos);
}

void DNC::set_help_hint()
{
	if(m_big_hint){
		m_listviewtextHelp->set_tooltip_markup("<b>PATTERN SYNTAX:</b>\n"
		                                       "       Patterns are used to search for songs in the media library, the properties\n"
		                                       "       can be found in the output of xmms2 info, or in the list box ooe right.\n\n"
		                                       "       &lt;property&gt;:&lt;string&gt;\n"
		                                       "\n"
		                                       "           Match  songs  whose  property matches the string. A ? in the string\n"
		                                       "           indicates a single wildcard character, and a * indicates multiple wildcard \n"
		                                       "           characters.\n\n"
		                                       "       &lt;property&gt;~&lt;string&gt;\n"
		                                       "\n"
		                                       "           Match songs whose property fuzzily matches the string. Equal to matching by\n"
		                                       "           &lt;property&gt;:*&lt;string&gt;*.\n\n"
		                                       "       &lt;property&gt;&lt;operation&gt;&lt;number&gt;\n"
		                                       "\n"
		                                       "           The operation can be <b>&lt;, &lt;=, &gt;</b> or <b>&gt;=</b>, the pattern\n"
		                                       "           will match songs whose property is a numerical value smaller, smaller or equal,\n"
		                                       "           bigger, bigger or equal in comparison to the number.\n"
		                                       "\n"
		                                       "       +&lt;property&gt;\n"
		                                       "\n"
		                                       "           Match songs which has the property.\n"
		                                       "\n"
		                                       "       <b>NOT</b> &lt;pattern&gt;\n"
		                                       "\n"
		                                       "           Match the complement of the pattern.\n"
		                                       "\n"
		                                       "       &lt;pattern&gt; &lt;pattern&gt;\n"
		                                       "              &lt;pattern&gt; <b>AND</b> &lt;pattern&gt;\n"
		                                       "\n"
		                                       "           Match songs that are matched by both patterns.\n"
		                                       "\n"
		                                       "       &lt;pattern&gt; <b>OR</b> &lt;pattern&gt;\n"
		                                       "\n"
		                                       "           Match songs that are matched by at least one of the two pattern.\n"
		                                       "\n"
		                                       "       ( &lt;pattern&gt; )\n"
		                                       "\n"
		                                       "           Match songs matched by the pattern, used for grouping AND and OR matches.\n"
		                                       "\n"
		                                       "       &lt;string&gt;\n"
		                                       "\n"
		                                       "           Match songs whose artist, title or album match the string.\n"
		                                       "\n"
		                                       "       #&lt;number&gt;\n"
		                                       "\n"
		                                       "           Match a specific media library id.\n"
		                                       "\n"
		                                       "");
	}else{ // m_big_hint false
		m_listviewtextHelp->set_tooltip_markup("<b>Pattern Help</b>\n<i>for more Help click <b>Help</b></i>");
	}
}

void DNC::set_big_hint(bool big_hint)
{
	m_big_hint = big_hint;
	set_help_hint();
}

bool DNC::get_big_hint()
{
	return m_big_hint;
}

Glib::ustring DNC::get_collectionname()
{
	return m_entryCollectionName->get_text();
}

Glib::ustring DNC::get_pattern()
{
	return m_entryPattern->get_text();
}

void DNC::on_entryCollectionName_changed(const Glib::ustring& s,int *i)
{
	std::cout << __FILE__ << '[' << __LINE__ << "] " << __PRETTY_FUNCTION__ << " s == " << s << std::endl;
	Glib::ustring text = m_entryCollectionName->get_text();
	if(valid_collection_name(text)){
		m_buttonOK->set_sensitive(true);
	}else{
		m_buttonOK->set_sensitive(false);
	}
}

bool DNC::valid_collection_name(Glib::ustring name)
{
	if(name.empty()) return false;
	Glib::ustring invalid(" \n\t\r");
	size_t pos = name.find_first_of(invalid);
	return (pos == Glib::ustring::npos);
}

void DNC::on_m_toolbuttonHelp_clicked()
{
	int res = m_dialogCollectionHelp->run();
	m_dialogCollectionHelp->hide();
}

void DNC::on_toolbutton_GetKeys()
{
	m_listviewtextAvailbleKeys->remove_all_rows();
	std::vector<std::pair<Glib::ustring, Glib::ustring> > lst = m_signal_getkeys.emit();
	for(std::vector<std::pair<Glib::ustring, Glib::ustring> >::iterator i = lst.begin(), i_end = lst.end(); i != i_end; ++i){
		int row = m_listviewtextAvailbleKeys->append(i->first);
		m_listviewtextAvailbleKeys->set_text(row, 1, i->second);
	}
}

DNC::type_signal_getkeys DNC::signal_getkeys()
{
	return m_signal_getkeys;
}



