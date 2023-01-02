/*
 *
 * (c) Copyright 1999 - 2022 Verific Design Automation Inc.
 * All rights reserved.
 *
 * This source code belongs to Verific Design Automation Inc.
 * It is considered trade secret and confidential, and is not to be used
 * by parties who have not received written authorization
 * from Verific Design Automation Inc.
 *
 * Only authorized users are allowed to use, copy and modify
 * this software provided that the above copyright notice
 * remains in all copies of this software.
 *
 *
*/

#include <iostream>
#include <fstream>
#include <cstring> // For memset and memcpy
#include <sstream> // For istringstream

#include "Strings.h"
#include "Array.h"
#include "VerificStream.h"

#include "ieee_1735.h"
#include "obfuscate.h"

#define PADDING RSA_PKCS1_PADDING

#ifdef VERIFIC_NAMESPACE
using namespace Verific ;
#endif

ieee_1735::ieee_1735() : Protect(BLOCK_SIZE)
{
    // Generate a session key for encryption
    for (int i=0 ; i<BLOCK_SIZE ; i++) {
        _session_key[i] = (std::rand() % 256) ;
    }
    _session_key[BLOCK_SIZE] = '\0' ;
}

unsigned
ieee_1735::encrypt(const char *in_buf, char **out_buf, unsigned in_size)
{
    // Generate pseudo-random initialization vector
    char iv[BLOCK_SIZE + 1] ;
    for (int i=0 ; i<BLOCK_SIZE ; i++) {
        iv[i] = (std::rand() % 256) ;
    }
    iv[BLOCK_SIZE] = '\0' ;

    // Prepend iv to in_buf
    char *msg = Strings::save((const char *) &iv, in_buf) ;

    // Encrypt data_block
    unsigned encrypted_len ;
    char *out_binary = EncryptAES_128_CBC(msg, (byte *) _session_key, (byte *) iv, &encrypted_len) ;
    Strings::free(msg) ;
    if (!out_binary) return 0 ;

    // Encode data using base64
    char *out_b64 = EncodeBase64(out_binary, encrypted_len) ;
    Strings::free(out_binary) ;

    // Break long line into 64 character lines
    *out_buf = InsertNewlines(out_b64, 64) ;
    Strings::free(out_b64) ;

    return Strings::len(*out_buf) ;
}

char *
ieee_1735::GetEncryptionHeader()
{
    // Return the encryption header that will be writen before the encrypted text.
    Array a(0) ;

    if (IsVhdl()) {
        a.InsertLast(Strings::save("`protect begin_protected\n")) ;
    } else {
        a.InsertLast(Strings::save("`pragma protect begin_protected\n")) ;
    }

    const char * const directives[] = {
        "verision", "encrypt_agent", "encrypt_agent_info", "author", "author_info",
        "data_method", "key_keyowner", "key_keyname", "key_method"} ;

    const char *prefix = (IsVhdl()) ? "`protect " : "`pragma protect " ;
    for (unsigned i=0 ; i<9 ; i++) {
        const char *value = GetDirectiveValue(directives[i]) ;
        if (!value) continue ;
        a.InsertLast(Strings::save(prefix, directives[i], " = ", value, "\n")) ;
    }

    // Get public key from directives
    char *public_key = FormatPublicKey(GetDirectiveValue("key_public_key")) ;
    BIO *keybio = BIO_new_mem_buf((byte *) public_key, -1) ;
    VERIFIC_ASSERT(keybio) ;
    RSA *rsa_public  = PEM_read_bio_RSA_PUBKEY(keybio, NULL, NULL, NULL) ;
    if (!rsa_public) {
        std::cerr << "Error: unable to load public key" << std::endl ;
        return 0 ;
    }

    // Encrypt session key using RSA
    char *key_block_binary = Strings::allocate(RSA_size(rsa_public)) ;
    unsigned encrypted_len = \
        RSA_public_encrypt(BLOCK_SIZE, (const byte *) _session_key, (byte *) key_block_binary,
                           rsa_public, PADDING) ;
    if (encrypted_len <= 0) {
        std::cerr << "Error: unable to encrypt key block (length=" << encrypted_len
                  << ")" << std::endl ;
        return 0 ;
    }
    RSA_free(rsa_public);
    BIO_free(keybio);
    Strings::free(public_key);

    // Base64 encode encrypted session key
    char *key_block_b64 = EncodeBase64(key_block_binary, encrypted_len) ;
    Strings::free(key_block_binary) ;
    char *key_block_b64_w_newlines = InsertNewlines(key_block_b64, 64) ;
    Strings::free(key_block_b64) ;

    // Add key_block info and actual value to encryption header
    a.InsertLast(Strings::save(prefix, "encoding = (enctype = \"base64\", line_length = 64,"
                                       " bytes = 128), key_block\n")) ;
    a.InsertLast(key_block_b64_w_newlines) ;

    // Add data_block info to encryption header (actual value added by encrypt)
    a.InsertLast(Strings::save("\n", prefix, "encoding = (enctype = \"base64\", line_length = 64,"
                                       " bytes = 128), data_block")) ;

    char *header = Strings::save(a) ;
    unsigned i ;
    char *s;
    FOREACH_ARRAY_ITEM(&a, i, s) {
        Strings::free(s) ;
    }
    return header ;
}

