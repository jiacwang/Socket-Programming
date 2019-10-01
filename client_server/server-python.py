import sys
import socket


RECV_BUFFER_SIZE = 2048
QUEUE_LENGTH = 10


def recvall(sock):
    data = b''
    while True:
        part = sock.recv(RECV_BUFFER_SIZE)
        data += part
        if len(part) == 0:
            # no more data to receive
            break
    
    sys.stdout.write(data)
    sys.stdout.flush()

    return data
    
    
def server(server_port):
    """TODO: Listen on socket and print received message to sys.stdout"""
    HOST = ''                 
    PORT = server_port              
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind((HOST, PORT))
    s.listen(QUEUE_LENGTH)
    
    while 1:
        # main accept loop
        conn, addr = s.accept()

        data_received = recvall(conn)

        if not data_received:
            data_received = recvall(conn)

        # we have received all data and printed it, so we can close connection 
        conn.close()
    # close the socket    
    s.close()
    


def main():
    """Parse command-line argument and call server function """
    if len(sys.argv) != 2:
        sys.exit("Usage: python server-python.py [Server Port]")
    server_port = int(sys.argv[1])
    server(server_port)

if __name__ == "__main__":
    main()
