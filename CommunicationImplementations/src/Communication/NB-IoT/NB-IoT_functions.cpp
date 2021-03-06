#include <Sodaq_nbIOT.h>
#include <NB-IoT_functions.h>
#include <Encryption_testing.h>

void print_unit8_NB(uint8_t *inputArray, int sizeOfInputArray) {
    for (int index = 0; index < sizeOfInputArray; ++index) {
        int num = inputArray[index];
        char hex[6];
        sprintf(hex, "0x%02x ", num);
        DEBUG_STREAM.print(hex);
    }
    DEBUG_STREAM.println();
}

void sendNBIoTUDP(const uint8_t *message, int messageSize, const char *targetIP,
                  int targetPort, Sodaq_nbIOT &nbiot) {
    DEBUG_STREAM.println("\n ---- sendNBIoTUDP() ----");

    int socketID = nbiot.createSocket();

    if (socketID >= 7 || socketID < 0) {
        DEBUG_STREAM.println("Failed to create socket");
        return;
    }

    DEBUG_STREAM.print("Created socket! (SocketID: ");
    DEBUG_STREAM.print(socketID);
    DEBUG_STREAM.println(")");
    DEBUG_STREAM.println("Sending UDP packet via socket");
    int lengthSent = nbiot.socketSend(socketID, targetIP, targetPort, message, messageSize);

    print_uint8(message, messageSize);
    DEBUG_STREAM.print("Amount of bytes sent: ");
    DEBUG_STREAM.println(lengthSent);
    nbiot.closeSocket(socketID);
    DEBUG_STREAM.println("Socket closed");
}

void performEncryptionNB(int encryptionType, uint8_t *plaintext, int inputSize,
                         uint8_t *ciphertextReceiver, uint8_t *Tag, uint8_t *IV) {
    /** IV initialization **/
    DEBUG_STREAM.println("Started IV generation");
    for (int index = 0; index < 16; ++index) {
        IV[index] = index;
    }


    /** Perform encryption and timings **/
    if (encryptionType == AES_GCM_ENCRYPTION)
    {
        cipher.encryption.aes_gcm_encryption(plaintext, ciphertextReceiver, Tag, inputSize, cipher.key, IV, false);
    }
    else if (encryptionType == ACORN_ENCRYPTION)
    {
        cipher.encryption.acorn_encryption(plaintext, ciphertextReceiver, Tag, inputSize, cipher.key, IV, false);
    }
    else if (encryptionType == ASCON_ENCRYPTION)
    {
        cipher.encryption.ascon_encryption(plaintext, ciphertextReceiver, Tag, inputSize, cipher.key, IV, false);
    }
}