The below is Markdown syntax. For proper view kinly open in Markdown viewer
## Intro 
A brief guide to install Raptor.

The downloaded *.tar.gz file from FTP site contains below files:

1. README.md (this file)
2. Raptor exe like Raptor_x.x.x.run

## Installing Raptor

### Prerequisites:

Raptor depends upon many OS dependecies. These dependecies can be installed by running the installer with SUDO privileges or as root user.

### Install Raptor

Upon executing the downloaded exe, it will create a folder named as Raptor_<version> and install binaries inside it.

#### Installing in default location

Go to the directory where you have downloaded a Raptor exe and execute it like below

```
./Raptor_x.x.x.run
```
Kindly replace `Raptor_x.x.x.run` with the version of Raptor exe file you have received.

#### Installing in preferred location

Go to the directory where you have downloaded a Raptor exe and execute like below

```
./Raptor_x.x.x.run -- -r <absolute path where Raptor will be installed>
```
Kindly replace `Raptor_x.x.x.run` with the version of Raptor exe file you have received.

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