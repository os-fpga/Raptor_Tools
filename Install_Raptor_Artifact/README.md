
This file is in Markdown syntax. For better readability, kindly open in Markdown viewer

# Intro 
A brief guide to install Raptor.

The downloaded *.tar.gz file from FTP site contains following files:

1. README.md (this file)
2. Raptor exe like Raptor_x.x.x.run

## Installing Raptor

### Prerequisites:

Raptor depends upon many OS dependecies. These dependecies can be installed by running the installer with SUDO privileges or as root user.

### Install Raptor

Upon executing the downloaded executable, it will create a folder named as {provided path or /opt}/RapidSilicon/Raptor/{release} and install binaries inside it.

#### Installing without any command line options

1): With SUDO/Admin privileges, say yes to dependencies installation and default path when installer prompt for questions. 

```
sudo ./Raptor_x.x.run 
```

2): Without SUDO/Admin privileges, say no to dependencies installation and default path when installer prompt for questions

```
./Raptor_x.x.run 
```

Kindly replace `Raptor_x.x.run` with the version of Raptor exe file you have received.

#### Installing with personal choices (With SUDO/Admin privileges)

You have following command line options available:

sudo ./Raptor_x.x.run <options>

* --accept            -> Grant acceptance of license on command line. It will still print the license. Please read it. 
* -r | --raptor-home  -> Giving the absolute path of directory where Raptor will be installed
* -i | --install-dep  -> Turn on the OS dependecies installation. This will need SUDO/Admin privileges
* -v | --verbose      -> Increase the installer verbosity.

#### Installing with personal choices (Without SUDO/Admin privileges)

You have following command line options available:

./Raptor_x.x.run <options>

* --accept               -> Grant acceptance of license on command line. It will still print the license. Please read it. 
* -b | --no-dep-install  -> By pass the OS related dependencies installation
* -r | --raptor-home     -> Giving the absolute path of directory where Raptor will be installed
* -v | --verbose         -> Increase the installer verbosity.



**Note**: When using -i or --install-dep SUDO/Admin privileges are required.      

Go to the directory where you have extracted the Raptor executable after downloading and execute it with any or with all above mentioned options like below.

```
./Raptor_x.x.run -r <absolute path where Raptor will be installed>
``` 

You can add --accept option as well to accept the EULA license. Like

```
./Raptor_x.x.run --accept -v -r <absolute path where Raptor will be installed>
``` 

And if you want to install dependencies as well which needs SUDO/Admin privileges, you can execute the installer as:

```
sudo ./Raptor_x.x.run --accept -v -i -r <absolute path where Raptor will be installed>
``` 

Kindly replace `Raptor_x.x.run` with the version of Raptor executable file you have received.

### Starting Raptor

Once the installation is done, go to {provided installation path}/RapidSilicon/Raptor/{release} and source the environment setup script "raptorenv_lin64.sh". For bash shell you can do:

source {provided installation path}/RapidSilicon/Raptor/{release}/raptorenv_lin64.sh

Then on same command line, writing

         raptor

Will pop up the GUI of Raptor.

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

For further details, you can see the FlexLM license server administration guide.

