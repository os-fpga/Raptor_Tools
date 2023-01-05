#include"obfuscate.h"
#include "function.h"
#include "openssl_rsa.h"
#define AES_BLOCK_SIZE 256
const int BUFSIZE = 4096;

//using namespace std;

const string default_encryption_cipher_ = "aes";
const int default_keysize_ = 256;
const int default_blocksize_ = 128;
const string default_encryption_mode_ = "cbc";
const unsigned   default_pbkdf2_iterations_ = 1000;
const unsigned   default_pbkdf2_saltlen_ = 8;

class Enc_Dec {
public:
	unsigned char *key = NULL, *iv=NULL; 
	Enc_Dec (const string &sourcefile, const string &destfile, char* input_pp){
	OpenSSL_add_all_algorithms();
    string passphrase = "OnE dAy RaPiDsilc@N wIll bE GrE@9 CAMp!!?";
    char *message = new char[passphrase.length() + 1];
    strcpy(message, passphrase.c_str());
    encrypt(input_pp,message);

	key =key_generation(passphrase,default_keysize_, default_pbkdf2_iterations_,default_pbkdf2_saltlen_ );
	encrypt_file(sourcefile,destfile);
	 }
private:

int encrypt_file(const string &sourcefile,const string &destfile)
{
    int rc = 1;
    int i;
    EVP_CIPHER_CTX *enc_ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(enc_ctx);
    char ciphername[100] = {0};
    const EVP_CIPHER *ciph;
    unsigned char inbuf[BUFSIZE], outbuf[BUFSIZE + AES_BLOCK_SIZE<<1]; // specific to AES
    fstream ofile;
    ifstream ifile;
    int bytes_read, bytes_encrypted,
        total_bytes_read = 0, total_bytes_encrypted = 0;
    
    // 1. Open input file
    ifile.open(sourcefile.c_str(), ios::in | ios::binary);
    if (!ifile.is_open()) {
        cerr << "Cannot open input file " << sourcefile << endl;
        return rc;
    }
    
    // 2. Check that output file can be opened and written
    ofile.open(destfile.c_str(), ios::out | ios::binary | ios::trunc);
    if (!ofile.is_open()) {
        cerr << "Cannot open input file " << sourcefile << endl;
        return rc;
    }
    // 4. Initialize encryption engine / context / etc.

    snprintf(ciphername, 99, "%s-%d-%s",
             default_encryption_cipher_.c_str(),
             default_keysize_, default_encryption_mode_.c_str());
    if (!(ciph = EVP_get_cipherbyname(ciphername))) {
        cerr << "Cannot find algorithm " << ciphername << endl;
        goto free_data;
    }
    //EVP_CIPHER_CTX_init(&enc_ctx);
    if (!EVP_EncryptInit_ex(enc_ctx, ciph, NULL, key, iv)) {
        cerr << "Cannot initialize encryption cipher " << ciphername << endl;
        goto free_data;
    }
    
    // 5.2 Read source file block, encrypt, and write to output stream
    while (!ifile.eof()) {
        ifile.read((char*)inbuf, BUFSIZE);
        bytes_read = (int) ifile.gcount(); // cast okay because BUFSIZE < MAX_INT
        if (bytes_read > 0) {
            if (!EVP_EncryptUpdate(enc_ctx, outbuf, &bytes_encrypted,
                                  inbuf, bytes_read)) {
                cerr << "Error encrypting chunk at byte "
                    << total_bytes_encrypted << endl;
                goto free_data;
            }
//            assert(bytes_encrypted > 0);
            if (bytes_encrypted > 0)
                ofile.write((char*)outbuf, bytes_encrypted);
            
            total_bytes_read += bytes_read;
            total_bytes_encrypted += bytes_encrypted;
        }
        bytes_read = bytes_encrypted = 0;
    }
    // 5.3 Encrypt and write final block of input
    EVP_EncryptFinal_ex(enc_ctx, outbuf, &bytes_encrypted);
    if (bytes_encrypted > 0) {
        ofile.write((char*)outbuf, bytes_encrypted);
    }
    printf("\n File Encrypted\n");
    // 6. cleanup
    ifile.close();
    ofile.close();
    rc = 0;
free_data:
    delete [] key;
    delete [] iv;
    return rc;
}

int usage(const char *programname) 
{
    cerr << "Usage: " << programname << ": Enter the input file to encrypt and output file to write and the public key " << endl;
    return 1;
}
};

int usage(const char *programname)
{
    cerr << "Usage: " << programname << ": Enter the input file to encrypt and output file to write and the public key " << endl;
    return 1;
}

int main(int argc, char *argv[]) 
{
   //Atleast 4 arguments are needed
   // arg[0]  is the binary
   // arg [1] is the number of files
   // arg [2] is the input file for the encryption
   // the rest are the files to be encrypted
   int numb ;
   sscanf(argv[1], "%d", &numb);
   printf (" The number of input files are : %d\n", numb);
   int check_numb = numb + 3;
    if (argc < 4) {
        return usage(argv[0]);
    }
    else if (argc != check_numb)
    {
         //cerr << "Usage: " The number of file to be increpted is not equal to number of inputs " << endl;
    return 1;
    }
    else
    {
        for (int i = 0; i< numb; i++)
        {
            int str_size = strlen(argv[i+3]);
            char* outputfile = new char [str_size];
            strcpy (outputfile, argv[i+3]);
            char e_add = 'e';
            strncat (outputfile, &e_add, 1) ;
             printf (" The input file name is : %s \n The output file name is %s \n", argv[i+3], outputfile);
            Enc_Dec E1 (argv[i+3],outputfile, argv[2]);
        }
    }    
    return 0;
}


