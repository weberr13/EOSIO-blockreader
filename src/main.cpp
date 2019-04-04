#include <string>
#include <iostream>
#include <vector>
#include <curl/curl.h>

#define ENDPOINT "https://api.eosnewyork.io/v1"

struct MemoryStruct {
    std::vector<char> memory;
};
 
static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
  mem->memory.insert(mem->memory.end(), (char *)contents, (char *)contents+realsize);
 
  return realsize;
}

int main(int argc, char *argv[]) { 
    std::string id;
    bool raw = false;
    for (int i = 1; i < argc ; i++) {
        std::string arg = argv[i];
        if ("--block-detail" == arg) {
            if (i+1 >= argc) {
                std::cerr << "must specify block id" << std::endl;
                return -1;
            }
            id = argv[++i];
            if (i + 1 < argc) {
                arg = argv[++i];
                if (arg != "--raw") {
                    std::cerr << "unknown argument " << arg << std::endl;
                    return -1;
                }
                raw = true;
            }
        } else {
            std::cerr << "unknown argument " << arg << std::endl;
            return -1;
        }
    }
    curl_global_init(CURL_GLOBAL_DEFAULT);

    CURL* curl = curl_easy_init();
    if (curl) {

        struct MemoryStruct chunk;

        curl_easy_setopt(curl, CURLOPT_URL, ENDPOINT);
 
#ifdef SKIP_PEER_VERIFICATION
        /*
        * If you want to connect to a site who isn't using a certificate that is
        * signed by one of the certs in the CA bundle you have, you can skip the
        * verification of the server's certificate. This makes the connection
        * A LOT LESS SECURE.
        *
        * If you have a CA cert for the server stored someplace else than in the
        * default bundle, then the CURLOPT_CAPATH option might come handy for
        * you.
        */ 
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif
 
#ifdef SKIP_HOSTNAME_VERIFICATION
        /*
        * If the site you're connecting to uses a different host name that what
        * they have mentioned in their server certificate's commonName (or
        * subjectAltName) fields, libcurl will refuse to connect. You can skip
        * this check, but this will make the connection less secure.
        */ 
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif
        /* send all data to this function  */ 
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        
        /* we pass our 'chunk' struct to the callback function */ 
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
 
        /* Perform the request, res will get the return code */ 
        CURLcode res = curl_easy_perform(curl);
        /* Check for errors */ 
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            long response_code;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
            std::cout << "response code: " << response_code << std::endl;

            std::cout << "got the following: ";
            for (auto i : chunk.memory) {
                std::cout << i;
            } 
        }

        /* always cleanup */ 
        curl_easy_cleanup(curl);
    } else {
        std::cerr << "failure initializing curl" << std::endl;
        return -1;
    }
 
    curl_global_cleanup();


    return 0;
}