char *
ieee_1735::GetEncryptionFooter()
{
    // Return the encryption footer (written after the encrypted text).
    return Strings::save((IsVhdl()) ? "`protect end_protected" : "`pragma protect end_protected") ;
}

char *
ieee_1735::decrypt(void)
{
    std::ifstream f(PRIVATE_KEY_FILENAME) ;
    if (!f.is_open()) {
        std::cerr << "Error: unable to open private key file: "
                  << PRIVATE_KEY_FILENAME << std::endl ;
        return 0 ;
    }
    std::string rsa((std::istreambuf_iterator<char>(f)),
                     std::istreambuf_iterator<char>()) ;
    f.close() ;

    BIO *keybio = BIO_new_mem_buf((byte *) rsa.data(), -1) ;
    if (keybio == NULL) {
        std::cerr << "Error: unable to create key bio" << std::endl ;
        return 0 ;
    }
    RSA *rsa_private = PEM_read_bio_RSAPrivateKey(keybio, NULL, NULL, NULL) ;
    if (!rsa_private) {
        std::cerr << "Error: unable to parse private key" << std::endl ;
        return 0 ;
    }

    /////////////////////////////////////////////////////////////////////////
    // Get key_block and use RSA decryption to get the original session_key
    /////////////////////////////////////////////////////////////////////////
    const char *key_method = GetVerifyDirectiveValue("key_method", "\"rsa\"") ;
    if (!key_method) return 0 ;

    const char *key_block_b64_newlines = GetVerifyDirectiveValue("key_block") ;
    if (!key_block_b64_newlines) return 0 ;
    char *key_block_b64 = RemoveNewlines(key_block_b64_newlines) ;
    unsigned decoded_len ;
    char *key_block = DecodeBase64(key_block_b64, &decoded_len) ;
    Strings::free(key_block_b64) ;
    char *session_key = Strings::allocate(RSA_size(rsa_private)) ;
    std::memset(session_key, '\0', RSA_size(rsa_private)) ;
    unsigned decrypted_len = \
        RSA_private_decrypt(decoded_len, (const byte *) key_block, (byte *) session_key,
                            rsa_private, PADDING) ;
    Strings::free(key_block) ;
    RSA_free(rsa_private) ;
    BIO_free(keybio) ;
    if (decrypted_len != BLOCK_SIZE) {
        std::cerr << "Error: unable to decrypt session key." << std::endl ;
        return 0 ;
    }
    VERIFIC_ASSERT(session_key[decrypted_len] == '\0') ;

    /////////////////////////////////////////////////////////////
    // Base64 decode and then AES decrypt data_block
    /////////////////////////////////////////////////////////////
    if (!GetVerifyDirectiveValue("encoding enctype", "\"base64\"")) return 0 ;
    if (!GetVerifyDirectiveValue("data_method", "\"aes128-cbc\"")) return 0 ;
    const char *data_block_b64_newlines = GetVerifyDirectiveValue("data_block") ;
    if (!data_block_b64_newlines) return 0 ;
    char *data_block_b64 = RemoveNewlines(data_block_b64_newlines) ;
    char *data_block = DecodeBase64(data_block_b64, &decoded_len) ;
    Strings::free(data_block_b64) ;

    // Copy iv from beginning of data_block
    char iv[BLOCK_SIZE + 1] ;
    std::memcpy(iv, data_block, BLOCK_SIZE) ;
    iv[BLOCK_SIZE] = '\0' ;

    // Shift data_block pointer to after iv part
    char *data_block_proper = data_block + BLOCK_SIZE ;

    // AES decrypt data block
    char *rtl = DecryptAES_128_CBC(data_block_proper, decoded_len - BLOCK_SIZE, (byte *) session_key, (byte *) iv) ;
    Strings::free(data_block) ;
    Strings::free(session_key) ;
    if (!rtl) return 0 ;

    char *rtl_w_newline = Strings::save(rtl, "\n") ;
    Strings::free(rtl) ;

    return rtl_w_newline ;
}

