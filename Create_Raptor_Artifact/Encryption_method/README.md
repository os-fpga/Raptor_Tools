 
## Introduction

The exe file is provided to encrypt and decrypt the required data. Without initial understanding of requirements
it will be hard to use the programe. Our requirement is to provide the file in such a way that the user can not 
read from the files but the data should be available to our flow. 
The encrypted files should be decrepted in side the flow for which we have to provide the key. The key should be
kept in the form that no one can extract it and decrypt the files by its own. If we place the key directly and it can
be read directly from our binary. So the key is placed in our source code and at time of building it will be obfuscated.
In this way no one can read the key from our binary.

## Dependecies

This method required to install OpenSSL libraries and GCC version 9.4.0
For OpenSSL on ubuntu

```
sudo apt-get install libssl-dev
```
The complete list of libraries it uses is as follows
	linux-vdso.so.1 
        libcrypto.so.10 
        libstdc++.so.6 
        libm.so.6 
        libgcc_s.so.1 
        libc.so.6 
        libdl.so.2 
        libz.so.1
        

and on CentOS

```
yum install openssl-libs
```

## Usage

To Encrypt the files

./Encryption_method <input_file to be encrypted>  <output_file which contains the encrypted data> <public_key>

If you want to build the file just do make.

## OUTPUTS

The output will be an encrypted file mentioned above and a vpr_e.bin file. If the password is changed replace the vpr_e.bin in the Raptor

## Requirements

The initial need is to encrypt the architecture files and to add support of encrypted architecture 
files the extension used for them is ".xmle" please use this extension.

**Note**

