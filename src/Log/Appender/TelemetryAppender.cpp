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
#include <netinet/in.h>
#include <arpa/inet.h>

#include <errno.h> //For errno - the error number
#include <netdb.h>   //hostent

#include "../../Common/Utils/json.hpp"

using namespace std::chrono;

logs::TelemetryAppender::TelemetryAppender(std::string Id_Robot, std::string PlotJuggler_hostname)
{
    id_ = Id_Robot;

    t_fd = socket(AF_INET, SOCK_DGRAM, 0); //UDP
    char hostname[50];
    strcpy(hostname, PlotJuggler_hostname.c_str()); //NOM DE LA VM DE LOG aec PLOTJUGGLER
    int err = hostname_to_ip(hostname, ip_);
    if (err == 1) {
        //printf("Impossible to resolve PLOTJUGGLER VM on %s ; err=%d => EXIT !\n", hostname, err);
        std::string ip = "192.168.4.104";
        //std::cout << "Impossible to resolve PLOTJUGGLER VM on " << PlotJuggler_hostname << "; err="<< err <<" => ip="<< ip << std::endl;

        strcpy(ip_, ip.c_str());

    }
    //printf("%s resolved to %s\n", hostname, ip_);

    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(9870); //PORT envoyer
    //addr_.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr_.sin_addr.s_addr = inet_addr(ip_);

}

void logs::TelemetryAppender::flush() {
    char buf[1024];

    lockMessages();

    while (this->messagesjson_.size() > 0) {
        std::string message = this->messagesjson_.front();

        strncpy(buf, message.c_str(), sizeof(buf));

        sendto(t_fd, buf, 1024, 0, (struct sockaddr*) &addr_, sizeof(addr_));
        this->messagesjson_.pop_front();
    }

    while (this->messages_.size() > 0) {
        std::string message = this->messages_.front();
        std::cout << message << std::endl; //AFFICHAGE CONSOLE
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
void logs::TelemetryAppender::writeMessage(const logs::Logger &logger, const logs::Level &level, const std::string &message) {

    if (level == logs::Level::TELEM or level == logs::Level::ERROR) {
        //LOG Telemetry
        writeMessageWithJsonTime(id_, logger, level, message);
    }

    if (level != logs::Level::TELEM) {
        //Log Console
        logs::MemoryAppender::writeMessage(logger, level, message);
    }
}

void logs::TelemetryAppender::writeMessageWithJsonTime(std::string id, const logs::Logger & logger, const logs::Level &level,
        const std::string & message)
{

    uint64_t duration = (duration_cast<microseconds>(system_clock::now() - start_).count());
    uint64_t ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    nlohmann::json j;
    j[id]["timestamp"] = (double)(ms/1000.0); //pour avoir les millisseconds 3 chiffres apres la virgule
    j[id]["elapsedtime_ms"] = (double)(duration/1000.0);


    if (level == logs::Level::TELEM) {
        try
        {
            j[id][logger.name()] = nlohmann::json::parse(message);
        }catch(const std::exception& e)
        {
            std::cout << e.what(); // information from length_error printed
            std::cout << "!!!!!!!ERROR msg is" << message << std::endl;
            exit(0);
        }

    }
    else if (level == logs::Level::ERROR) {
        j[id][logger.name()]["ERROR"][message] = duration;
    }

    this->lockMessages();
    this->messagesjson_.push_back(j.dump());
    this->unlockMessages();
}

//return 1 if error
int logs::TelemetryAppender::hostname_to_ip(char * hostname, char* ip) {
    struct hostent *he;
    struct in_addr **addr_list;
    int i;

    if ((he = gethostbyname(hostname)) == NULL) {
        // get the host info
        //herror("gethostbyname");
        return 1;
    }

    addr_list = (struct in_addr **) he->h_addr_list;

    for (i = 0; addr_list[i] != NULL; i++) {
        //Return the first one;
        strcpy(ip, inet_ntoa(*addr_list[i]));
        return 0;
    }

    return 1;
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
