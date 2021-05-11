#include <Arduino.h>

int random(int min, int max);
void recvWithStartEndMarkers();
void showNewData();
void sendWithStartEndMarkers(String data);
String addStartEndMarkers(String input);
String checkUnoData();
String getTemperature();
String getHumidity();
String getSoilMoisture(int sensorNumber);
String processor(const String& var);