/*
 * Class for UDP sockets
 *
 * Copyright (C) 2019 Simon D. Levy
 *
 * MIT License
 */

#pragma once

#ifdef _WIN32
#include "WindowsSocket.hpp"
#else
#include "LinuxSocket.hpp"
#endif

class UdpSocket : public Socket {

    protected:

        struct sockaddr_in _si_other;
        socklen_t _slen = sizeof(_si_other);

        void setupTimeout(uint32_t msec)
        {
            if (msec > 0) {
                Socket::setUdpTimeout(msec);
            }
        }

    public:

        void sendData(void * buf, size_t len)
        {
            sendto(_sock, (const char *)buf, (int)len, 0,
                    (struct sockaddr *) &_si_other, (int)_slen);

        }

        bool receiveData(void * buf, size_t len)
        {
            return recvfrom(_sock, (char *)buf, (int)len, 0,
                    (struct sockaddr *) &_si_other, &_slen) 
                == (recv_size_t)len;
        }

        static UdpSocket * free(UdpSocket * socket)
        {
            socket->closeConnection();
            delete socket;
            return NULL;
        }
};
