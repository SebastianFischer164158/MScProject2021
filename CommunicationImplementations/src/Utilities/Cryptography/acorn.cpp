#include <Acorn128.h>
#include <Arduino.h>

#define POINTITERATION 50

bool acorn_encryption(uint8_t *plaintext, uint8_t *ciphertext, uint8_t *tag,
                      int size, uint8_t *key, uint8_t *iv, int cipherSize) {
    /** Manually change if the method is not used for timing **/
    bool timingRequired = true;

    /** Initiate the Acorn cipher **/
    Acorn128 cipher;

    /** It is important to clear the key and iv before usage, ensures no strange rotations are affecting the encryption process **/
    cipher.clear();
    cipher.setKey(key, cipherSize);
    cipher.setIV(iv, cipherSize);

    /** Clear memory address values before using it. This ensures no weird values suddently appearing **/
    memset(tag, 0xBA, cipherSize);
    memset(ciphertext, 0xBA, size);

    cipher.encrypt(ciphertext, plaintext, size);

    if (timingRequired) {
        unsigned long start;
        unsigned long elapsed;
        int count;
        Serial.println("acorn-encryption,");
        /** Perform the encryption og compute the tag **/
        for (int graphIteration = 0; graphIteration < POINTITERATION; graphIteration++){
            start = micros();
            for (count = 0; count < 500; ++count) {
                cipher.encrypt(ciphertext, plaintext, size);
            }
            elapsed = micros() - start;

            Serial.print(elapsed / (size * 500.0));
            Serial.println(",");
        }
    }


    cipher.computeTag(tag, size);

    return true;
}

bool acorn_decryption(uint8_t *ciphertext, uint8_t *plaintext, uint8_t *tag,
                      int size, uint8_t *key, uint8_t *iv, int cipherSize) {
    /** Manually change if the method is not used for timing **/
    bool timingRequired = true;

    /** Initiate the Acorn cipher **/
    Acorn128 cipher;

    /** It is important to clear the key and iv before usage, ensures no strange rotations are affecting the decryption process **/
    cipher.clear();
    cipher.setKey(key, cipherSize);
    cipher.setIV(iv, cipherSize);

    /** Clear memory address values before using it. This ensures no weird values suddently appearing **/
    memset(plaintext, 0xBA, size);

    /** Perform the decryption **/
    cipher.decrypt(plaintext, ciphertext, size);

    if (timingRequired) {
        unsigned long start;
        unsigned long elapsed;
        int count;
        Serial.println("acorn-decryption,");
        /** Perform the encryption og compute the tag **/
        for (int graphIteration = 0; graphIteration < POINTITERATION; graphIteration++){
            start = micros();
            for (count = 0; count < 500; ++count) {
                cipher.decrypt(plaintext, ciphertext, size);
            }
            elapsed = micros() - start;

            Serial.print(elapsed / (size * 500.0));
            Serial.println(",");
        }
    }

    /** Validate the received tag **/
    if (!cipher.checkTag(tag, cipherSize)) {
        return false;
    }

    return true;
}
