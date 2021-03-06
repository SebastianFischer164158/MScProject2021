#ifndef MSCPROJECT2021_UTILITIES_H
#define MSCPROJECT2021_UTILITIES_H
// include guards

void print_uint8(const uint8_t *input, int size);
void print_char(char *input, int size);

void copy_uint8(const uint8_t *input, uint8_t *output, int size);

void charToUint8(const char *input, uint8_t *plaintext, int size);
void uint8ToChar(const uint8_t *plaintext, char *toText, int size);

void AssembleAuthenticatedEncryptionPacket(const uint8_t *iv, const uint8_t *tag, int defaultSize, const uint8_t *ciphertextReceiver, uint8_t *packetBuffer, int packetSize);
void DisassembleAuthenticaedEncryptionPacket(uint8_t *iv, uint8_t *tag, int defaultSize, uint8_t *ciphertext,
                                             const uint8_t *packetBuffer, int packetSize);

void GenerateInitializationVector(uint8_t *IV, int size);
#endif


