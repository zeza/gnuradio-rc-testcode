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
#include <amiccom_frame_detect.h>


amiccom_frame_detect_sptr
amiccom_make_frame_detect (int sync, int addr, int payload_size)
{
	return amiccom_frame_detect_sptr (new amiccom_frame_detect (sync, addr, payload_size));
}


amiccom_frame_detect::amiccom_frame_detect (int sync, int addr, int payload_size)
	: gr_sync_block ("frame_detect",
		gr_make_io_signature (1, 1, sizeof (char)),
		gr_make_io_signature (1, 1, sizeof (char)))
{
}


amiccom_frame_detect::~amiccom_frame_detect ()
{
}


int
amiccom_frame_detect::work (int noutput_items,
			gr_vector_const_void_star &input_items,
			gr_vector_void_star &output_items)
{
	const float *in = (const float *) input_items[0];
	float *out = (float *) output_items[0];

	// Do <+signal processing+>

	// Tell runtime system how many output items we produced.
	return noutput_items;
}

