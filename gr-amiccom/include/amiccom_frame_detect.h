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

#ifndef INCLUDED_AMICCOM_FRAME_DETECT_H
#define INCLUDED_AMICCOM_FRAME_DETECT_H

#include <amiccom_api.h>
#include <gr_sync_block.h>

class amiccom_frame_detect;
typedef boost::shared_ptr<amiccom_frame_detect> amiccom_frame_detect_sptr;

AMICCOM_API amiccom_frame_detect_sptr amiccom_make_frame_detect (int sync, int addr, int payload_size);

/*!
 * \brief <+description+>
 *
 */
class AMICCOM_API amiccom_frame_detect : public gr_sync_block
{
	friend AMICCOM_API amiccom_frame_detect_sptr amiccom_make_frame_detect (int sync, int addr, int payload_size);

	amiccom_frame_detect (int sync, int addr, int payload_size);

 public:
	~amiccom_frame_detect ();


	int work (int noutput_items,
		gr_vector_const_void_star &input_items,
		gr_vector_void_star &output_items);
};

#endif /* INCLUDED_AMICCOM_FRAME_DETECT_H */

