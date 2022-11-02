#!/bin/bash
set -e
go_tar=0
go_all=0
go_p=0
do_scp=0
printf "This script build and install Raptor\nIt creates tar of Raptor from custom install directory\n\n"
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

# a temp function to do temporary work
temp () {

    # remove sta from Raptor install dir
    rm -rvf $1/bin/sta $1/lib/libOpenSTA.a $1/include/sta
}


# this check is needed 
is_raptor_home_absolute () {

ok=0
[ "$1" != "${1#/}" ] || ok=1
if [ $ok -eq 1 ]
then
    echo "Kindly provide absolute path to -r or --raptor_home option"
    exit 1
fi

}

# make sure SSH keys are deployed to FTP server
# $1 input is absolute path to Tools directory 
# $2 version of Raptor
# do manual scp or not (here it is $3 but coming as $4)
# create version dir inside upload ($4)
upload_to_ftp () {

# create tar.gz having .run and README
final_tar_dir=`dirname $1` 
cd $final_tar_dir/Install_Raptor_Artifact && tar -cvzf Raptor_$2\.tar.gz Raptor_$2\.run README.md && rm Raptor_$2\.run  && echo "Done Creating final tar"
mkdir -p $final_tar_dir/upload && rm -rf $final_tar_dir/upload/* && mkdir -p $final_tar_dir/upload/$4
mv $final_tar_dir/Install_Raptor_Artifact/Raptor_$2\.tar.gz  $final_tar_dir/upload/$4

# run this command only when running manually
if [ $3 -eq 1 ]
then
ssh -p 14178 rsbuilder@116.58.61.19 mkdir -p Raptor/$(date '+%F')
ssh -p 14178 rsbuilder@116.58.61.19 rm -rf /opt/customer/latest/Raptor*
scp -P 14178  $final_tar_dir/Install_Raptor_Artifact/Raptor_$2\.tar.gz rsbuilder@116.58.61.19:/home/rsbuilder/Raptor/$(date '+%F')
scp -P 14178  $final_tar_dir/Install_Raptor_Artifact/Raptor_$2\.tar.gz rsbuilder@116.58.61.19:/opt/customer/latest
fi

}

do_encryption () {

cd $SCRIPT_DIR/Encryption_method/src && make
device_base_dir=$1/share/raptor/etc/devices
rs_device="gemini/gemini_vpr.xml mpw1/TSMC22nm_vpr.xml"
for device in $rs_device
do
    echo "Encrypting --> $device_base_dir/$device"
    $SCRIPT_DIR/Encryption_method/Encryption_method $device_base_dir/$device  $device_base_dir/$device\e $SCRIPT_DIR/Encryption_method/public_key 
    rm -f $device_base_dir/$device
done
# change the name of devices on device.xml
device_config=$1/share/raptor/etc/device.xml
for device in $rs_device
do
    echo "current device is --> $device"
    sed -i "s|$device|$device\e|g" $device_config
    echo "done change for $device"
done

}

#Required source: the path of directory where raptor is already installed
# exe file will be created in Install_Raptor_Artifact
# go_production will enable the encryption
# do scp if running manually
# append release number from command line
create_tar_ () {

# call temp function to do any temp work around
temp $1
    source=$1
    destination=`dirname $2`
    destination=$destination/Install_Raptor_Artifact/gen_exe
    raptor_version=`$source/bin/raptor --version | grep "Version" | awk '{print $3}'`
    if [ -z $raptor_version ]
    then
        echo "Raptor can't be invoked from the provided path. Kindly rectify the error"
        exit 1 
    else
        if [ $3 -eq 1 ]
        then 
        do_encryption $source
        fi
	cd $SCRIPT_DIR && wget https://github.com/RapidSilicon/pre_build_artifacts/releases/download/v1.0/external_libs.tar.gz
        tar -xvzf $2/external_libs.tar.gz -C $source
        cd $destination && tar -cjvf Raptor_$5\.tar -C $source .
        [ -f $SCRIPT_DIR/../../licenses/rs-eula.txt ] && lic="$SCRIPT_DIR/../../licenses/rs-eula.txt" || { echo "Failed to find license"; exit 1; }
        cd ../ && $SCRIPT_DIR/makeself-2.4.0/makeself.sh  --sha256 --license $lic $destination Raptor_$5\.run "Raptor installer" ./install.sh
    fi
    upload_to_ftp $2 $5 $4 "$raptor_version"

}

# Clone, compile and install from GitHub repo. Required: RAPTOR_HOME 
clone_compile_install_from_rapo () {

    printf "Now will perform following:\n"
    printf "1. Clone Raptor source code from GitHub\n2. Compile\n3. Install in directory specified by -w or --work_space\n"
    WORK_SPACE=$1
    touch $WORK_SPACE/.raptor_install.log
    if [ $? -ne 0 ]
    then
        echo "Seems like don't have write permission in $WORK_SPACE"
        exit 1
    fi
    src_from_github=$WORK_SPACE/clone_dir
    mkdir -p $src_from_github
    cd $src_from_github && git clone git@github.com:RapidSilicon/Raptor.git | tee $WORK_SPACE/.raptor_install.log
    cd $src_from_github/Raptor &&  git submodule update --init --recursive | tee -a $WORK_SPACE/.raptor_install.log
    if [ $? -ne 0 ]
    then 
        echo "Failed to clone Raptor Sub-modules Repos, it might be permisison issue or internet connectivity problem"
        exit 1
    fi
    if [ $2 -eq 1 ]
    then
	echo "Will create Production Build"
        cd $src_from_github/Raptor && make PRODUCTION_BUILD=1
    else
        cd $src_from_github/Raptor && make
    fi
    if [ $? -ne 0 ]
    then 
        echo "Failed to build Raptor from source"
        exit 1
    fi
    install_dir=$WORK_SPACE/instl_dir
    mkdir -p $install_dir
    if [ $2 -eq 1 ]
    then
        echo "Will install Production Build"    
    cd $src_from_github/Raptor && make install PREFIX=$install_dir PRODUCTION_BUILD=1
    else
       cd $src_from_github/Raptor && make install PREFIX=$install_dir
    fi
}

usage()
{
  echo "Usage: create_release.sh     [ -h | --help]             show the help
                             [ -c | --create_tar ]      Flag to turn on tar generation in present directory. It required -w or --work_space.
			     [ -p | --production]       Flag to turn on production build.
                             [ -v | --raptor-version]   Raptor version for release
                             [ -m | --manual]           Flag to turn on manual uploading of scp.
                             [ -w | --work_space  ]     Specify the Directory where clone, build and installation will occur or where already pre-build raptor is exist.
                             [ -a | --all ]             Flag to turn on cloning, building, installation and tar generation. It required -w or --work_space"
  exit 2
}

if [ $# -eq 0 ]; then
    echo "No arguments provided"
    usage
    exit 1
fi

PARSED_ARGUMENTS=$(getopt -a -n create_release -o hacpmv:w: --long help,all,create_tar,production,manual,raptor-version:,work_space: -- "$@")
VALID_ARGUMENTS=$?
if [ "$VALID_ARGUMENTS" != "0" ]; then
  usage
fi
#echo "PARSED_ARGUMENTS is $PARSED_ARGUMENTS"
eval set -- "$PARSED_ARGUMENTS"
while :
do
  case "$1" in
    -w | --work_space)  w_dir="$2";   shift 2 ;;
    -v | --raptor-version)  release="$2";   shift 2 ;;
    -c | --create_tar)  go_tar=1 ; shift  ;;
    -p | --production)  go_p=1 ; shift  ;;
    -m | --manual)      do_scp=1 ; shift  ;;
    -a | --all)         go_all=1 ; shift  ;;    
    -h | --help)        usage  ;;
    # -- means the end of the arguments; drop this, and break out of the while loop
    --) shift; break ;;
    # If invalid options were passed, then getopt should have reported an error,
    # which we checked as VALID_ARGUMENTS when getopt was called...
    *) echo "Unexpected option: $1 - this should not happen."
       usage ;;
  esac
done

if [ -z $release ]
then
    echo "Providing Raptor release number is mandatory. Exiting"
    exit 1
fi

ostype=`egrep '^(NAME)=' /etc/os-release  | grep -o -e Ubuntu -e CentOS`
echo "Detected OS type is $ostype"
#are_dep_ok
# generate tar
if [ $go_tar -eq 1 ]
then
    if [[ -z "$w_dir" ]]
    then
        echo "Generate Tar from already installed Raptor. Required install directory path"
        exit 1
    fi
    create_tar_ $w_dir $SCRIPT_DIR $go_p $do_scp $release
    echo "Done creating tar of already present Raptor"
    exit
fi
if [ $go_all -eq 1 ]
then
    if [[ -z "$w_dir" ]]
    then
        echo "Required work space directory path. It is where clone, build and installation of raptor will occur"
        exit 1
    fi 
    is_raptor_home_absolute $w_dir   
    clone_compile_install_from_rapo $w_dir $go_p
    echo "Done building and installing Raptor"
    create_tar_ $w_dir/instl_dir $SCRIPT_DIR $go_p $do_scp $version
    echo "Done creating tar of recent build Raptor"
fi
