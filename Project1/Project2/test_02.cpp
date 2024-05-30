#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

int main()
{
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    SOCKADDR_IN addr, cAddr;
    int r, len;
    char buff[1024];

    // 1. ����Э��汾
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
        printf("����汾Э��ʧ��\n");
        return -1;
    }
    printf("����Э��ɹ�\n");

    // 2. ����socket
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        printf("����socketʧ��\n");
        WSACleanup();
        return -2;
    }
    printf("����socket�ɹ�\n");

    // 3. ����Э���ַ��
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.S_un.S_addr = inet_addr("192.168.50.123");
    addr.sin_port = htons(10086);

    // 4. ��
    r = bind(serverSocket, (sockaddr*)&addr, sizeof(addr));
    if (r == SOCKET_ERROR) {
        printf("��ʧ��\n");
        closesocket(serverSocket);
        WSACleanup();
        return -3;
    }
    printf("�󶨳ɹ�\n");

    // 5. ����
    r = listen(serverSocket, 10);
    if (r == SOCKET_ERROR) {
        printf("����ʧ��\n");
        closesocket(serverSocket);
        WSACleanup();
        return -4;
    }
    printf("�����ɹ�\n");

    // 6. �ȴ��ͻ�������
    len = sizeof(cAddr);
    while (1) {
        clientSocket = accept(serverSocket, (sockaddr*)&cAddr, &len);
        if (clientSocket == INVALID_SOCKET) {
            printf("������ӵ��\n");
            closesocket(serverSocket);
            WSACleanup();
            return -5;
        }
        printf("�пͻ������ӵ��������ˣ�ip��ַ�ǣ�%s\n", inet_ntoa(cAddr.sin_addr));

        // 7. ͨ��
        while (1) {
            memset(buff, 0, sizeof(buff));
            r = recv(clientSocket, buff, sizeof(buff) - 1, 0); // �������ݣ�����һ���ַ��Ŀռ��'\0'
            if (r > 0) {
                buff[r] = '\0'; // ����ַ���������
                printf("�յ���Ϣ��%s\n", buff);
            }
            else if (r == 0) {
                printf("�ͻ��˶Ͽ�����\n");
                break; // �ͻ��˹ر�����
            }
            else {
                perror("��������ʧ��");
                break;
            }
        }

        // 8. �رտͻ���socket
        closesocket(clientSocket);
    }

    // 9. �ر�serverSocket
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}