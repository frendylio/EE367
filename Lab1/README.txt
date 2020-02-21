README
=============================================================================
## Compile Instructions:

Run `make all` to compile client and server.

client is stored in ./Client while server is stored in ./

==============================================================================

## Run Instructions:

To run in same terminal window, append '&' when running server (e.g. ./server &)
This makes server run in the background.

Change to the Client/ directory before running client. Make sure you are
in the project home directory before running server.

Running client in the same directory as server may cause unintended
consequences. This is because file commands do not take into account
relative file paths; instead, file searching occurs in the directory
each program is executed from.

For example, I run the following:
$ ./server &
$ Client/client
$ download test.txt

Since I have executed both programs from the same directory, the following
happens:
1) server reads and sends ./test.txt to the client
2) client overwrites ./test.txt with data received from server (the contents
of ./test.txt)

This is redundant and may cause unintended results (e.g. a problem occurs during
data transmission--the original contents of ./test.txt have now been 
completely/partially lost