verific_stream *
ieee_1735::get_decryption_stream()
{
    // Get the decrypted string
    char *decrypted_str = ieee_1735::decrypt() ;
    if (!decrypted_str) return 0 ;

    // Create a new verific_istream object
    verific_istream *decrypt_stream = new verific_istream(new std::istringstream(std::string(decrypted_str))) ;
    Strings::free(decrypted_str) ;

    return decrypt_stream ;
}

char *
ieee_1735::EncodeBase64(const char *in_buf, unsigned in_size)
{
    BIO *buff, *b64f ;
    BUF_MEM *ptr ;

    b64f = BIO_new(BIO_f_base64()) ;
    buff = BIO_new(BIO_s_mem()) ;
    buff = BIO_push(b64f, buff) ;

    BIO_set_flags(buff, BIO_FLAGS_BASE64_NO_NL) ;
    BIO_set_close(buff, BIO_CLOSE) ;
    BIO_write(buff, in_buf, in_size) ;
    BIO_flush(buff) ;
    BIO_get_mem_ptr(buff, &ptr) ;

    unsigned len = ptr->length ;
    char *out_buf = Strings::allocate(len + 1) ;
    std::memcpy(out_buf, ptr->data, len) ;
    out_buf[len] = '\0' ;

    BIO_free_all(buff) ;

    return out_buf ;
}

char *
ieee_1735::DecodeBase64(const char *in_buf, unsigned *length)
{
    char *in = RemoveNewlines(in_buf) ;

    // compute decoded length
    unsigned l = Strings::len(in) ;
    if (in[l - 1] == '=' && in[l - 2] == '=') {
        l = (l * 3) / 4 - 2 ;
    } else if (in[l - 1] == '=') {
        l = (l * 3) / 4 - 1 ;
    } else {
        l = (l * 3) / 4 ;
    }
    char *out_buf = Strings::allocate(l + 1) ;
    out_buf[l] = '\0' ;

    BIO *bio = BIO_new_mem_buf((void *) in, -1) ;
    VERIFIC_ASSERT(bio) ;
    BIO *b64 = BIO_new(BIO_f_base64()) ;
    VERIFIC_ASSERT(b64) ;
    bio = BIO_push(b64, bio) ;

    // Do not use newlines to flush buffer
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL) ;
    *length = BIO_read(bio, out_buf, Strings::len(in)) ;
    VERIFIC_ASSERT(*length == l) ;
    BIO_free_all(bio) ;
    Strings::free(in);

    return out_buf ;
}

/*
 * Encryption method based on examples from openssl website:
 *   https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption
 */
char *
ieee_1735::EncryptAES_128_CBC(const char *in_buf, byte *key, byte *iv, unsigned *out_len)
{
    // Create and initialise the context
    EVP_CIPHER_CTX *ctx ;
    if (!(ctx = EVP_CIPHER_CTX_new())) {
        std::cerr << "Error: failed to initialize AES encryption(1)" << std::endl ;
        return 0 ;
    }

    // Initialise the encryption operation.
    if (EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv) != 1) {
        std::cerr << "Error: failed to initialize AES encryption(2)" << std::endl ;
        return 0 ;
    }

    // Cipher text expands upto BLOCK_SIZE (typically 16 bytes)
    int out_len1 = Strings::len(in_buf) + BLOCK_SIZE ;
    char *out_buf = Strings::allocate(out_len1 + 1) ;

    // Provide the message to be encrypted, and obtain the encrypted output.
    if (EVP_EncryptUpdate(ctx, (byte *) out_buf, &out_len1,
                          (const byte*) in_buf, Strings::len(in_buf)) != 1) {
        std::cerr << "Error: failed to perform AES encryption" << std::endl ;
        return 0 ;
    }

    // Finalise the encryption. Further out bytes may be written at this stage.
    int out_len2 ;
    if (EVP_EncryptFinal_ex(ctx, (byte *) out_buf + out_len1, &out_len2) != 1) {
        std::cerr << "Error: failed to finalize AES encryption" << std::endl ;
        return 0 ;
    }
    *out_len = out_len1 + out_len2 ;

    EVP_CIPHER_CTX_free(ctx) ;

    return out_buf ;
}

