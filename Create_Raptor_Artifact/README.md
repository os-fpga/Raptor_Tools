## Intro 
This script is used to:
1.  Clone, build and install the Raptor in the user specified directory.
2.  Generate the Tar ball from the installed Raptor Directory.

See the help of script for usage:

```bash
bash create_release.sh -h
```
```
This script build and install Raptor
It creates tar of Raptor from custom install directory

Usage: create_release.sh     [ -h | --help]             show the help
                             [ -c | --create_tar ]      Flag to turn on tar generation in present directory. It required -w or --work_space.
			                 [ -p | --production]       Flag to turn on production build.
                             [ -m | --manual]           Flag to turn on manual uploading of scp.
                             [ -w | --work_space  ]     Specify the Directory where clone, build and installation will occur or where already pre-build raptor is exist.
                             [ -a | --all ]             Flag to turn on cloning, building, installation and tar generation. It required -w or --work_space

```

## Working:

### Prerequisites:

* For CentOS, install the require dependencies given [here](https://github.com/RapidSilicon/Raptor/blob/main/.github/workflows/install_centos_dependencies_build.sh)
* For  Ubuntu, install the require dependencies given [here](https://github.com/RapidSilicon/Raptor/blob/main/.github/workflows/install_ubuntu_dependencies_build.sh)

* The generating exe will be uploaded to FTP. Make sure you have deployed the SSH keys

**Note:** GCC version greater than 9 and Qt5 is required for building Raptor. Kindly install them before proceeding. 

### Generate Tar

Excute able is generated from the already installed/buld Raptor. Execute the script like below to generate tar. The tar will be generated in directory where this script is residing

```bash
bash create_release -c -w <path where Raptor is installed>
```

### Clone, build, install and generate tar:

To do everything that is clone, build, install and generate tar with production build enable, execute script like this:

```bash
bash create_release -p -a -w <path where clone, build and install will occur>
```