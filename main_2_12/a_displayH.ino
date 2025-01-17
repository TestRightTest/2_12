#include <WiFi.h>

#define numChannels 16

bool timeExtension = false;
char ssid[50];
char password[50];

bool isWifiConnected = false;

int rotationIntervalMinutes;//Time extention rotation interval
const int MAX_DATA_SIZE = 500; // Adjust the size according to your needs
char incomingData[MAX_DATA_SIZE];

unsigned long channelStartTimes[numChannels] = {0};
String lastElapsedTime[numChannels] = {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};

int selectedChannel = 0; // Variable to store the channel number of the last received address from 0x5000 to 0x50F0
bool isRunning[numChannels]; // Array to store whether each channel is started or not
bool isCompleted[numChannels];

int timerRotation = 10; // Initial countdown value
uint32_t previousRotationEpoch = 0;
uint32_t previousCountdownEpoch = 0;

int serialNum;
String deviceID;
//String helpline = "+91 85880 06453"; fetch_mac();

uint16_t mbrt[numChannels];

uint16_t SPwifiIcon = 0x8B30;
uint16_t wifiIcon = 0x22C0;

uint16_t deviceIDadd = 0x2180;
uint16_t helplineadd = 0x2220;

uint16_t ssidAdd = 0x2040;
uint16_t passAdd = 0x20E0;

uint16_t rotationIntervalAdd = 0x2470;
uint16_t setTempAdd = 0x2480;
uint16_t firmwareAdd = 0x2460;

uint16_t rotationMsg = 0x2330;

uint16_t countdownAdd = 0x2300;

uint16_t logBoxAdd = 0x2390;

uint16_t timeAdd = 0x25D6;
//uint16_t dateAdd = 0x25EA;
uint16_t temperatureAdd = 0x1010;

unsigned long previousMillisTemp = 0;
const long intervalTemp = 15000;  // 15 seconds

uint16_t startArray[] = {
  0x1020, 0x1040, 0x1060, 0x1080,
  0x10A0, 0x10C0, 0x10E0, 0x1100,
  0x1120, 0x1140, 0x1160, 0x1180,
  0x11A0, 0x11C0, 0x11E0, 0x1200
};

uint16_t channelIdArray[] = {
  0x1220, 0x1230, 0x1240, 0x1250,
  0x1260, 0x1270, 0x1280, 0x1290,
  0x12A0, 0x12B0, 0x12C0, 0x12D0,
  0x12E0, 0x12F0, 0x1300, 0x1310
};

uint16_t nameArray[] = {
  0x1320, 0x1360, 0x13A0, 0x13E0,
  0x1420, 0x1460, 0x14A0, 0x14E0,
  0x1520, 0x1560, 0x15A0, 0x15E0,
  0x1620, 0x1660, 0x16A0, 0x16E0
};

uint16_t timeArray[] = {
  0x1730, 0x1740, 0x1750, 0x1760,
  0x1770, 0x1780, 0x1790, 0x17A0,
  0x17B0, 0x17C0, 0x17D0, 0x17E0,
  0x17F0, 0x1800, 0x1810, 0x1820
};

uint16_t SPtimeArray[] = {
  0x8A30, 0x8A40, 0x8A50, 0x8A60,
  0x8A70, 0x8A80, 0x8A90, 0x8AA0,
  0x8AB0, 0x8AC0, 0x8AD0, 0x8AE0,
  0x8AF0, 0x8B00, 0x8B10, 0x8B20
};

uint16_t progressArray[] = {
  0x1830, 0x1840, 0x1850, 0x1860,
  0x1870, 0x1880, 0x1890, 0x18A0,
  0x18B0, 0x18C0, 0x18D0, 0x18E0,
  0x18F0, 0x1900, 0x1910, 0x1920
};

uint16_t SPprogressArray[] = {
  0x8830, 0x8840, 0x8850, 0x8860,
  0x8870, 0x8880, 0x8890, 0x88A0,
  0x88B0, 0x88C0, 0x88D0, 0x88E0,
  0x88F0, 0x8900, 0x8910, 0x8920
};

uint16_t channelButtonAddresses[] = {
  0x5000, 0x5010, 0x5020, 0x5030,
  0x5040, 0x5050, 0x5060, 0x5070,
  0x5080, 0x5090, 0x50A0, 0x50B0,
  0x50C0, 0x50D0, 0x50E0, 0x50F0
};

uint16_t SPprogressbar[] = {
  0x8930, 0x8940, 0x8950, 0x8960,
  0x8970, 0x8980, 0x8990, 0x89A0,
  0x89B0, 0x89C0, 0x89D0, 0x89E0,
  0x89F0, 0x8A00, 0x8A10, 0x8A20
};

String startChannelData[16];
String completedChannelData[16];
String stopedChannelData[16];

// Declare a variable to track the current screen
int currentScreen = 0;

int rotationInterval = 15; // Default rotation interval
const int minRotationInterval = 5;
const int maxRotationInterval = 99;

uint32_t testCount = 0;  // Initialize all counters to zero
uint32_t latestTestCounts[16] = {0};

unsigned long closeLidStartTime = 0;

uint8_t rotationAngle = 62;
