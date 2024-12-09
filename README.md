To run the TCP client-server application

Client APIs located in client.c
server.c calls student database APIs in the student.c

The application execution starts from the main.c which calls read_data function in the parser.c

With Makefile:
Step 1: Run "make server" and "make client" to get executables server and client respectively
Step 2: In one terminal, run "./server" to run TCP server and in another terminal run client "./client <inputFile> <ipaddress> 9998" (port number is 9998)

Without Makefile:
gcc -o client main.c parser.c client.c
gcc -o server server.c student.c writer.c
In one terminal, run "./server" and in another terminal run "./client <inputFile> <ipaddress> 9998"

