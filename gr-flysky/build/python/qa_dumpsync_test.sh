#!/bin/sh
export GR_DONT_LOAD_PREFS=1
export srcdir=/shampoo/sdr/projects/gr-flysky/python
export PATH=/shampoo/sdr/projects/gr-flysky/build/python:$PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DYLD_LIBRARY_PATH
export DYLD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DYLD_LIBRARY_PATH
export PYTHONPATH=/shampoo/sdr/projects/gr-flysky/build/swig:$PYTHONPATH
/usr/bin/python2.6 /shampoo/sdr/projects/gr-flysky/python/qa_flysky_dumpsync.py 
