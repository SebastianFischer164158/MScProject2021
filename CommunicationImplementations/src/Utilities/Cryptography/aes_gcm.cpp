#include <Arduino.h>
#include <AES.h>
#include <GCM.h>

bool aes_gcm_encryption(uint8_t *plaintext, uint8_t *ciphertext, uint8_t *tag,
                        int size, uint8_t *key, uint8_t *iv, int cipherSize) {

    /** Initiate the Acorn cipher **/
    GCM<AES128> gcmAesEncryption;

    /** It is important to clear the key and iv before usage, ensures no strange rotations are affecting the encryption process **/
    gcmAesEncryption.clear();
    gcmAesEncryption.setKey(key, cipherSize);
    gcmAesEncryption.setIV(iv, cipherSize);

    /** Clear memory address values before using it. This ensures no weird values suddently appearing **/
    memset(tag, 0xBA, cipherSize);
    memset(ciphertext, 0xBA, size);

    /** Perform the encryption og compute the tag **/
    unsigned long beforeEncryption = micros();
    gcmAesEncryption.encrypt(ciphertext, plaintext, size);
    unsigned long afterEncrypt = micros() - beforeEncryption;

    Serial.print("Message size: ");
    Serial.println(size);
    Serial.print(float(afterEncrypt) / float(size));
    Serial.println(" micro seconds encryption timing");

    gcmAesEncryption.computeTag(tag, size);
    return true;
}

bool aes_gcm_decryption(uint8_t *ciphertext, uint8_t *plaintext, uint8_t *tag,
                        int size, uint8_t *key, uint8_t *iv, int cipherSize) {

    /** Initiate the Acorn cipher **/
    GCM<AES128> cipherDecryption;

    /** It is important to clear the key and iv before usage, ensures no strange rotations are affecting the decryption process **/
    cipherDecryption.clear();
    cipherDecryption.setKey(key, cipherSize);
    cipherDecryption.setIV(iv, cipherSize);

    /** Clear memory address values before using it. This ensures no weird values suddently appearing **/
    memset(plaintext, 0xBA, size);

    /** Perform the decryption **/
    unsigned long beforeDecryption = micros();
    cipherDecryption.decrypt(plaintext, ciphertext, size);
    unsigned long afterDecrypt = micros() - beforeDecryption;

    Serial.print(float(afterDecrypt) / float(size));
    Serial.println(" micro seconds decryption timing");

    /** Validate the received tag **/
    if (!cipherDecryption.checkTag(tag, cipherSize)) {
        return false;
    }

    return true;
}