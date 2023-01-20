
This file is in Markdown syntax. For better readability, kindly open in Markdown viewer

# Intro 
A brief guide to install Raptor.

The downloaded *.tar.gz file from FTP site contains following files:

1. README.md (this file)
2. Raptor exe like Raptor_x.x.run

## Before you begin:

Raptor installer use QT installer Framework. If you have installed the OS with Desktop environment enabled then you can skip and move to Raptor installation. You can run below command to check either Desktop environment is installed or not:

```
echo $XDG_CURRENT_DESKTOP
```

If above command print nothing then you are running headless (just command line with no Desktop environment installed) then make sure to install below dependencies. 

On Debian:

```
    sudo apt install qtbase5-dev -y
```

On RedHat:

```
    sudo yum install -y xcb-util-image-devel xcb-util-keysyms-devel xcb-util-renderutil-devel xcb-util-wm-devel libxcb libxcb-devel xcb-util xcb-util-devel libxkbcommon-devel libxkbcommon-x11-devel
```

If you are running any other OS then search and install QT5 dependencies.

## Installing Raptor

### Prerequisites:

Raptor depends upon OS dependecies. These dependecies can be installed by running the installer with SUDO privileges or as root user.

### Install Raptor

Installer has 2 modes. 
1): GUI mode
2): Batch mode

Provide empty directory to installer for installation as installer wipe the directory before doing installation
Upon executing the downloaded executable, it will create a folder named as {provided path or /opt}/RapidSilicon/Raptor/{release} and install binaries inside it.

#### In GUI mode

1): Install Raptor as well as dependencies. Invoke installer like below:

```
            sudo ./Raptor_x.x.run 
```

2): Install only Raptor with no dependencies. Make sure you select install directory with write permissions and on prompting for dependencies installation, say, click no

```
                    ./Raptor_x.x.run 
```

#### In Batch mode

You have following command line options available:

* --batch                   -> [Mandatory] Run in batch
* --accept                  -> [Mandatory] Grant acceptance of license on command line. It will still print the license. Please read it.  
* -r | --raptor-home        -> [Mandatory] Giving the absolute path of directory where Raptor will be installed. 
* -i | --install-dep        -> Turn on the OS dependecies installation. This will need SUDO/Admin privileges
* -b | --no-dep-install     -> Turn off the OS related dependecies installation

Note: It is mandatory to provide either --install-dep or --no-dep-install.

Above options can be used in following combinations

1): Without SUDO/Admin privileges, Installing at desired path with no OS dependencies:

```
     ./Raptor_x.x.run  --batch --accept --no-dep-install -r <absolute path where Raptor will be installed>
```

2): With SUDO/Admin privileges, say yes to dependencies installation on prompt with default path. 

```
    sudo ./Raptor_x.x.run --batch --accept --install-dep -r /opt/raptor 
```

3): With SUDO/Admin privileges, say yes to dependencies installation on prompt with custom path.

```
    sudo ./Raptor_x.x.run --batch --accept --install-dep -r <absolute path where Raptor will be installed> 
```

Kindly replace `Raptor_x.x.run` with the version of Raptor exe file you have received.

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

