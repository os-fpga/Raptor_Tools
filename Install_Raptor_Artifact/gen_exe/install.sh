#!/bin/bash
set -e
echo -e "###############################################################################" 
echo -e "\tInstalling Raptor, A Rapid Silicon complete Software solution"
echo -e "###############################################################################"

base_dir=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

are_dep_ok () {

echo "[INFO]    Proceeding to Install OS dependencies"

user_id_str=`id |  awk '{ print $1 }'`
user_id=`echo "$user_id_str" | sed 's/uid=\([0-9]*\).*/\1/'`

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
    echo "[ERROR]   You choose to install dependencies but you don't have admin rights."
    echo "[ERROR]   Run this script with sudo or as root user"
    exit 1
fi

}

#Required path of tar file, external_lib tar file and destination directory where Raptor will be installed
install_from_tar () {

# 3rg input args is about verbosity. By default it is off
if [ -z $3 ]
then
    tar_flags="-xf"
else
    tar_flags="-xvf"
fi
#    raptor_tar_path=$1
#    RAPTOR_HOME=$2   --> install destination
    echo -e "[INFO]     Raptor Tar file that will be used is $1"

    raptor_instl_dir=`echo $1 | sed -E 's/[A-Za-z_]/ /g;s/. {1,}$//;s/^ {1,}([0-9])/\1/'`
    raptor_instl_dir=$2/RapidSilicon/Raptor/$raptor_instl_dir
    echo "[INFO]    Raptor will be installed in $raptor_instl_dir"

    if [[ -f "$1" ]]
    then
        touch $2/.raptor_install.log
        if [ $? -ne 0 ]
        then
            echo "[ERROR]   Seems like don't have write permission in $2"
            exit 1
        else
            [[ ! -d "$raptor_instl_dir" ]] && mkdir -p $raptor_instl_dir || { echo "Specified Directory already has Raptor installed. Can't over write so aborting installation.. :("; exit 1; }
            echo "[INFO]    Doing the installation...."
            tar $tar_flags $1 -C $raptor_instl_dir | tee $2/.raptor_install.log
            cp $raptor_instl_dir/share/raptor/doc/README.md $raptor_instl_dir
            cd $raptor_instl_dir/share/envs/litex && echo $(pwd) > .venv
            mv $2/.raptor_install.log $raptor_instl_dir
            chmod -R 755 $raptor_instl_dir/../../../RapidSilicon
        fi
    else
        echo "[ERROR]   Tar file of Raptor does not exist"
        exit 1
    fi
    
    echo -e "#####################################################################################################"
    echo -e "# Installation is done :)"
    echo -e "# To invoke Raptor\n         source $raptor_instl_dir/raptorenv_lin64.sh and type raptor --version"
    echo -e "# For detail usage --> See\n      $raptor_instl_dir/README.md"                      
    echo -e "#####################################################################################################"


}

# this check is needed as we need to generate raptor_setup.sh
is_raptor_home_absolute () {

ok=0
[ "$1" != "${1#/}" ] || ok=1
if [ $ok -eq 1 ]
then
    echo "[ERROR]   Kindly provide absolute path to -r or --raptor-home option"
    exit 1
fi

}

usage()
{
  echo "Usage: install.sh            [ -h | --help]                 show the help
                             [ -i | --install-dep  ]        Turn on the OS related dependecies installation. 
                             [ -v | --verbose      ]        Turn on the verbosity.
                             [ -b | --no-dep-install   ]    Turn off the OS related dependecies installation
                             [ -r | --raptor-home  ]        Specify the absolute path of Directory where Raptor will be Installed. Default is /opt"
  exit 2
}


PARSED_ARGUMENTS=$(getopt -a -n install -o hivbr: --long help,install-dep,verbose,no-dep-install,raptor-home: -- "$@")
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
    -i | --install-dep)     dep_install="yes";         shift   ;;
    -v | --verbose)         go_verbose=1;         shift   ;;
    -b | --no-dep-install)      bypass_dep="yes";         shift   ;;
    -h | --help)            usage  ;;
    # -- means the end of the arguments; drop this, and break out of the while loop
    --) shift; break ;;
    # If invalid options were passed, then getopt should have reported an error,
    # which we checked as VALID_ARGUMENTS when getopt was called...
    *) echo "[ERROR]    Unexpected option: $1 - this should not happen."
       usage ;;
  esac
done

ostype=`egrep '^(NAME)=' /etc/os-release  | grep -o -e Ubuntu -e CentOS`
#echo "[INFO]    Installing Raptor on $ostype"
if [ "$ostype" == "CentOS" ]
then
    echo -e "\n[INFO]   You are installing Raptor on CentOS OS."
elif [ "$ostype" == "Ubuntu" ]
then
    echo -e "\n[INFO]   You are installing Raptor for Ubuntu OS."
else
    echo "unkown OS"
    exit 1
fi


# install from tar
t_path="Raptor_*.tar.gz"

if [ "$dep_install" == "yes" ] && [ "$bypass_dep" == "yes" ]
then
    echo "[ERROR]       Install Dependencies and not install Dependencies can't be true at same time."
    echo "[ERROR]       Aborting Installation"
    exit 2
fi

if [ "$bypass_dep" == "yes" ]
then
    echo "[WARNING]    Not attempting to install dependencies. But Some features of Raptor will not work"

elif [ "$dep_install" == "yes" ] # received yes from command line for OS
then
        are_dep_ok
else # did not receive anything thing regarding dependencies so ask the user
    echo -e "[INFO]         Raptor needs Python 3.6 and Qt plugins for its proper functionality"
    read -r -p "[?]             Would you like to install above dependencies. Need Admin/SUDO rights [y|Y][]n|N] " input
    case $input in
      [yY][eE][sS]|[yY])
            echo "[INFO]    You said Yes."
            are_dep_ok
            ;;
      [nN][oO]|[nN])
            echo "[WARNING]    Not attempting to install dependencies. But Some features of Raptor will not work"
            ;;
      *)
            echo "Invalid input..."
            exit 1
            ;;
    esac
fi 

if [ -z $raptor_h ]
    then
        echo "[INFO]    Raptor install path is not specified."
        read -r -p "[?]     Is /opt good? May need Admin/SUDO Rights [Y/n] " input
        case $input in
            [yY][eE][sS]|[yY])
                raptor_h=/opt
                ;;
            [nN][oO]|[nN])
                read -r -p "[?]     Please type absolute path for installation directory:  " raptor_h    
            ;;
      *)
            echo "Invalid input..."
            exit 1
            ;;
        esac
fi
is_raptor_home_absolute $raptor_h
install_from_tar $t_path $raptor_h $go_verbose
echo -e "[INFO]     Done installing Raptor :)"


