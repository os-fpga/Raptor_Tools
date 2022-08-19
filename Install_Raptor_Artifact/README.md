The below is Markdown syntax. For proper view kinly open in Markdown viewer
## Intro 
The `gen_exe/install.sh` is wrapped around the tar exe to install the Raptor from the tar ball of Raptor.

From FTP, the download tar.gz file has following files:

1. README.md (this file)
2. Raptor exe like Raptor_x.x.x.run

## Installing Raptor

### Prerequisites:

* For CentOS, install the required OS dependencies given in centos_os_dep.sh
* For Ubuntu, install the required OS dependencies given in ubuntu_os_dep.sh

### Install Raptor

Upon executing the downloaded exe, it will create a folder named as Raptor_<version> and install binaries inside it.

#### Installing in default location

Go to the directory where you have downloaded a Raptor exe and execute like below

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

Rapid Silicon use FlexLM based license technology to license its product. The FlexLM depends upon lsb-core. Kindly do install them as per your OS.

#### To start a node locked license just simply set the below environment variable

```
export LM_LICENSE_FILE=<path to license file.lic>
```

#### To start the floating type license file:

1. Place the license file in a directory.
2. In license file, write the name of your machine before your host id and port after the host id
3. In the license file, correct the path of vendor daemon 
4. Now start the FlexLM daemon like below

```
<path to lmgrd>/lmgrd -c <path of license file> -l <path to save the log file>
```

Now go to the machine where you have installed the Raptor and setup the below variable:

```
export LM_LICENSE_FILE=<port>@<ip address of license server>
```

For details, you can see the FlexLM lciense server administration guide.