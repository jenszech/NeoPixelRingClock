
#include "Arduino.h"

// Include WebServer Lib
#include <ESP8266WebServer.h>
#include <FS.h>  // muss vor <detail\RequestHandlersImpl.h> stehen (s. Hinweis im Anschluss)
#include <EspHtmlTemplateProcessor.h>

class Webserver {
public:
    Webserver();
    void initialize();
    void loopWebServer();

//private:
    //Page handler
    static void notfoundPage();
    
    static void handleRootIndex();
    static void handleRootConfig();
    static String indexKeyProcessor(const String& var);

    static ESP8266WebServer server;
    static EspHtmlTemplateProcessor templateProcessor;
};
