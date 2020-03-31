/*!
 * \file
 * \brief Implémentation de la classe TelemetryAppender.
 */

#include "TelemetryAppender.hpp"

#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <ctime>


using namespace std::chrono;

logs::TelemetryAppender::TelemetryAppender(std::string ID)
{
    std::cout << "setup telemetry" << std::endl;
    id_ = ID;
    //setup telemetry
    t_fd = socket(AF_INET, SOCK_DGRAM, 0);
    int bcast = 1;
    setsockopt(t_fd, SOL_SOCKET, SO_BROADCAST, &bcast, sizeof(bcast));
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(10001);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(t_fd, (struct sockaddr*) &addr, sizeof(addr));



}

void logs::TelemetryAppender::flush()
{
    char buf[1024];
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(10000);
    addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);

    lockMessages();
    while (this->messages_.size() > 0) {
        std::string message = this->messages_.front();
        std::string message_net = id_ + "|" + message;

        std::cout << message << std::endl; //affichage console

        strncpy( buf, message_net.c_str(), sizeof( buf ) );
        //snprintf(buf, 1024, "MSG:test");

        sendto(t_fd, buf, 1024, 0, (struct sockaddr*) &addr, sizeof(addr));
        this->messages_.pop_front();
    }
    unlockMessages();

}

/*!
 * \brief Méthode générique de trace d'un message.
 * \param logger
 *        Logger de référence du message.
 * \param level
 *        Niveau de référence du message.
 * \param message
 *        Message à tracer.
 */
void logs::TelemetryAppender::writeMessage(const logs::Logger &logger, const logs::Level &level,
        const std::string &message)
{
    //logs::MemoryAppender::writeMessage(logger, level, message);
    //ajout de l'heure
//    time_t rawtime;
//      struct tm * timeinfo;
//      char buffer[80];
//
//      time (&rawtime);
//      timeinfo = localtime(&rawtime);
//
//      strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S ",timeinfo);
//      std::string str(buffer);

      //std::cout << str;
    //system_clock::time_point t = system_clock::now();
    //long duration = (duration_cast<microseconds>(t - start_).count());


    //std::cout << "It took me " << (duration_cast<microseconds>(t - start_).count()) << " microseconds.";
    //std::string d = std::to_string((duration_cast<microseconds>(t - start_).count()));

    logs::MemoryAppender::writeMessage(logger, level, message);
}

/*
 static int t_fd;

 void SetupTelemetry()
 {
 t_fd = socket(AF_INET, SOCK_DGRAM, 0);
 int bcast = 1;
 setsockopt(t_fd, SOL_SOCKET, SO_BROADCAST, &bcast, sizeof(bcast));
 struct sockaddr_in addr;
 addr.sin_family = lAF_INET;
 addr.sin_port = htons(10001);
 addr.sin_addr.s_addr = htonl(INADDR_ANY);
 bind(t_fd, (struct sockaddr *)&addr, sizeof(addr));
 }

 void SendADCTelemetry()
 {
 char buf[1024];
 struct sockaddr_in addr;
 addr.sin_family = AF_INET;
 addr.sin_port = htons(10000);
 addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
 snprintf(buf, 1024,
 "ADC: %04d %04d %04d %04d %04d %04d %04d %04d",
 MBUF->ADC[0], MBUF->ADC[1], MBUF->ADC[2], MBUF->ADC[3],
 MBUF->ADC[4], MBUF->ADC[5], MBUF->ADC[6], MBUF->ADC[7]);
 sendto(t_fd, buf, 1024, 0, (struct sockaddr *)&addr, sizeof(addr));
 }

 */
