/* -*- c++ -*- */
/* 
 * Copyright 2012 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_FLYSKY_BIND_CODE_H
#define INCLUDED_FLYSKY_BIND_CODE_H

#include <flysky_api.h>
#include <gr_sync_block.h>

class flysky_bind_code;
typedef boost::shared_ptr<flysky_bind_code> flysky_bind_code_sptr;

FLYSKY_API flysky_bind_code_sptr flysky_make_bind_code (int bind_code);

/*!
 * \brief <+description+>
 *
 */
class FLYSKY_API flysky_bind_code : public gr_sync_block
{
	friend FLYSKY_API flysky_bind_code_sptr flysky_make_bind_code (int bind_code);

	flysky_bind_code (int bind_code);

 public:
	~flysky_bind_code ();


	int work (int noutput_items,
		gr_vector_const_void_star &input_items,
		gr_vector_void_star &output_items);
};

#endif /* INCLUDED_FLYSKY_BIND_CODE_H */

