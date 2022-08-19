 
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

and on CentOS

```
yum install openssl-libs
```

## Usage

To Encrypt the files

./Encryption_method <input_file to be encrypted>  <output_file which contains the encrypted data>

In case if you wants to test the file.

./Decryption_method <input_file to be decrepted>  <output_file which contains the decrypted data>

## Requirements

The initial need is to encrypt the architecture files and to add support of encrypted architecture 
files the extension used for them is ".exml" please use this extension.

**Note**

The reason of providing two seperate methods is to ease your work as if both are in one executeable 
you will have to select every time what you will have to do.
If you need to change anything I have also provided the source code src directory kindly build it with the following command
g++ aes.cpp -o result -lcrypto