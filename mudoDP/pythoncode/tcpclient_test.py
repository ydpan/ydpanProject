from socket import *
tcp_client_socket = socket(AF_INET,SOCK_STREAM)

server_ip = '127.0.0.1'
server_port = 2016
tcp_client_socket.connect(server_ip,server_port)
recvData = tcp_client_socket.recv(1024)
print("recv datas",recvData.decode('gbk'))