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
    m_scrolledwindowAvailbleKeys(0), m_listviewtextAvailbleKeys(0), 
    m_scrolledwindowHelp(0), m_listviewtextHelp(0), m_big_hint(false)
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
		m_listviewtextAvailbleKeys->set_text(row, 1, "string");
		row = m_listviewtextAvailbleKeys->append("tracknr");
		m_listviewtextAvailbleKeys->set_text(row, 1, "string");
		row = m_listviewtextAvailbleKeys->append("artist");
		m_listviewtextAvailbleKeys->set_text(row, 1, "string");
		row = m_listviewtextAvailbleKeys->append("mime");
		m_listviewtextAvailbleKeys->set_text(row, 1, "string");
		row = m_listviewtextAvailbleKeys->append("AND");
		m_listviewtextAvailbleKeys->set_text(row, 1, "Operator");
		row = m_listviewtextAvailbleKeys->append("OR");
		m_listviewtextAvailbleKeys->set_text(row, 1, "Operator");
		row = m_listviewtextAvailbleKeys->append("NOT");
		m_listviewtextAvailbleKeys->set_text(row, 1, "Operator");
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
		m_listviewtextHelp->append("PATTERN");
		m_listviewtextHelp->append("       Patterns are used to search for songs in the media library, some of these patterns may require escaping  (with  '\\')  depending  on  what");
		m_listviewtextHelp->append("       shell is used. The properties can be found in the output of xmms2 info.");
		m_listviewtextHelp->append("");
		m_listviewtextHelp->append("       <property>:<string>");
		m_listviewtextHelp->append("");
		m_listviewtextHelp->append("           Match  songs  whose  property matches the string. A ? in the string indicates a single wildcard character, and a * indicates multiple");
		m_listviewtextHelp->append("           wildcard characters.");
		m_listviewtextHelp->append("");
		m_listviewtextHelp->append("       <property>~<string>");
		m_listviewtextHelp->append("");
		m_listviewtextHelp->append("           Match songs whose property fuzzily matches the string. Equal to matching by <property>:*<string>*.");
		m_listviewtextHelp->append("");
		m_listviewtextHelp->append("       <property><operation><number>");
		m_listviewtextHelp->append("");
		m_listviewtextHelp->append("           The operation can be <, <=, > or >=, the pattern will match songs whose property is a numerical value smaller, smaller or equal, big‐");
		m_listviewtextHelp->append("           ger, bigger or equal in comparison to the number.");
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
}

DNC::~DNC()
{
}

void DNC::on_selection_changed_AvailableKeys()
{
}

void DNC::on_toolbutton_AddKey()
{
}

void DNC::set_help_hint()
{
	if(m_big_hint){
		m_listviewtextHelp->set_tooltip_markup("<b>PATTERN</b>\n"
		                                       "       Patterns are used to search for songs in the media library, some of these patterns may require escaping  (with  '\')  depending  on  what\n"
		                                       "       shell is used. The properties can be found in the output of xmms2 info.\n"
		                                       "\n"
		                                       "       &lt;property&gt;:&lt;string&gt;\n"
		                                       "\n"
		                                       "           Match  songs  whose  property matches the string. A ? in the string indicates a single wildcard character, and a * indicates multiple\n"
		                                       "           wildcard characters.\n"
		                                       "\n"
		                                       "       &lt;property&gt;~&lt;string&gt;\n"
		                                       "\n"
		                                       "           Match songs whose property fuzzily matches the string. Equal to matching by &lt;property&gt;:*&lt;string&gt;*.\n"
		                                       "\n"
		                                       "       &lt;property&gt;&lt;operation&gt;&lt;number&gt;\n"
		                                       "\n"
		                                       "           The operation can be <b>&lt;, &lt;=, &gt;</b> or <b>&gt;=</b>, the pattern will match songs whose property is a numerical value smaller, smaller or equal, big‐\n"
		                                       "           ger, bigger or equal in comparison to the number.\n"
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





