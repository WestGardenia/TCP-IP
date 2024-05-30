#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

int main()
{
    WSADATA wsaData;
    SOCKET clientSocket;
    SOCKADDR_IN addr;
    char buff[1024];
    int r;

    // 1. ����Э��汾
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
        printf("����汾Э��ʧ��\n");
        return -1;
    }
    printf("����Э��ɹ�\n");

    // 2. ����socket
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) { // ʹ��INVALID_SOCKET��
        printf("����socketʧ��\n");
        WSACleanup();
        return -2;
    }
    printf("����socket�ɹ�\n");

    // 3. ��ȡ������Э���ַ��
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.S_un.S_addr = inet_addr("192.168.50.123");
    addr.sin_port = htons(10086);

    // 4. ���ӷ�����
    r = connect(clientSocket, (sockaddr*)&addr, sizeof(addr));
    if (r == SOCKET_ERROR) {
        printf("���ӷ�����ʧ��\n");
        closesocket(clientSocket); // �ر��׽���
        WSACleanup();
        return -1;
    }
    printf("���ӷ������ɹ�\n");

    // 5. ͨ��
    while (1) {
        memset(buff, 0, sizeof(buff));
        printf("���������֣�");
        if (fgets(buff, sizeof(buff), stdin) == NULL) { // ʹ��fgets����ȡһ������
            break; // �������ʧ�ܣ����˳�ѭ��
        }
        r = send(clientSocket, buff, strlen(buff), 0);
        if (r == SOCKET_ERROR) { // ���send�����Ƿ�ɹ�
            perror("��������ʧ��");
            break;
        }
    }

    // 6. �ر�socket
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}