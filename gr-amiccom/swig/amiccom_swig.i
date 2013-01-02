/* -*- c++ -*- */

#define AMICCOM_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "amiccom_swig_doc.i"


%{
#include "amiccom_frame_detect.h"
%}


GR_SWIG_BLOCK_MAGIC(amiccom,frame_detect);
%include "amiccom_frame_detect.h"
