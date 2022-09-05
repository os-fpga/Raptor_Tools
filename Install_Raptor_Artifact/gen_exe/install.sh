#!/bin/bash
set -e
echo -e "###############################################################################" 
echo -e "\tInstalling Raptor, A Rapid Silicon complete Software solution"
echo -e "###############################################################################"

base_dir=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

shoud_i () {

user_id_str=`id |  awk '{ print $1 }'`
user_id=`echo "$user_id_str" | sed 's/uid=\([0-9]*\).*/\1/'`

read -r -p "Do you want us to install OS dependecies for you, It will need admin rights? [Y/n] " input
 
case $input in
      [yY][eE][sS]|[yY])
            echo "You say Yes. Proceeding to Install"
            if [ "$user_id" = 0 ]
            then
                if [ "$ostype" == "CentOS" ]
                then
                    bash  $base_dir/centos_os_dep.sh
                fi
                if [ "$ostype" == "Ubuntu" ]
                then
                    bash $base_dir/ubuntu_os_dep.sh
                fi
            else
                echo "You choose to install dependencies but you don't have admin rights."
                echo "Run this script with sudo or as root user"
                exit 1
            fi

            ;;
      [nN][oO]|[nN])
            echo "Okay, I will not attempt to install. But Raptor can't be run without them so exiting."
            exit 1
            ;;
      *)
            echo "Invalid input..."
            exit 1
            ;;
esac

}

are_dep_ok () {
    if [ "$ostype" == "CentOS" ]
    then
        echo -e "\nYou are installing Raptor on CentOS OS.\nExecuting installer with Admin rights can install all OS dependencies"
    elif [ "$ostype" == "Ubuntu" ]
    then
        echo -e "\nYou are installing Raptor for Ubuntu OS.\nExecuting installer with Admin rights can install all OS dependencies"
    else
        echo "unkown OS"
        exit 1
    fi
read -r -p "Are OS dependencies are already installed? [Y/n] " input
 
case $input in
      [yY][eE][sS]|[yY])
            echo "You say Yes. Proceeding to Install"
            ;;
      [nN][oO]|[nN])
            echo "You say No."
            shoud_i $ostype
            ;;
      *)
            echo "Invalid input..."
            exit 1
            ;;
esac

}

#Required path of tar file, external_lib tar file and destination directory where Raptor will be installed
install_from_tar () {

#    raptor_tar_path=$1
#    RAPTOR_HOME=$2   --> install destination
    echo -e "Raptor Tar file that will be used is\n$1"

    raptor_instl_dir=$(basename $1 .tar)
    echo "Raptor will be installed in $raptor_instl_dir"

    if [[ -f "$1" ]]
    then
        touch $2/.raptor_install.log
        if [ $? -ne 0 ]
        then
            echo "Seems like don't have write permission in $2"
            exit 1
        else
            [[ ! -d "$2/$raptor_instl_dir" ]] && mkdir -p $2/$raptor_instl_dir
            tar -xvf $1 -C $2/$raptor_instl_dir | tee $2/.raptor_install.log
            cp $2/$raptor_instl_dir/share/raptor/doc/README.md $2/$raptor_instl_dir
            mv $2/.raptor_install.log $2/$raptor_instl_dir
        fi
    else
        echo "Tar file of Raptor does not exist"
        exit 1
    fi
    
    echo -e "########################################################################################"
    echo -e "# Installation is done :)"
    echo -e "# To invoke Raptor\n         source $2/$raptor_instl_dir/raptorenv_lin64.sh and type raptor --version"
    echo -e "# For detail usage --> See\n      $2/$raptor_instl_dir/README.md"
    echo -e "########################################################################################"


}

# this check is needed as we need to generate raptor_setup.sh
is_raptor_home_absolute () {

ok=0
[ "$1" != "${1#/}" ] || ok=1
if [ $ok -eq 1 ]
then
    echo "Kindly provide absolute path to -r or --raptor-home option"
    exit 1
fi

}

usage()
{
  echo "Usage: install.sh            [ -h | --help]             show the help
                             [ -r | --raptor-home  ]    Specify the absolute path of Directory where Raptor will be Installed. Default is /opt"
  exit 2
}


PARSED_ARGUMENTS=$(getopt -a -n install -o hr: --long help,raptor-home: -- "$@")
VALID_ARGUMENTS=$?
if [ "$VALID_ARGUMENTS" != "0" ]; then
  usage
fi
#echo "PARSED_ARGUMENTS is $PARSED_ARGUMENTS"
eval set -- "$PARSED_ARGUMENTS"
while :
do
  case "$1" in
    -r | --raptor-home)     raptor_h="$2";   shift 2 ;;  
    -h | --help)            usage  ;;
    # -- means the end of the arguments; drop this, and break out of the while loop
    --) shift; break ;;
    # If invalid options were passed, then getopt should have reported an error,
    # which we checked as VALID_ARGUMENTS when getopt was called...
    *) echo "Unexpected option: $1 - this should not happen."
       usage ;;
  esac
done

ostype=`egrep '^(NAME)=' /etc/os-release  | grep -o -e Ubuntu -e CentOS`
echo "Detected OS type is $ostype"

are_dep_ok

# install from tar
t_path="./Raptor_*.tar"
if [[ -z "$raptor_h" ]]
then
    read -r -p "You have not specified the install path. Is /opt is good? [Y/n] " input
case $input in
      [yY][eE][sS]|[yY])
            echo "You say Yes. Proceeding to Install"
            raptor_h=/opt
            ;;
      [nN][oO]|[nN])
            echo "You say No. Kindly provide the Install path using -r or --raptor_home option"
            usage
            ;;
      *)
            echo "Invalid input..."
            exit 1
            ;;
esac
fi
is_raptor_home_absolute $raptor_h
install_from_tar $t_path $raptor_h
echo -e "Done installing Raptor"