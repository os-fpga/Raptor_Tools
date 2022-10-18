
This file is in Markdown syntax. For better readability, kindly open in Markdown viewer

## Intro 
A brief guide to install Raptor.

The downloaded *.tar.gz file from FTP site contains following files:

1. README.md (this file)
2. Raptor exe like Raptor_x.x.x.run

## Installing Raptor

### Prerequisites:

Raptor depends upon many OS dependecies. These dependecies can be installed by running the installer with SUDO privileges or as root user.

### Install Raptor

Upon executing the downloaded executable, it will create a folder named as {PREFIX}/RapidSilicon/Raptor/<release> and install binaries inside it.

#### Installing with default options

With default options, you are allowing the Raptor executable to install all the OS pre requisite and install Raptor in default location i.e. /opt
Providing any other options except `-v or --verbose and --accept` will have no effect in this case.
Go to the directory where you have extracted the Raptor executable after downloading and execute it with SUDO/admin account like below

```
./Raptor_x.x.run 
```

Or if you want to accept the license

```
./Raptor_x.x.run --accept 
```

Kindly replace `Raptor_x.x.run` with the version of Raptor exe file you have received.

#### Installing with personal choices

You have following command line options available:

* --accept            -> Grant acceptance of license on command line. It will still print the license. Please read it. 
* -b | --batch-mode   -> running in interactive mode. **Mandatory if you want to use -r or -i option**
* -r | --raptor-home  -> you are giving the absolute path of directory where Raptor will be installed
* -i | --install-dep  -> Turn on the OS dependecies installation
* -v | --verbose      -> increase the installer verbosity.

Go to the directory where you have extracted the Raptor executable after downloading and execute it with any or with all above mentioned options like below

```
./Raptor_x.x.run -- -b -i -v -r <absolute path where Raptor will be installed>
``` 

You can add --accept option as well. Like

```
./Raptor_x.x.run --accept -- -b -i -v -r <absolute path where Raptor will be installed>
``` 

Kindly replace `Raptor_x.x.run` with the version of Raptor executable file you have received.

### Starting License:

Rapid Silicon use FlexLM based license technology to license its product. The FlexLM depends upon lsb-core. Kindly install them as per your OS.

#### To start a node locked license just simply set the below environment variable

```
export LM_LICENSE_FILE=<path to license file.lic>
```

#### To start the floating type license file:

1. Place the license file in a directory of your choice.
2. In license file, write the name of your machine before host id and port after the host id
3. In the license file, correct the path of `rapidsil` daemon 
4. Now start the FlexLM daemon like below

```
${Raptor install directory}/bin/Flex_LM/lmgrd -c <path of license file> -l <path to save the log file>
```

Now go to the machine where you have installed the Raptor and set the below variable:

```
export LM_LICENSE_FILE=<port>@<ip address of license server>
```

For further details, you can see the FlexLM lciense server administration guide.
