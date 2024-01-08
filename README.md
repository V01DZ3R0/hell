# hell
* Tiny HTTP Server on C, using only standard libraries.  
* Has it's own tiny (and cute) CLI.
* On clean exit does not occupy any extra sockets/ports/processes.
## some details
You can visit homepage (which is store in site.html) on http://localhost:1666/home by default. Server itself is on child-process and interface is on parent-process. Restart function is useful for changing homepage on-a-run. 
## alzo
Uses daemon to serve, hell, demon, you got it. I wanted to use 666 port, but is restrcted so executing required sudo. It is named "hell", because it is hello_world-level server but without o_world. "xd" fucntion was made for testing only, but maybe it rly does smth, who knows...
