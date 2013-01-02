gnuradio-rc-testcode
====================
These are some gnuradio modules I wrote while experimenting with
2.4ghz ISM band remote controllers. The specific controllers used
were a "Turnigy 9X" and a WLToys V911 remote. Both of these 
implementing the flysky protocol.


status
======
I have not touched these for 6 months, committing
as they were when I left them. Consider this to be
pre-alpha test code.


http://www.rcgroups.com/forums/showthread.php?t=1626362
http://www.rcgroups.com/forums/showthread.php?t=1673507


todo
===
There was some additional misplaced code that is back on the todo list

* python code turn the received data into a joystick using linux uinput . This replaced a $5 usb/ppm adapter with a $2000 USRP.
* tx code, this was written before the hopping sequence was known, so I just transmitted on every channel within the USRP's bandwidth. This didn't not work.


credits
=======
Based completely on gnuradio and gr-clicker

http://gnuradio.org
http://sourceforge.net/projects/gr-clicker/


license
=====
Everything here should be GPLv3

