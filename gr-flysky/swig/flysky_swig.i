/* -*- c++ -*- */

#define FLYSKY_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "flysky_swig_doc.i"


%{
#include "flysky_dumpsync.h"
#include "flysky_bind_code.h"
%}


GR_SWIG_BLOCK_MAGIC(flysky,dumpsync);
%include "flysky_dumpsync.h"

GR_SWIG_BLOCK_MAGIC(flysky,bind_code);
%include "flysky_bind_code.h"
