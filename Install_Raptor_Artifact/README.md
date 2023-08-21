
This file is in Markdown syntax. For better readability, kindly open in Markdown viewer

# Intro 
A brief guide to install Raptor.

The downloaded *.tar.gz file from FTP site contains following files:

1. README.md (this file)
2. Raptor exe like Raptor_x.x.run

## Installing Raptor

Installer has 2 modes. 

1): GUI mode

2): Batch mode

Provide empty directory to installer for installation as installer wipe the directory before doing installation
Upon executing the downloaded executable, it will create a folder named as {provided path or /opt/RapidSilicon}/Raptor/{release} and install binaries inside it.

#### In GUI mode

1): Install Raptor without SUDO:

```
         ./Raptor_x.x.run 
```

2): Install Raptor at Admin writable location. Invoke installer with SUOD like below: 

```
                sudo    ./Raptor_x.x.run 
```

#### In Batch mode

You have following command line options available:

* --batch                   -> [Mandatory] Run in batch mode.
* --accept                  -> [Mandatory] Grant acceptance of license on command line. 
* -r | --raptor-home        -> [Mandatory] Giving the absolute path of directory where Raptor will be installed. 


Above options can be used in following combinations

1): Without SUDO/Admin privileges, Installing at desired path:

```
     ./Raptor_x.x.run  --batch --accept -r <absolute path where Raptor will be installed>
```

2): With SUDO/Admin privileges, click next to accept the default path on installation screen. 

```
    sudo ./Raptor_x.x.run --batch --accept -r /opt/RapidSilicon
```

Kindly replace `Raptor_x.x.run` with the version of Raptor run file you have received.

### Post Installation

After doing the installation, you can run the Raptor. But for HDL simulator to function properly, you need to install the below dependencies.
#### For Unix:

Follow the below steps to configure the Raptor HDL simulator:

* Open Terminal

* Change directory to Raptor installation folder:

```
        cd <Raptor install path>/Raptor/<release>/share
```

Where Raptor install path is the directory where you have intalled Raptor
release is like 2023.01

* Execute the script with admin privileges "unix_dep.sh" in this directory.

```
        bash sudo unix_dep.sh
```

This will prompt for password. Enter Admin/root password. 

### Starting Raptor

Once the installation is done, go to {provided installation path or /opt/RapidSilicon}/Raptor/{release} and source the environment setup script "raptorenv_lin64.sh". For bash shell you can do:

source {provided installation path}/Raptor/{release}/raptorenv_lin64.sh

Then on same command line, writing

         raptor

Will pop up the GUI of Raptor.

### Starting License:

Rapid Silicon use FlexLM based license technology to license its product. The FlexLM depends upon lsb-core. Kindly install them as per your OS.

#### To start a node locked license just simply set the below environment variable

```
export LM_LICENSE_FILE=<absolute path to license file.lic>
```

#### To start the floating type license file:

1. Place the license file in a directory of your choice.
2. In license file, write the name of your machine before host id and port after the host id
3. In the license file, correct the path of `rapidsil` daemon 
4. Now start the FlexLM daemon like below

```
${Raptor install directory}/bin/Flex_LM/lmgrd -c <path of license file> -l <path to save the debug log file>
```

Now go to the machine where you have installed the Raptor and set the below variable:

```
export LM_LICENSE_FILE=<port>@<ip address of license server>
```

For further details, you can see the FlexLM license server administration guide.

