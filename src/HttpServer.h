#pragma once

#include <WebServer.h>

#include "EEPROMConfiguration.h"

class HttpServer
{
public:
	HttpServer() ;
	virtual ~HttpServer();

	void setup(void);
	void handle(void);

	String getContentType(String filename);
  bool handleFileRead(String path);
  
  static void handleNotFound();
  static void moveLeg();
  static void walk();
  static void handleGetConfig();
  static void handleResetConfig();
  static void handleSetConfig();

  WebServer& webServer();
private:

  WebServer           _webServer;
  void sendCors();
};

#if !defined(NO_GLOBAL_INSTANCES)
extern HttpServer HTTPServer;
#endif

