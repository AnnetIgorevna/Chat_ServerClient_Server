#include "Chat_ServerTCP.h"
#include <arpa/inet.h>

Chat_ServerTCP::~Chat_ServerTCP()
{
    // ��������� �����, ��������� ����������
    close(socket_file_descriptor);
}

bool Chat_ServerTCP::createSocket()
{
    // ��������� �����
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    // ���� �� �������
    if (socket_file_descriptor == -1) {
        std::cout << "Socket creation failed!" << std::endl;
        return false;
    }
    // �������� IP-����� �����
    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
    // ������� ����� ����� ��� �����
    serveraddress.sin_port = htons(PORT);
    // ���������� IPv4
    serveraddress.sin_family = AF_INET;
    
    // �������� �����
    bind_status = bind(socket_file_descriptor, (struct sockaddr*)&serveraddress,
        sizeof(serveraddress));
    // ���� �� �������
    if (bind_status == -1) {
        std::cout << "Socket binding failed!" << std::endl;
        return false;;
    }
    return true;
}

void Chat_ServerTCP::run()
{
    // ����� ����������� � ����� �������� �������� �� ������� ��������
    connection_status = listen(socket_file_descriptor, 5);
    // ���� �� �������
    if (connection_status == -1) {
        std::cout << "Socket is unable to listen for new connections!" << std::endl;
        exit(1);
    }
    else {
        std::cout << "Server is listening for new connection..." << std::endl;
    }

    length = sizeof(client);
    // ������ ��� ������� � �������� ����� ����� � ���������� ��� ����������
    connection = accept(socket_file_descriptor, (struct sockaddr*)&client, &length);
    // ���� �� �������
    if (connection == -1) {
        std::cout << "Server is unable to accept the data from client!" << std::endl;
        exit(1);
    }

    std::cout << "Server: got connection from " << inet_ntoa(client.sin_addr)  << " port " << ntohs(client.sin_port) << std::endl;
    communication();
}

void Chat_ServerTCP::communication()
{
    // �������� � ��������� ������
    while (1) {
        // �������� ���������
        bzero(message, MESSAGE_LENGTH);
        std::cout << "Message from Server: ";
        std::cin.getline(message, MESSAGE_LENGTH);
        //std::cin >> message;
        ssize_t bytes = write(connection, message, sizeof(message));
        // ���� �������� >= 0  ����, ������ ��������� ������ �������
        if (bytes >= 0) {
            std::cout << "Data successfully sent to the client!" << std::endl;
        }

        // ����� ���������
        bzero(message, MESSAGE_LENGTH);
        read(connection, message, sizeof(message));
        if (strncmp("end", message, 3) == 0) {
            std::cout << "Client Exited" << std::endl;
            std::cout << "Press any key to exit..." << std::endl;
            std::cin.get();
            return;
        }
        std::cout << "Message from client: " << message << std::endl;
    }
}


