// http_client.h

#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include "sensors.h"

class HTTPWebClient{
    public:
        void init(const char *serverURL);
        bool sendReadings(const SensorData &data);

    private:
        String _serverURL;
};

#endif