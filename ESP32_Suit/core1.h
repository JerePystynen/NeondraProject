/*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^

[CORE 1]
  Communicates with the clients that want to control the suit.
  Serves a website with changeable settings.

*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*^*/

/// WEBSERVER

#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <mDNS.h>

// Access Point Credentials
const char* AP_SSID = "Neondra_AP";
const char* AP_PASSWORD = "neondra123";

#include "interface.h"

IPAddress IPaddr(127, 0, 0, 1);
IPAddress IPmask(255, 255, 255, 0);

const bool USE_SECONDS = true;
int hours, minutes, seconds;

const char* ntp_server_name = "pool.ntp.org";
WiFiUDP udp;                                            // Used to send and receive NTP packets
NTPClient timeClient(udp, ntp_server_name, 2 * 60 * 60);  // NTPClient to get time with UTC offset in seconds (UTC+2)

WebServer server(80);

// Interface

void handleRoot(void) {
  server.send(200, "text/html", HTML_CONTENT);
}

void handleAjax(void) {
  String data = server.arg("plain");
  Serial.println("Received data via Ajax POST: " + data);

  // Process the received data as needed

  server.send(200, "text/plain", "Data received successfully!");
}

void handlePlay(void) {
  df_player_mini.start();
  should_play = true;
  current_status = "Playing";
  handleRoot();
}

void handleStop(void) {
  df_player_mini.stop();
  should_play = false;
  current_status = "Stopped";
  handleRoot();
}

void handlePlayNext(void) {
  playNext();
  handleRoot();
}

void handlePlayPrevious(void) {
  df_player_mini.play(--current_track > 0 ? current_track : COUNT_TRACKS);
  should_play = true;
  current_status = "Previous";
  handleRoot();
}

void handleVolumeUp(void) {
  if (current_volume < VOLUME_MAX) {
    current_volume += 5;
    if (current_volume > VOLUME_MAX) current_volume = VOLUME_MAX;
    df_player_mini.volume(current_volume);
  }
  current_status = "VolumeUp";
  handleRoot();
}

void handleVolumeDown(void) {
  if (current_volume > 5) {
    current_volume -= 5;
    df_player_mini.volume(current_volume);
  }
  current_status = "VolumeDown";
  handleRoot();
}

void initializeInterface(void) {
  // Connect to the existing Wi-Fi network as a station
  WiFi.hostname("Neondra Suit");
  WiFi.begin(STATION_SSID, STATION_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");
  // Define web server routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/play", HTTP_GET, handlePlay);
  server.on("/stop", HTTP_GET, handleStop);
  server.on("/volumeup", HTTP_GET, handleVolumeUp);
  server.on("/volumedown", HTTP_GET, handleVolumeDown);
  server.on("/playnext", HTTP_GET, handlePlayNext);
  server.on("/playprevious", HTTP_GET, handlePlayPrevious);

  // Communication between Neondra and Spitfire
  server.on("/receive", HTTP_GET, []() {
    Serial.println("Spitfire's initial ping received. Let's send something back!");
    server.send(200);
    //messageSpitfire();
  });

  // Start the server
  server.begin();
  //MDNS.addService("http", "tcp", 80);

  // Initialize and synchronize time
  // timeClient.begin();
  // timeClient.update();
}

// Setup

void SETUP_CORE_1(void) {
  initializeInterface();

  // Set up the access point
  Serial.println("Initializing Wi-Fi access point...");
  WiFi.softAP(AP_SSID, AP_PASSWORD);
  WiFi.softAPConfig(IPaddr, IPaddr, IPmask);
  IPAddress myIP = WiFi.softAPIP();
  Serial.println("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  // Enable mDNS to create a local domain called "neondra.local"
  //if (!MDNS.begin("neondra")) {
  //  Serial.println("Error setting up MDNS responder!");
  //  while (1) {
  //    delay(1000);
  //  }
  //}
  //Serial.println("mDNS responder started");
}

// Loop

void LOOP_CORE_1(void) {
  // Handle ongoing mDNS operations
  //MDNS.update();
  server.handleClient();
  delay(100);
}