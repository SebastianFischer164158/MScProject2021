#include <WiFi.h>
#include <WiFiUdp.h>
#include <WiFi_Helper.h>
#include <cstring>
#include <utilities.h>

WiFiUDP UDP;
//WiFi information for the setup.
#define AP_CHANNEL 1
#define AP_CLOACK 0
#define AP_MAX_CON 5
//#define AP_PASSWORD ""

char UDP_RECEIVER_BUFFER[350];

void Create_Open_AccessPoint(const char *SSID,
                             int WiFi_Channel,
                             int Hide_Access_Point,
                             int Max_Number_of_Connections) {

    WiFi.softAP(SSID,nullptr,WiFi_Channel,Hide_Access_Point,Max_Number_of_Connections);
    Serial.println("Assigned IP address of Access Point: ");
    Serial.println(WiFi.softAPIP());
    Serial.print("MAC address: ");
    Serial.println(WiFi.macAddress());
}

/** Wait in while-loop for a client connecting to the WiFi **/
void Wait_For_Client_Connection(){
    Serial.println("Waiting for clients to connect!");
    while(WiFi.softAPgetStationNum() <= 0){
        delay(50);
    }
    Serial.println("Client connected! ");
    Serial.println(WiFi.softAPgetStationNum());
    Serial.println();
}

void setup() {
    Serial.begin(115200);
    Create_Open_AccessPoint(AP_SSID, AP_CHANNEL, AP_CLOACK, AP_MAX_CON);
    UDP.begin(UDP_PORT);
    Wait_For_Client_Connection();
}

bool encrypted = false;

void loop(){
    Serial.println();
    String received_message_UDP = Receive_UDP_Packet(UDP, UDP_RECEIVER_BUFFER);

    if (ENABLE_ENCRYPTION){
        /** Encryption **/
        Serial.println("------------------------------------");
        int responseSize = (int) received_message_UDP.length();

        Serial.print("Response size: ");
        Serial.println(responseSize);

        uint8_t ciphertext[responseSize];
        uint8_t buffer[responseSize];
        Serial.println("STEP1");
        for (int index = 0; index < responseSize; index++)
        {
            buffer[index] = received_message_UDP[index];
        }
        Serial.println("STEP2");
        delay(10);
        DisassembleAuthenticaedEncryptionPacket(IV, Tag, 16, ciphertext, buffer, responseSize);
        Serial.println("STEP3");
        delay(10);
        performDecryption(ciphertext, Tag, IV, responseSize);
        Serial.println("STEP4");
        Serial.println();
        memset(buffer, 0, sizeof(buffer));
        Serial.println("STEP5");
    } else {
        String output_format = "Received UDP packet from ["+UDP.remoteIP().toString()+":"
                               + String(UDP.remotePort()) + "]" + " - Message: ";
        Serial.println(output_format);
        Serial.println(received_message_UDP);
    }

    delay(10);
    memset(UDP_RECEIVER_BUFFER, 0, sizeof(UDP_RECEIVER_BUFFER));

}

