# Project 5: Socket Programming

As discussed in lecture, socket programming is the standard way to write programs that communicate over a network. While originally developed for Unix computers programmed in C, the socket abstraction is general and not tied to any specific operating system or programming language. This allows programmers to use the socket mental model to write correct network programs in many contexts.

This project will give you experience with basic socket programming.  You will write 2 pairs of TCP client and server programs for sending and receiving text messages over the Internet. One client/server pair will be written in C, the other in Python. The Python version will be significantly shorter.

The client and server programs in both languages should meet the following specifications. Be sure to read these before and after programming to make sure your implementation fulfills them:

##### Server specification
* Each server program should listen on a socket, wait for a client to connect, receive a message from the client, print the message to stdout, and then wait for the next client indefinitely.
* Each server should take one command-line argument: the port number to listen on for client connections.
* Each server should accept and process client communications in an infinite loop, allowing multiple clients to send messages to the same server. The server should only exit in response to an external signal (e.g. SIGINT from pressing `ctrl-c`).
* Each server should maintain a short (10) client queue and handle multiple client connection attempts sequentially. In real applications, a TCP server would fork a new process to handle each client connection concurrently, but that is not necessary for this assignment.
* Each server should gracefully handle error values potentially returned by socket programming library functions (see specifics for each language below).

##### Client specification
* Each client program should contact a server, read a message from stdin, send the message, and exit.
* Each client should read and send the message *exactly* as it appears in stdin until reaching an EOF (end-of-file).
* Each client should take two command-line arguments: the IP address of the server and the port number of the server.
* Each client must be able to handle arbitrarily large messages by iteratively reading and sending chunks of the message, rather than reading the whole message into memory first.
* Each client should handle partial sends (when a socket only transmits part of the data given in the last `send` call) by attempting to re-send the rest of the data until it has all been sent.
* Each client should gracefully handle error values potentially returned by socket programming library functions.

##### Getting started

We have provided scaffolding code in the `project5/client_server/` directory.
*You should read and understand this code before starting to program.*

You should program only in the locations of the provided files marked with `TODO` comments. There is one `TODO` section per client and one per server. You can add functions if you wish, but do not change file names, as they will be used for automated testing.

The following sections provide details for the client and server programs in each language.

##### C
The classic "Beej's Guide to Network Programming" is located here: https://beej.us/guide/bgnet/html/single/bgnet.html.  The [system call section](https://beej.us/guide/bgnet/html/single/bgnet.html#syscalls) and [client/server example section](https://beej.us/guide/bgnet/html/single/bgnet.html#clientserver) will be most relevant. The man pages are also useful for looking up individual functions (e.g.  `man socket`).

The files `client-c.c` and `server-c.c` contain scaffolding code. You will need to add socket programming and I/O code in the locations marked `TODO`. The reference solutions have roughly 70  (well commented and spaced) lines of code in the `TODO` sections of each file. Your implementations may be shorter or longer.

You should build your solution by running `make` in the `project5/client_server` directory. Your code *must* build using the provided Makefile. The server should be run as `./server-c [port] > [output file]`. The client should be run as `./client-c [server IP] [server port] < [message file]`.

##### Python
The documentation for Python socket programming is located here: https://docs.python.org/2/library/socket.html.  The first few paragraphs at the top, the [section on socket objects](https://docs.python.org/2/library/socket.html#socket-objects) and the [first example](https://docs.python.org/2/library/socket.html#example) are particularly relevant.

The files `client-python.py` and `server-python.py` contain the scaffolding code. You will need to add socket programming code in the locations marked `TODO`. The reference solutions have roughly 15  (well commented and spaced) lines of code in the `TODO` sections of each file. Your implementations may be shorter or longer.

The Python socket functions will automatically raise Exceptions with helpful error messages. No additional error handling is required.

The server should be run as `python server-python.py [port] > [output file]`. The client should be run as `python client-python.py [server IP] [server port] < [message file]`.

##### Testing

You should test your implementations by attempting to send messages from your clients to your servers. The server can be run in the background (append a `&` to the command) or in a separate SSH window. You should use `127.0.0.1` as the server IP and a high server port number between 10000 and 60000. You can kill a background server with the command `fg` to bring it to the foreground then `ctrl-c`.

The Bash script `test_client_server.sh` will test your implementation by attempting to send the same message ("Testing 1234!\n") between all 4 combinations of your clients and servers (C client to C server, C client to Python server, etc.). Run the script as

`./test_client_server.sh [server port]`

If you get a permissions error, run `chmod 744 test_client_server.sh` to give the script execute privileges.

For each client/server pair, the test script will print "SUCCESS" if the message is sent and received correctly. Otherwise it will print a diff of the sent and received message.

###### Debugging hints
Here are some debugging tips. If you are still having trouble, ask a question on Piazza or see an instructor during office hours.

* There are defined buffer size and queue length constants in the scaffolding code. Use them. If they are not defined in a particular file, you don't need them.
* There are multiple ways to read and write from stdin/stdout in C and Python. Any method is acceptable as long as it does not read an unbounded amount into memory at once and does not modify the message.
* If you are using buffered I/O to write to stdout, make sure to call `flush` or the end of a long message may not write.
* Remember to close the socket at the end of the client program.
* Make sure you are using `127.0.0.1` as the server IP argument to the client and the same server port for both client and server programs.
* If you get "address already in use" errors, make sure you don't already have a server running. Otherwise, restart your ssh session.
* If you are getting other connection errors, try a different port between 10000 and 60000.

##### Submission and grading
Submit the project by uploading your modified client and server files to Canvas.

We will grade your projects by running the `test_client_server.sh` script and additional tests with large messages, multiple simultaneous clients, etc. Double check the specifications above and perform your own tests before submitting.

#### Acknowledgements
Adapted with permission from Nick Feamster (feamster@cs.princeton.edu)

