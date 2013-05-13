/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * matrix-box.cc
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

#include "matrix-box.h"

MatrixBox::Row::Row()
  : num_columns(0), id(-1)
{
}

MatrixBox::Row::Row(guint num_cols)
  : num_columns(num_cols), id(-1)
{
}

MatrixBox::Row::Row(guint num_cols, const std::vector<Gtk::Label*>& r, int _id)
  : num_columns(num_cols), id(_id)
{
	g_return_if_fail( num_columns == r.size() );
	
	for(guint i = 0; i < num_columns; ++i){
		row.insert(row.end(), r[i]);
	}
}

void MatrixBox::Row::add(const std::vector<Gtk::Label*>& r, int _id)
{
	g_return_if_fail( num_columns == r.size() );
	
	for(guint i = 0; i < num_columns; ++i){
		row.insert(row.end(), r[i]);
	}
}

MatrixBox::Row::~Row()
{
}


MatrixBox::MatrixBox(guint num_cols)
  : num_columns(num_cols), currentid(-1)
{
    add(m_grid); // add the m_grid to the MatrixBox Gtk::EventBox dirived widget

	m_grid.set_row_homogeneous(true);
	//m_grid.set_column_homogeneous(true);
	m_grid.set_column_spacing(20);
	m_grid.set_row_spacing(5);
	//  set the background colour of the ListViewFormatText to white //
	std::string style_sheet = ".white_bg {background: #FFFFFF; color: #000000; } ";
	//style_sheet += ".green_bg {background: #33FF33; color: #000000; }";
	Glib::RefPtr<Gtk::StyleContext> stylecontext = get_style_context();
	Glib::RefPtr<Gtk::CssProvider> cssprov = Gtk::CssProvider::create();
	cssprov->load_from_data(style_sheet);
	stylecontext->add_provider(cssprov, GTK_STYLE_PROVIDER_PRIORITY_USER);
	stylecontext->add_class("white_bg");
	//stylecontext->add_class("green_bg");
	stylecontext->context_save();
}

MatrixBox::~MatrixBox()
{
	clear_items();
}

void  MatrixBox::set_headings(const std::vector<Glib::ustring>& heads)
{
	g_return_if_fail( num_columns == heads.size());
	
	guint j = 0;
	for(guint i = 0; i < num_columns; ++i){
		Gtk::Label *tmp = new Gtk::Label(heads[i], Gtk::ALIGN_START);
		tmp->set_use_markup();
		tmp->set_selectable();

		m_grid.attach(*tmp, i, j, 1, 1);
		m_grid.show_all();
	}
}

guint MatrixBox::append(const std::vector<Glib::ustring>& row, int _id){
	std::cout << __FILE__ << '[' << __LINE__ << "] row.size() == " << row.size() << std::endl;
	g_return_val_if_fail( num_columns == row.size(), row.size());

	std::vector<Gtk::Label*> _row;
	guint j = rows.size() + 1;
	Row *r = new Row(num_columns, /*hbox,*/ _row, _id);
	for(guint i = 0; i < num_columns; ++i){
		Gtk::Label *tmp = new Gtk::Label(row[i], Gtk::ALIGN_START);
		tmp->set_use_markup();
		tmp->set_selectable();
		//tmp->set_opacity(1);

		_row.insert(_row.begin(), tmp);
		tmp->set_events(Gdk::BUTTON_PRESS_MASK);
		tmp->signal_button_press_event().connect( sigc::bind<int, Row*>( sigc::mem_fun(*this, &MatrixBox::on_button_Pressed), _id, r), false );

		m_grid.attach(*tmp, i, j, 1, 1);
	}
	std::cout << __FILE__ << '[' << __LINE__ << "] _row.size() == " << _row.size() << std::endl;
	r->add(_row, _id);
	rows.insert(rows.end(), r);
	m_grid.show_all();
	return rows.size() - 1;
}


bool MatrixBox::on_button_Pressed(GdkEventButton* event, int _id, Row* row){
	std::cout << "entering on_button_Pressed" << std::endl;
	
    for(std::vector<Gtk::Label*>::iterator label(row->begin()), label_end(row->end()); label != label_end; ++label){
		(*label)->select_region(0);
	}
	if(event->type == GDK_2BUTTON_PRESS){ //double click //
		currentid = _id;
		m_signal_dblclicked.emit(currentid);
		std::cout << "double click: " << currentid << std::endl;
		return false;
	}else if(event->type == GDK_BUTTON_PRESS){ // single click //
		currentid = _id;
		m_signal_clicked.emit(currentid);
		std::cout << "single click: " << currentid << std::endl;
		std::vector<Gtk::Label*>::iterator label(row->end()), label_end(row->begin());
		do{
			--label;
			std::cout << (*label)->get_text() << "\t|\t" << std::flush;
		}while(label != label_end);
		std::cout << std::endl;
		return true;
	}else if(event->type == GDK_3BUTTON_PRESS){ // single click //
		currentid = _id;
		std::cout << "triple click: " << currentid << std::endl;
		return false;
	}else if(event->type == GDK_BUTTON_RELEASE){
		currentid = _id;
		std::cout << "button release: " << currentid << std::endl;
		return false;
	}
	return false;
}


void MatrixBox::clear_items()
{
	Gtk::Label *tmp;
	for(std::vector<Row*>::iterator i(rows.begin()), i_end(rows.end()); i != i_end; ++i){
		//Gtk::Box *hbox = i->get_hbox();
		for(std::vector<Gtk::Label*>::iterator j((*i)->begin()), j_end((*i)->end()); j != j_end; ++j){
			tmp = *j;
			m_grid.remove(*tmp);
			delete tmp;
		}
		Row *r = *i;
		delete r;
		//rows.erase(i);
	}
	rows.clear();
}

std::vector<Gtk::Label*>::iterator MatrixBox::Row::begin()
{
	return row.begin();
}

std::vector<Gtk::Label*>::iterator MatrixBox::Row::end()
{
	return row.end();
}


MatrixBox::type_signal_clicked MatrixBox::signal_clicked()
{
  return m_signal_clicked;
}

MatrixBox::type_signal_clicked MatrixBox::signal_dblclicked()
{
  return m_signal_dblclicked;
}





