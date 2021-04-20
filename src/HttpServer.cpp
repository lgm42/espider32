#include <littleFS.h>
#include <ArduinoJson.h>

#include "HttpServer.h"
#include "MovementManager.h"
#include "Logger.h"

/********************************************************/
/******************** Public Method *********************/
/********************************************************/

HttpServer::HttpServer()
    : _webServer(80)
{
}

HttpServer::~HttpServer()
{
}

void HttpServer::setup(void)
{
  _webServer.on("/restart", [&]() {
    _webServer.send(200, "text/plain", "ESP restart now !");
    delay(200);
    ESP.restart();
  });

  _webServer.on("/moveLeg", HttpServer::moveLeg);
  _webServer.on("/walk", HttpServer::walk);

  _webServer.on("/getConfig", HttpServer::handleGetConfig);
  _webServer.on("/resetConfig", HttpServer::handleResetConfig);
  _webServer.on("/setConfig", HTTP_POST, HttpServer::handleSetConfig);

  _webServer.onNotFound([&]() {
		if (!handleFileRead(_webServer.uri()))
			_webServer.send(404, "text/plain", "FileNotFound");
	});

  _webServer.begin();
}

void HttpServer::handle(void)
{
  _webServer.handleClient();
}

String HttpServer::getContentType(String filename)
{
  if (_webServer.hasArg("download"))
    return "application/octet-stream";
  else if (filename.endsWith(".htm"))
    return "text/html";
  else if (filename.endsWith(".html"))
    return "text/html";
  else if (filename.endsWith(".css"))
    return "text/css";
  else if (filename.endsWith(".js"))
    return "application/javascript";
  else if (filename.endsWith(".png"))
    return "image/png";
  else if (filename.endsWith(".gif"))
    return "image/gif";
  else if (filename.endsWith(".jpg"))
    return "image/jpeg";
  else if (filename.endsWith(".ico"))
    return "image/x-icon";
  else if (filename.endsWith(".xml"))
    return "text/xml";
  else if (filename.endsWith(".pdf"))
    return "application/x-pdf";
  else if (filename.endsWith(".zip"))
    return "application/x-zip";
  else if (filename.endsWith(".gz"))
    return "application/x-gzip";
  return "text/plain";
}

// send the right file to the client (if it exists)
bool HttpServer::handleFileRead(String path)
{
  HTTPServer.sendCors();
	if (_webServer.method() == HTTP_OPTIONS)
  {
    _webServer.send(204);
		return true;
	}

  if (path.endsWith("/")) 
      path += "index.html";

  /*if (path.endsWith("/index.html"))
  {
    _webServer.sendHeader("Content-Encoding", "gzip");
    _webServer.send(200, "text/html", index_html_gz, index_html_gz_len);
    return true;
  }*/
  return false;
}

void HttpServer::sendCors()
{
  _webServer.sendHeader("Access-Control-Allow-Origin", "*");
  _webServer.sendHeader("Access-Control-Max-Age", "10000");
  _webServer.sendHeader("Access-Control-Allow-Methods", "PUT,POST,GET,OPTIONS");
  _webServer.sendHeader("Access-Control-Allow-Headers", "*");
}

void HttpServer::handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += HTTPServer.webServer().uri();
  message += "\nMethod: ";
  message += (HTTPServer.webServer().method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += HTTPServer.webServer().args();
  message += "\n";
  for (uint8_t i = 0; i < HTTPServer.webServer().args(); i++)
  {
    message += " " + HTTPServer.webServer().argName(i) + ": " + HTTPServer.webServer().arg(i) + "\n";
  }
  HTTPServer.webServer().send(404, "text/plain", message);
}

void HttpServer::moveLeg()
{
  String message = "moveLeg ok";

  int legNumber = HTTPServer.webServer().arg("leg").toInt();

  if (HTTPServer.webServer().hasArg("haunch"))
    Movement.legs()[legNumber]->haunch().position(HTTPServer.webServer().arg("haunch").toInt());
  if (HTTPServer.webServer().hasArg("knee"))
    Movement.legs()[legNumber]->knee().position(HTTPServer.webServer().arg("knee").toInt());
  if (HTTPServer.webServer().hasArg("ankle"))
    Movement.legs()[legNumber]->ankle().position(HTTPServer.webServer().arg("ankle").toInt());

  HTTPServer.sendCors();
  HTTPServer.webServer().send(200, "text/plain", message);
}

void HttpServer::walk()
{
  String message = "walk ok";

  Movement.walkFront();
  HTTPServer.sendCors();
  HTTPServer.webServer().send(200, "text/plain", message);
}

void HttpServer::handleGetConfig()
{
  HTTPServer.sendCors();
  HTTPServer.webServer().send(200, "application/json", Configuration.toJson());
}

void HttpServer::handleResetConfig()
{
  String message;
  
  Configuration.restoreDefault();

  HTTPServer.sendCors();
  HTTPServer.webServer().send(200, "application/json", message);
}

void HttpServer::handleSetConfig()
{

  Log.println("handleSetConfig");
  HTTPServer.sendCors();
  if (HTTPServer.webServer().hasArg("plain")== false){ //Check if body received

        HTTPServer.webServer().send(400, "text/plain", "Body not received");
        return;
  }

  Configuration.fromJson(HTTPServer.webServer().arg("plain"));

  HTTPServer.webServer().send(200, "application/json", Configuration.toJson());
}

WebServer &HttpServer::webServer()
{
  return _webServer;
}

#if !defined(NO_GLOBAL_INSTANCES)
HttpServer HTTPServer;
#endif