/*
 * Decryption method based on examples from openssl website:
 *   https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption
 */
char *
ieee_1735::DecryptAES_128_CBC(const char *in_buf, unsigned in_size, byte *key, byte *iv)
{

    // Create and initialise the context
    EVP_CIPHER_CTX *ctx ;
    if (!(ctx = EVP_CIPHER_CTX_new())) {
        std::cerr << "Error: failed to initialize AES decryption(1)" << std::endl ;
        return 0 ;
    }

    // Initialise the decryption operation.
    if (EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv) != 1) {
        std::cerr << "Error: failed to initialize AES decryption(2)" << std::endl ;
        return 0 ;
    }

    // Provide the message to be decrypted, and obtain the plaintext output.
    char *out_buf = Strings::allocate(in_size + 1) ;
    int len, out_len ;
    if (EVP_DecryptUpdate(ctx, (byte *) out_buf, &len, (const byte *) in_buf, in_size) != 1) {
        std::cerr << "Error: failed to AES decrypt message" << std::endl ;
        return 0 ;
    }

    // Finalise the decryption. Further out bytes may be written at
    // this stage.
    out_len = len ;
    if (EVP_DecryptFinal_ex(ctx, (byte *) out_buf + len, &len) != 1) {
        std::cerr << "Error: failed to finalize AES decryption" << std::endl ;
        return 0 ;
    }
    out_buf[out_len + len] = '\0' ;

    EVP_CIPHER_CTX_free(ctx) ;

    return out_buf ;
}

char *
ieee_1735::InsertNewlines(const char *s, unsigned width)
{
    // Returns a new string that is a copy of s with newlines characters
    // inserted to break long lines in multiple lines with the specified width.
    VERIFIC_ASSERT(s) ;
    VERIFIC_ASSERT(width > 0) ;
    unsigned sl = Strings::len(s) ;
    unsigned rl = sl + ((sl - 1) / width) ;
    char *r = Strings::allocate(rl + 1) ;
    unsigned i = 0 ;
    for (unsigned j=0 ; j<Strings::len(s) ; j++) {
        r[i] = s[j] ;
        i++ ;
        if (((j % width) == (width - 1)) && (j != Strings::len(s) - 1)) {
            if (s[j] != '\n') {
                r[i] = '\n' ;
                i++ ;
            }
        } else {
            VERIFIC_ASSERT(s[j] != '\n') ;
        }
    }
    VERIFIC_ASSERT(i == rl) ;
    r[rl] = '\0' ;
        return r ;
}

char *
ieee_1735::RemoveNewlines(const char *s)
{
    char *new_s = Strings::allocate(Strings::len(s) + 1) ;
    unsigned i = 0 ;
    for (unsigned j=0 ; j<Strings::len(s) ; j++) {
        if (s[j] == '\n') continue ;
        new_s[i] = s[j] ;
        i++ ;
    }
    new_s[i] = '\0' ;
    return new_s;
}

char *
ieee_1735::FormatPublicKey(const char *public_key)
{
    char *public_key_newlines = InsertNewlines(public_key, 64) ;
    char *s = Strings::save("-----BEGIN PUBLIC KEY-----\n",
                            public_key_newlines, "\n",
                            "-----END PUBLIC KEY-----") ;
    Strings::free(public_key_newlines);
    return s;
}

const char *
ieee_1735::GetVerifyDirectiveValue(const char *key, const char *expected)
{
    VERIFIC_ASSERT(key) ;
    const char *value = GetDirectiveValue(key) ;
    if (!value) {
        std::cerr << "Error: directive '" << key << "' not found" << std::endl ;
        return 0 ;
    }
    if (expected && !Strings::compare(value, expected)) {
        std::cerr << "Error: directive '" << key << "' value '" << value << "' does not match expected '"
                  << expected << "'" << std::endl ;
        return 0 ;
    }
    return value ;
}

bool
ieee_1735::IsVhdl()
{
    linefile_type lf = GetLinefile() ;
    if (!lf) return false ;
    const char *filename = LineFile::GetFileName(lf) ;
    if (!filename) return false ;
    return (Strings::len(filename) > 4 && Strings::compare_nocase(filename + Strings::len(filename) - 4, ".vhd")) ||
           (Strings::len(filename) > 4 && Strings::compare_nocase(filename + Strings::len(filename) - 5, ".vhdl")) ;
}
