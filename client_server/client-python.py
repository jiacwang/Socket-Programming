import sys
import socket


SEND_BUFFER_SIZE = 2048

def client(server_ip, server_port):
    """TODO: Open socket and send message from sys.stdin"""

    HOST = server_ip                
    PORT = server_port              
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, PORT))

    while 1:
        # partial send, each time we send SEND_BUFFER_SIZE data
        buffer = sys.stdin.read(SEND_BUFFER_SIZE)
        if not buffer:
            break
        # this print is for debugging only
        print (buffer)
        s.sendall(buffer)
    # we should read from stdin and send it
    s.close()
     

def main():
    """Parse command-line arguments and call client function """
    if len(sys.argv) != 3:
        sys.exit("Usage: python client-python.py [Server IP] [Server Port] < [message]")
    server_ip = sys.argv[1]
    server_port = int(sys.argv[2])
    client(server_ip, server_port)

if __name__ == "__main__":
    main()
