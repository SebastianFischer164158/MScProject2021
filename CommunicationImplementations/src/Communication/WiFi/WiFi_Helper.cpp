#include <Arduino.h>
#include <Encryption_testing.h>
#include <WiFiUdp.h>
#include <WiFi.h>


void performEncryption(int encryptionType, uint8_t *plaintext,
                       int inputSize,
                       uint8_t *ciphertextReceiver,
                       uint8_t *Tag,
                       uint8_t *IV) {
    /** IV initialization **/
    GenerateInitializationVector(IV, 16);

    /** Perform encryption and timings **/
    if (encryptionType == 1)
    {
        cipher.encryption.aes_gcm_encryption(plaintext, ciphertextReceiver, Tag, inputSize, cipher.key, IV, false);
    }
    else if (encryptionType == 2)
    {
        cipher.encryption.acorn_encryption(plaintext, ciphertextReceiver, Tag, inputSize, cipher.key, IV, false);
    }
    else if (encryptionType == 3)
    {
        cipher.encryption.ascon_encryption(plaintext, ciphertextReceiver, Tag, inputSize, cipher.key, IV, false);
    }
}

void performDecryption(uint8_t *ciphertext,
                       uint8_t *Tag,
                       uint8_t *IV,
                       int ciphertextSize) {
    /** Plaintext buffer **/
    uint8_t plaintextReceiver[ciphertextSize];
    char text[ciphertextSize];

    /** Perform decryption and timing of the following **/
    cipher.decryption.aes_gcm_decryption(ciphertext, plaintextReceiver, Tag, ciphertextSize, cipher.key, IV, false);

    Serial.print("IV: ");
    print_uint8(IV, 16);

    Serial.print("Tag: ");
    print_uint8(Tag, 16);

    Serial.print("Ciphertext: ");
    print_uint8(ciphertext, ciphertextSize - 32);

    uint8ToChar(plaintextReceiver, text, ciphertextSize-32);
    Serial.print("Decrypted Text: ");
    print_char(text, ciphertextSize-32);
}

void Build_And_Send_UDP_Packet(WiFiUDP &WiFiUDP,
                               char const *msg,
                               IPAddress &address,
                               int UDP_Port_Number) {
    WiFiUDP.beginPacket(address, UDP_Port_Number);
    WiFiUDP.print(msg);
    WiFiUDP.endPacket();
}

void Build_And_Send_UDP_Packet(WiFiUDP &WiFiUDP, const uint8_t *msg, int msg_size,
                               IPAddress &address, int UDP_Port_Number) {
    WiFiUDP.beginPacket(address, UDP_Port_Number);

    char tempContainer[msg_size];
    for (int index = 0; index < msg_size; index++)
    {
        tempContainer[index] = (char) msg[index];
    }

    WiFiUDP.print(tempContainer);
    WiFiUDP.endPacket();
}

String Receive_UDP_Packet(WiFiUDP &WiFiUDP, char packet_buffer[]){
    String tmp = "";
    while(tmp == ""){
        int length = WiFiUDP.parsePacket();
        while(WiFiUDP.read(packet_buffer, length) > 0){
            WiFiUDP.read(packet_buffer, length);
            delay(20);
        }
        tmp = packet_buffer;
    }
    return tmp;
}

void Connect_To_AP(const char *SSID, WiFiClass &WiFi_instance) {
    Serial.println("Connecting to the Access Point: ");
    Serial.println(SSID);

    WiFi_instance.begin(SSID);
    while (WiFiClass::status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Connecting to WiFi_instance..");
    }

    Serial.println("Connected to WiFi_instance");
    Serial.print("IP Address: ");
    Serial.println(WiFi_instance.localIP());
    Serial.print("MAC Address: ");
    Serial.println(WiFi_instance.macAddress());
}
