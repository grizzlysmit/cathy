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

#ifndef _MESAGE_H_
#define _MESAGE_H_
#include "basemesage.h"

template<typename ArgT>
class mesage: public basemesage 
{
	public:
		typedef ArgT arg_type;
		
		mesage(basemesage::message_type mt, ArgT arg);
		virtual ~mesage();

		ArgT get_arg();
	protected:
		ArgT _arg;

	private:
		
};


template<typename ArgT>
mesage<ArgT>::mesage(basemesage::message_type mt, ArgT arg)
  : basemesage(mt), _arg(arg)
{
}

template<typename ArgT>
mesage<ArgT>::~mesage()
{
}

template<typename ArgT>
typename mesage<ArgT>::arg_type mesage<ArgT>::get_arg()
{
	return _arg;
}

#endif // _MESAGE_H_

