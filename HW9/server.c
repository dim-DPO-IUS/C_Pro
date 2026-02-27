/*!
 * Simple chat program (server side).cpp - http://github.com/hassanyf
 * Version - 2.0.1
 *
 * Copyright (c) 2016 Hassan M. Yousuf
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int server;          // дескриптор сервера
    int client;          // дескриптор клиента 
    int portNum = 8001;  // номера порта, на котором происходит соединение (0 до 65535)
    int bufsize = 1024;  // размер буффера
    char buffer[bufsize]; // буффер обмена для приема/отправки данных к/от сервера


    struct sockaddr_in server_addr;
    socklen_t size;


    server = socket(AF_INET, SOCK_STREAM, 0); // Создание сервер-сокета
    printf("SERVER\n");

    if (server < 0)
    {
        printf("Error establishing socket...\n");
        exit(1);
    }

    printf("=> Socket server has been created...\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // IP-адрес компьютера (любой доступный интерфейс)
    server_addr.sin_port = htons(portNum);           // порт, на котором сервер будет работать

    if ((bind(server, (struct sockaddr*)&server_addr, sizeof(server_addr))) < 0)
    {
        printf("=> Error binding connection, the socket has already been established...\n");
        return -1;
    }

    size = sizeof(server_addr);
    printf("=> Looking for clients...\n"); // Ждем клиента

    listen(server, 1);

    int clientCount = 1;

    printf("=> Server started. Press Ctrl+C to stop...\n");
    printf("==========================================\n\n");

    while (1)  // Бесконечный цикл - работает до Ctrl+C
    {
        client = accept(server, (struct sockaddr*)&server_addr, &size);

        // первая проверка действительности подключения
        if (client < 0)
        {
            printf("=> Error on accepting...\n");
            continue;
        }

        printf("=> Connected with the client %d, you are good to go...\n", clientCount);
        printf("\n=> Waiting for request...\n");

        int result = recv(client, buffer, bufsize - 1, 0);
        if (result < 0)
        {
            printf("\n\n=> Connection terminated error %d with IP %s\n", result, inet_ntoa(server_addr.sin_addr));
            close(client);
            continue;
        }

        buffer[result] = '\0';

        printf("\n========== REQUEST ==========\n");
        printf("%s\n", buffer);
        printf("=============================\n");

        static int led1_state = 0;  // 0 - выключен, 1 - включен
        static int led2_state = 0;

        if (strstr(buffer, "/on1"))
        {
            printf("=> LED1 включен!\n");
            led1_state = 1;
        }
        if (strstr(buffer, "/off1"))
        {
            printf("=> LED1 выключен!\n");
            led1_state = 0;
        }
        if (strstr(buffer, "/on2"))
        {
            printf("=> LED2 включен!\n");
            led2_state = 1;
        }
        if (strstr(buffer, "/off2"))
        {
            printf("=> LED2 выключен!\n");
            led2_state = 0;
        }

        char response[3072] =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html; charset=utf-8\r\n"
            "\r\n"
            "<!DOCTYPE HTML>"
            "<html>"
            "  <head>"
            "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
            "    <style>"
            "      body { font-family: Arial; margin: 40px; background-color: #f5f5f5; }"
            "      h1 { color: #333; }"
            "      .led-container {"
            "        background-color: white;"
            "        padding: 20px;"
            "        border-radius: 10px;"
            "        box-shadow: 0 2px 10px rgba(0,0,0,0.1);"
            "        margin: 20px 0;"
            "      }"
            "      .led-row {"
            "        display: flex;"
            "        align-items: center;"
            "        margin: 20px 0;"
            "        padding: 10px;"
            "        background-color: #fafafa;"
            "        border-radius: 8px;"
            "      }"
            "      .led {"
            "        width: 60px;"
            "        height: 60px;"
            "        border-radius: 50%;"
            "        margin-right: 20px;"
            "        transition: all 0.3s ease;"
            "        border: 3px solid #333;"
            "      }"
            "      .led-on {"
            "        background-color: #0066ff;"
            "        box-shadow: 0 0 20px #0066ff;"
            "      }"
            "      .led-off {"
            "        background-color: #cccccc;"
            "        box-shadow: none;"
            "      }"
            "      .button-container {"
            "        margin-left: 20px;"
            "      }"
            "      button {"
            "        padding: 12px 30px;"
            "        font-size: 16px;"
            "        border: none;"
            "        border-radius: 5px;"
            "        cursor: pointer;"
            "        margin: 0 5px;"
            "        transition: all 0.3s ease;"
            "      }"
            "      .on { background-color: #4CAF50; color: white; }"
            "      .on:hover { background-color: #45a049; transform: scale(1.05); }"
            "      .off { background-color: #f44336; color: white; }"
            "      .off:hover { background-color: #da190b; transform: scale(1.05); }"
            "      .status-text {"
            "        margin-left: 20px;"
            "        font-size: 18px;"
            "        color: #666;"
            "      }"
            "    </style>"
            "  </head>"
            "  <body>"
            "    <h1> ESP32 - Web Server</h1>"
            "    <div class=\"led-container\">"
            "      <div class=\"led-row\">"
            "        <div class=\"led ";

        // Добавляем класс для LED1 (on или off)
        if (led1_state)
            strcat(response, "led-on");
        else
            strcat(response, "led-off");

        strcat(response, "\"></div>"
            "        <h2>LED #1</h2>"
            "        <div class=\"status-text\">");

        // Добавляем текстовый статус для LED1
        if (led1_state)
            strcat(response, "⚡ Включен");
        else
            strcat(response, " Выключен");

        strcat(response, "</div>"
            "        <div class=\"button-container\">"
            "          <a href=\"on1\"><button class=\"on\">ВКЛ</button></a>"
            "          <a href=\"off1\"><button class=\"off\">ВЫКЛ</button></a>"
            "        </div>"
            "      </div>"
            "      <div class=\"led-row\">"
            "        <div class=\"led ");

        // Добавляем класс для LED2 (on или off)
        if (led2_state)
            strcat(response, "led-on");
        else
            strcat(response, "led-off");

        strcat(response, "\"></div>"
            "        <h2>LED #2</h2>"
            "        <div class=\"status-text\">");

        // Добавляем текстовый статус для LED2
        if (led2_state)
            strcat(response, "⚡ Включен");
        else
            strcat(response, " Выключен");

        strcat(response, "</div>"
            "        <div class=\"button-container\">"
            "          <a href=\"on2\"><button class=\"on\">ВКЛ</button></a>"
            "          <a href=\"off2\"><button class=\"off\">ВЫКЛ</button></a>"
            "        </div>"
            "      </div>"
            "    </div>"
            "    <p><em> Сервер работает... Нажми Ctrl+C для остановки</em></p>"
            "  </body>"
            "</html>");

        send(client, response, strlen(response), 0);
        printf("=> HTTP response sent (%ld bytes)\n", strlen(response));

        printf("\n\n=> Connection terminated with IP %s\n", inet_ntoa(server_addr.sin_addr));
        close(client);

        clientCount++;
        printf("\n=> Waiting for next client...\n");
        printf("==========================================\n\n");

    } 

    close(server);
    printf("\nGoodbye...\n");

    return 0;
}