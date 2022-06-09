#ifndef DES_H
#define DES_H


#define ENCRYPTION_MODE 1
#define DECRYPTION_MODE 0
#define DES_KEY_SIZE 8

void generate_key(unsigned char* key);
void generate_sub_keys(unsigned char* main_key, key_set* key_sets);
void process_message(unsigned char* message_piece, unsigned char* processed_piece, key_set* key_sets, int mode);
int des(int mode, const char * input, const char * output,const char * key);

#endif
