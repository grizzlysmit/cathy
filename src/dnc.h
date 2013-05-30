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

#ifndef _DNC_H_
#define _DNC_H_
#include <gtkmm.h>
#include <gtkmm/filefilter.h>
#include <xmmsclient/xmmsclient++.h>
#include <xmmsclient/xmmsclient++/list.h>
#include <xmmsclient/xmmsclient++/coll.h>

#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <vector>
#include "lvt.h"

class DNC : public Gtk::Dialog 
{
	public:
		DNC(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder);
		~DNC();

		void set_big_hint(bool big_hint = true);
		bool get_big_hint();
	protected:
		const Glib::RefPtr<Gtk::Builder>& m_builder;

		Gtk::ScrolledWindow *m_scrolledwindowAvailbleKeys;
		LVT *m_listviewtextAvailbleKeys;
		Gtk::ScrolledWindow *m_scrolledwindowHelp;
		LVT *m_listviewtextHelp;

		bool m_big_hint;

		void on_selection_changed_AvailableKeys();
		void on_toolbutton_AddKey();
		void set_help_hint();
	private:
		
};

#endif // _DNC_H_

