/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 *
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * io.h - Input/output functions (serial/ethernet)
 */

#include <windows.h>
#include <string.h>
#include <winsock.h>
#include <stdbool.h>
#include "io.h"
#include "protocol.h"
#include "splash.h"

#pragma comment(lib,"winsock.lib")

#define true 1
#define false 0

extern padByte buffer[16384];
extern padByte replay_buffer[16384];
extern int replaylen;
extern int buflen;

WSADATA wsaData;
struct sockaddr_in serveraddr;
WORD wVersionRequested;
SOCKET sockfd;

/**
 * io_init() - Set-up the I/O
 */
SOCKET io_init(void)
{
    u_long imode = 0;

    memcpy(buffer,splash,sizeof(splash));
    memcpy(replay_buffer,splash,sizeof(splash));
    buflen=replaylen=sizeof(splash);

    wVersionRequested = 0x101; // Winsock 1.1
    WSAStartup(wVersionRequested,&wsaData);

    sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr("47.190.140.76");
    serveraddr.sin_port = htons((unsigned short) 8005);
    ioctlsocket(sockfd, FIONBIO, &imode);
    connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    return sockfd;
}

/**
 * io_send_byte(b) - Send specified byte out
 */
void io_send_byte(unsigned char b)
{
    send(sockfd,&b,1,0);
}

/**
 * io_main() - The IO main loop
 */
int io_main(void)
{
    buflen = recv(sockfd,&buffer[0],16384,0);

    if (buflen>0)
    {
        memcpy(&replay_buffer[replaylen],&buffer[0],buflen);
        replaylen += buflen;
    }

    return buflen;
}

/**
 * io_done() - Called to close I/O
 */
void io_done(void)
{
    closesocket(sockfd);
    WSACleanup();
}

void io_replay_clear(void)
{
    memset(replay_buffer,0,sizeof(replay_buffer));
    replaylen = 0;
}
