#ifndef MSCPROJECT2021_AES_GCM_H
#define MSCPROJECT2021_AES_GCM_H

bool aes_gcm_encryption(uint8_t *plaintext,
                        uint8_t *ciphertext,
                        uint8_t *Tag,
                        int size,
                        uint8_t *key,
                        uint8_t *IV,
                        bool timingRequired = false);

bool aes_gcm_decryption(uint8_t *ciphertext,
                        uint8_t *plaintext,
                        uint8_t *Tag,
                        int size,
                        uint8_t *key,
                        uint8_t *IV,
                        bool timingRequired = false);

#endif //MSCPROJECT2021_AES_GCM_H
