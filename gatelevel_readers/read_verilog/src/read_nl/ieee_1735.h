#ifndef _VERIFIC_ieee_1735_H_
#define _VERIFIC_ieee_1735_H_

#include <openssl/ssl.h>
#include "Protect.h"

#ifdef VERIFIC_NAMESPACE
namespace Verific { // start definitions in verific namespace
#endif

#define BLOCK_SIZE 16  // 128 bits

class ieee_1735 : public Protect
{
public:

    ieee_1735() ;
    virtual ~ieee_1735() {}

    typedef unsigned char byte ;
    const char* pvt_key_file;

    // Encryption methods
    virtual unsigned encrypt(const char *in_buf, char **out_buf, unsigned in_size) ;
    virtual char *GetEncryptionHeader() ;
    virtual char *GetEncryptionFooter() ;

    // Decryption method
    virtual char *decrypt(void) ;
    virtual verific_stream *get_decryption_stream() ;
    void set_pvt_key(const char *key_file) ;
    const char* get_pvt_key();

protected:

    // Base 64 encode/decode static methods
    static char *EncodeBase64(const char *in_buf, unsigned in_size) ;
    static char *DecodeBase64(const char *in_buf, unsigned *out_len) ;

    // AES encrypt/decrypt methods
    static char *EncryptAES_128_CBC(const char *in_buf, byte *key, byte *iv, unsigned *out_len) ;
    static char *DecryptAES_128_CBC(const char *in_buf, unsigned in_size, byte *key, byte *iv) ;

    // Other helper methods
    static char *InsertNewlines(const char *s, unsigned width) ;
    static char *RemoveNewlines(const char *s) ;
    static char *FormatPublicKey(const char *public_key) ;
    const char *GetVerifyDirectiveValue(const char *key, const char *expected=0) ;
    bool IsVhdl() ;

    char _session_key[BLOCK_SIZE + 1] ;
} ; // class ieee_1735

/* -------------------------------------------------------------- */

#ifdef VERIFIC_NAMESPACE
} // end definitions in verific namespace
#endif

#endif // #ifndef _VERIFIC_ieee_1735_H_
