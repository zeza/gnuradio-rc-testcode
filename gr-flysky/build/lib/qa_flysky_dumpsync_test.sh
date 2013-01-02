#!/bin/sh
export GR_DONT_LOAD_PREFS=1
export srcdir=/shampoo/sdr/projects/gr-flysky/lib
export PATH=/shampoo/sdr/projects/gr-flysky/build/lib:$PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DYLD_LIBRARY_PATH
export DYLD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DYLD_LIBRARY_PATH
export PYTHONPATH=$PYTHONPATH
qa_flysky_dumpsync 
