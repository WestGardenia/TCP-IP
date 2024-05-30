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

    // 1. 请求协议版本
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
        printf("请求版本协议失败\n");
        return -1;
    }
    printf("请求协议成功\n");

    // 2. 创建socket
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) { // 使用INVALID_SOCKET宏
        printf("创建socket失败\n");
        WSACleanup();
        return -2;
    }
    printf("创建socket成功\n");

    // 3. 获取服务器协议地址族
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.S_un.S_addr = inet_addr("192.168.50.123");
    addr.sin_port = htons(10086);

    // 4. 连接服务器
    r = connect(clientSocket, (sockaddr*)&addr, sizeof(addr));
    if (r == SOCKET_ERROR) {
        printf("连接服务器失败\n");
        closesocket(clientSocket); // 关闭套接字
        WSACleanup();
        return -1;
    }
    printf("连接服务器成功\n");

    // 5. 通信
    while (1) {
        memset(buff, 0, sizeof(buff));
        printf("请输入文字：");
        if (fgets(buff, sizeof(buff), stdin) == NULL) { // 使用fgets来读取一行输入
            break; // 如果输入失败，则退出循环
        }
        r = send(clientSocket, buff, strlen(buff), 0);
        if (r == SOCKET_ERROR) { // 检查send函数是否成功
            perror("发送数据失败");
            break;
        }
    }

    // 6. 关闭socket
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}