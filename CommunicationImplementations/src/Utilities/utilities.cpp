#include <Arduino.h>
#include <RNG.h>
#include <TransistorNoiseSource.h>
#include <utilities.h>
#include <Encryption_testing.h>

/** Print each value of uint8 array **/
void print_uint8(const uint8_t *input, int size) {
    for(int i = 0; i < size; i++){
        printf("0x%02x ", input[i]);
        fflush(stdout);
    }
    Serial.println();
}

/** Print each value of char array **/
void print_char(char *input, int size) {
    for (int index = 0; index < size; ++index) {
        printf("%c", input[index]);
        fflush(stdout);
    }
    Serial.println();
}

/** Copy one uint8 array to another uint8 array **/
void copy_uint8(const uint8_t *input, uint8_t *output, int size) {
    for(int i = 0; i < size; i++){
        output[i] = input[i];
    }
}

/** Converts char array to uint8 array **/
void charToUint8(const char *input, uint8_t *plaintext, int size) {
    for (int index = 0; index < size; ++index) {
        plaintext[index] = input[index];
    }
}

/** Converts uint8 array to char array **/
void uint8ToChar(const uint8_t *plaintext, char *toText, int size) {
    for (int index = 0; index < size; ++index) {
        toText[index] = plaintext[index];
    }
}

/** Converts uint8 array to string **/
std::string uint8ToString(const uint8_t *plaintext, int size) {
    std::string value = "";
    for (int index = 0; index < size; ++index) {
        value += plaintext[index];
    }

    return value;
}

/** Encapsulate iv, tag and ciphertext to one packet **/
void AssembleAuthenticatedEncryptionPacket(const uint8_t *iv, const uint8_t *tag, int defaultSize,
                                           const uint8_t *ciphertextReceiver, uint8_t *packetBuffer,
                                           int packetSize) {
    memset(packetBuffer, 0xBA, packetSize);

    for (int index = 0; index < packetSize; ++index) {
        if (index < defaultSize){
            packetBuffer[index] = iv[index];
        }
        else if (index >= defaultSize && index < defaultSize*2)
        {
            packetBuffer[index] = tag[index - defaultSize];
        }
        else {
            packetBuffer[index] = ciphertextReceiver[index - defaultSize*2];
        }
    }
}
/** Dissameble the IV, tag and ciphertext to three different pointers**/
void DisassembleAuthenticaedEncryptionPacket(uint8_t *iv, uint8_t *tag, int defaultSize, uint8_t *ciphertext,
                                             const uint8_t *packetBuffer, int packetSize) {
    memset(iv, 0xBA, defaultSize);
    memset(tag, 0xBA, defaultSize);
    memset(ciphertext, 0xBA, packetSize-(2*defaultSize));

    for (int index = 0; index < packetSize; ++index) {
        if (index < defaultSize){
            iv[index] = packetBuffer[index];
        }
        else if (index >= defaultSize && index < defaultSize*2)
        {
            tag[index - defaultSize] = packetBuffer[index];
        }
        else {
            ciphertext[index - defaultSize*2] = packetBuffer[index];
        }
    }
}
/** Genereates a random IV from Noise Source **/
void GenerateInitializationVector(uint8_t *IV, int size) {
    TransistorNoiseSource noise(A0);
    RNG.begin("RANDOM_NUMB_GEN");
    RNG.addNoiseSource(noise);

    bool IsKeyGenerated = false;
    while (!IsKeyGenerated){
        RNG.loop();
        if(RNG.available(size)){
            RNG.rand(IV, size);
            IsKeyGenerated = true;
        }
    }
}


void performEncryption(uint8_t *plaintext, int inputSize,
                       uint8_t *ciphertextReceiver, uint8_t *tag, uint8_t *iv) {
    /** IV initialization **/
    GenerateInitializationVector(iv, 16);

    /** Perform encryption and timings **/
    cipher.encryption.acorn_encryption(plaintext, ciphertextReceiver, tag, inputSize, cipher.key, iv, SIZE);

    uint8_t plaintextReceiver[20];
    char text[20];

    cipher.decryption.acorn_decryption(ciphertextReceiver, plaintextReceiver, tag, inputSize, cipher.key, iv, SIZE);

    uint8ToChar(plaintextReceiver, text, inputSize);
    Serial.print("Decrypted Text: ");
    print_char(text, 20);
}

void performDecryption(uint8_t *ciphertext, uint8_t *tag, uint8_t *iv) {
    /** Plaintext buffer **/
    uint8_t plaintextReceiver[20];
    char text[20];

    /** Perform decryption and timing of the following **/
    cipher.decryption.acorn_decryption(ciphertext, plaintextReceiver, tag, 20, cipher.key, iv, SIZE);

    uint8ToChar(plaintextReceiver, text, 20);
    Serial.print("Decrypted Text: ");
    print_char(text, 20);
}

