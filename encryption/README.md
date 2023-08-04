# RS Encryption Tool:
RS is using IEEE 1735 which is IEEE Recommended Practice for Encryption and Management of Electronic Design Intellectual Property (IP). This standard was approved and published in late 2015​. It Provides confidentiality while ensuring interoperability between EDA tools.

# Usage:

## For Developers:

Clone this repo:

```bash
git clone git@github.com:RapidSilicon/Raptor_Tools.git --recurse-submodules --progress
```

Go to encryption directory and create a build directory:

```
cd encryption && mkdir build
```
Go to this build directory and invoke the cmake like below:

```bash
cmake ..
```
Once cmake confiuraiton is done then invoke make command:

```bash
make
```
Now go to bin directory and invoke the exe

## For End User:

The end user will receive a tar file. 
Do the below step to unpack the tool:

1. Create a new directory. This is your install directory.
2. Go to this directory and open a terminal in this directory.
3. Run the below command to unpack the tool:

```
tar -xvzf <path to rs_encryption_tool.tar.gz>
```
4. Once done. you will see three directories, those are 
    
    1. bin 
    
    2. exmaples
    
    3. lib
5. Go to bin directory and run the below command inside the bin directory

```
export PATH=$(pwd):$PATH
```

6. Now run below command to verify that you can invoke the tool from any directory:

```
which rs_encrypt
```
If above command prints the path of install directory then tool is installed successfully. 

7. For running a simple example use this command 

```
rs_encrypt  -k  -k examples/vcs_pragmas_public examples/RS_Key -f  examples/simple_rtl.v
```