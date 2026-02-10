#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <qstring.h>


/**
 * @class Environment
 * @brief Singleton-tyyppinen apuluokka, joka sisältää backendin base-URL:n.
 *
 * Käytetään kaikkialla API-kutsuissa.
 */
class Environment
{
public:
    Environment();
    /**
     * @brief Palauttaa backendin perus-URL:n.
     * @return esim. "http://localhost:3000"
     */
    static QString base_url();
};

#endif // ENVIRONMENT_H
