#!/bin/bash
set -e
echo -e "###############################################################################" 
echo -e "\tInstalling Raptor, A Rapid Silicon complete Software solution"
echo -e "###############################################################################"

base_dir=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
go_batch="no"
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
                             [ -B | --batch  ]              Turn on the batch mode.
                             [ -a | --accept  ]             Accept the EULA license.
                             [ -i | --install-dep  ]        Turn on the OS related dependecies installation. 
                             [ -b | --no-dep-install   ]    Turn off the OS related dependecies installation
                             [ -r | --raptor-home  ]        Specify the absolute path of Directory where Raptor will be Installed. Default is /opt"
  exit 2
}


PARSED_ARGUMENTS=$(getopt -a -n install -o hibBar: --long help,batch,accept,install-dep,no-dep-install,raptor-home: -- "$@")
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
    -b | --batch)            go_batch="yes";         shift   ;;
    -a | --accept)            accept_lic="yes";         shift   ;;
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
    echo -e "\n[INFO]\tYou are installing Raptor on CentOS OS."
elif [ "$ostype" == "Ubuntu" ]
then
    echo -e "\n[INFO]\tYou are installing Raptor for Ubuntu OS."
else
    echo "unkown OS"
    exit 1
fi


# install from QtIFW generated .run file
t_path="RaptorInstaller*.run"
if [ $go_batch == "no" ] 
then
    echo -e "[INFO]\tRunning in GUI mode"
    ./$t_path
    exit
fi

if [ "$dep_install" == "yes" ] && [ "$bypass_dep" == "yes" ]
then
    echo "[ERROR]\tInstall Dependencies and not install Dependencies can't be true at same time."
    echo "[ERROR]\tAborting Installation"
    exit 2
fi
if [ -z $raptor_h ]
then
        echo "[ERROR]\tRaptor install path is not specified. Need install path in batch mode"
        exit 2
else
    is_raptor_home_absolute $raptor_h
fi

if [ "$bypass_dep" == "yes" ]
then
    echo "[WARNING]\tNot attempting to install dependencies. But Some features of Raptor will not work"
    ./$t_path --root $raptor_h --accept-licenses --confirm-command --auto-answer Raptor.Dependencies=No  --accept-messages in
elif [ "$dep_install" == "yes" ]
then
    ./$t_path --root $raptor_h --accept-licenses --confirm-command --auto-answer Raptor.Dependencies=Ok --accept-messages in
else 
    echo -e "[ERROR]\tIt is mandatory to give either --install-dep or --no-dep-install"
    exit 2
fi 
echo -e "[INFO]\tDone installing Raptor :)"
