#include <iostream>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include"obfuscate.h"
#define KEY_LENGTH       2048
#define PUBLIC_EXPONENT  59     //Public exponent should be a prime number.
#define PUBLIC_KEY_PEM   1
#define PRIVATE_KEY_PEM  0

#define LOG(x)               \
        cout << x << endl;   \

using namespace std;

RSA * read_RSA(RSA * keypair, int pem_type, char *file_name) {

    RSA   *rsa = NULL;
    BIO  *fp  = NULL;

    if(pem_type == PUBLIC_KEY_PEM) {
        fp = BIO_new_file(file_name, "rb");
        PEM_read_bio_RSAPublicKey(fp, &rsa, NULL, NULL);
        BIO_free(fp);

    }
    else if(pem_type == PRIVATE_KEY_PEM) {
    	fp = BIO_new_file(file_name, "rb");
        PEM_read_bio_RSAPrivateKey(fp, &rsa, NULL, NULL);
        BIO_free(fp);

    }

    return rsa;
}

int public_encrypt(int flen, unsigned char* from, unsigned char* to, RSA* key, int padding) {
    
    int result = RSA_public_encrypt(flen, from, to, key, padding);
    return result;
}


void create_encrypted_file(char* encrypted, RSA* key_pair) {

    FILE* encrypted_file = fopen("vpr_e.bin", "w");
    fwrite(encrypted, sizeof(*encrypted), RSA_size(key_pair), encrypted_file);
    fclose(encrypted_file);
}



void encrypt ( char* p_key,char *input_message){
    LOG("Encryption has been started.");
    RSA *public_key;
    int Block_size= 100;
    char *message= input_message;
    char *encrypt = NULL;
    RSA *keypair = RSA_new();
    char *public_key_pem  = p_key;
    public_key  = read_RSA(keypair, PUBLIC_KEY_PEM, public_key_pem);
    encrypt = (char*)malloc(RSA_size(public_key));
    int encrypt_length = public_encrypt(strlen(message) + 1, (unsigned char*)message, (unsigned char*)encrypt, public_key, RSA_PKCS1_OAEP_PADDING);
    if(encrypt_length == -1) {
        LOG("An error occurred in public_encrypt() method");
    }
    else{
        LOG("Data has been encrypted.");
        create_encrypted_file(encrypt, public_key);
        LOG("Encrypted file has been created.");
    }
}
