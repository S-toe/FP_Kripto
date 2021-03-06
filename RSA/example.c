#include <stdio.h>
#include "rsa.h"
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  struct public_key_class pub[1];
  struct private_key_class priv[1];
  //Buat Private Key dan Public Key dahulu
  rsa_gen_keys(pub, priv, PRIME_SOURCE_FILE);
  // pub->exponent=9126805504;
  // pub->modulus=65536;
  // priv->exponent=109877395456;
  // priv->modulus=110058536960;
  printf("Private Key:\n Modulus: %lld\n Exponent: %lld\n", (long long)priv->modulus, (long long) priv->exponent);
  printf("Public Key:\n Modulus: %lld\n Exponent: %lld\n", (long long)pub->modulus, (long long) pub->exponent);
  
  char message[] = "12345678";
  int i;

  printf("Original:\n");
  for(i=0; i < strlen(message); i++){
    printf("%lld\n", (long long)message[i]);
  }  
  // printf("Original:\n");
  // for(i=0; i < strlen(message); i++){
  //   printf("%lld", (long long)message[i]);
  // } 
  printf("\n");
  long long *encrypted = rsa_encrypt(message, sizeof(message), pub);
  
  if (!encrypted){
    fprintf(stderr, "Error in encryption!\n");
    return 1;
  }
  printf("Encrypted:\n");
  for(i=0; i < strlen(message); i++){
    printf("%c", encrypted[i]);
  }  

  printf("\nEncrypted:\n");
  for(i=0; i < strlen(message); i++){
    printf("%lld ", encrypted[i]);
  }  
  printf("\n");
  
  char *decrypted = rsa_decrypt(encrypted, 8*sizeof(message), priv);
  if (!decrypted){
    fprintf(stderr, "Error in decryption!\n");
    return 1;
  }
  printf("\nDecrypted:\n");
  for(i=0; i < strlen(message); i++){
    printf("%c", decrypted[i]);
  }  
  
  printf("\n");
  free(encrypted);
  free(decrypted);
  return 0;
}