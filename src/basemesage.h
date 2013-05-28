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

#ifndef _BASEMESAGE_H_
#define _BASEMESAGE_H_

class basemesage
{
	public:
		enum message_type { status, disconnect, connect, 
			plchange, update_pos, pls_loaded, list, current_id,
		    handle_volume, handle_playtime, handle_medialib_info,
		    handle_mlib_entry_changed};
		
		basemesage(message_type mt);
		virtual ~basemesage();

		message_type get_message_type();
	protected:
		message_type _mt;

	private:

};



#endif // _BASEMESAGE_H_

