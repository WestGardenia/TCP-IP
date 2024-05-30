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

    // 1. 请求协议版本
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
        printf("请求版本协议失败\n");
        return -1;
    }
    printf("请求协议成功\n");

    // 2. 创建socket
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        printf("创建socket失败\n");
        WSACleanup();
        return -2;
    }
    printf("创建socket成功\n");

    // 3. 创建协议地址族
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.S_un.S_addr = inet_addr("192.168.50.123");
    addr.sin_port = htons(10086);

    // 4. 绑定
    r = bind(serverSocket, (sockaddr*)&addr, sizeof(addr));
    if (r == SOCKET_ERROR) {
        printf("绑定失败\n");
        closesocket(serverSocket);
        WSACleanup();
        return -3;
    }
    printf("绑定成功\n");

    // 5. 监听
    r = listen(serverSocket, 10);
    if (r == SOCKET_ERROR) {
        printf("监听失败\n");
        closesocket(serverSocket);
        WSACleanup();
        return -4;
    }
    printf("监听成功\n");

    // 6. 等待客户端连接
    len = sizeof(cAddr);
    while (1) {
        clientSocket = accept(serverSocket, (sockaddr*)&cAddr, &len);
        if (clientSocket == INVALID_SOCKET) {
            printf("服务器拥堵\n");
            closesocket(serverSocket);
            WSACleanup();
            return -5;
        }
        printf("有客户端连接到服务器了，ip地址是：%s\n", inet_ntoa(cAddr.sin_addr));

        // 7. 通信
        while (1) {
            memset(buff, 0, sizeof(buff));
            r = recv(clientSocket, buff, sizeof(buff) - 1, 0); // 接收数据，留出一个字符的空间给'\0'
            if (r > 0) {
                buff[r] = '\0'; // 添加字符串结束符
                printf("收到消息：%s\n", buff);
            }
            else if (r == 0) {
                printf("客户端断开连接\n");
                break; // 客户端关闭连接
            }
            else {
                perror("接收数据失败");
                break;
            }
        }

        // 8. 关闭客户端socket
        closesocket(clientSocket);
    }

    // 9. 关闭serverSocket
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}