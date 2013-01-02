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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gr_io_signature.h>
#include <flysky_bind_code.h>

typedef unsigned char u8;
u8 bind_data[] = {0x55,0x55,0x55,0x55,0x54,0x75,0x52,0x2a,0xaa,0xfb,0x81,0x02,0x00,0xe3,0x05,0xe3,0x05,0xe3,0x05,0xe3,0x05,0xe3,0x05,0xe3,0x05,0xe3,0x05,0xe3,0x05}; 

flysky_bind_code_sptr
flysky_make_bind_code (int bind_code)
{
	return flysky_bind_code_sptr (new flysky_bind_code (bind_code));
}


flysky_bind_code::flysky_bind_code (int bind_code)
	: gr_sync_block ("bind_code",
		gr_make_io_signature (0, 0, 0),
		gr_make_io_signature (1, 1, sizeof (char)))
{
    int code = bind_code;
}


flysky_bind_code::~flysky_bind_code ()
{
}


int
flysky_bind_code::work (int noutput_items,
			gr_vector_const_void_star &input_items,
			gr_vector_void_star &output_items)
{
	unsigned char *out = (unsigned char*)  output_items[0];


	return noutput_items;
}

