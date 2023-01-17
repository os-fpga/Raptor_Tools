ostype=`egrep '^(NAME)=' /etc/os-release  | grep -o -e Ubuntu -e CentOS`
dep_list_redhat="gperftools libxcb xcb-util libxkbcommon-devel libxkbcommon-x11-devel xcb-util-image-devel xcb-util-keysyms-devel xcb-util-renderutil-devel xcb-util-wm-devel gtk3"
dep_list_debian="libgoogle-perftools-dev qtbase5-dev libgtk-3-0"
#echo "[INFO]    Installing Raptor on $ostype"
if [ "$ostype" == "CentOS" ]
then
    [ `which yum` ] && echo "Package manager is yum" || { echo "Fail to find package manager. Manually install below dependencies"; echo $dep_list_redhat; exit 1; }
    for dep in $dep_list_redhat
    do
        echo "$dep"
        sudo yum install $dep
    done
elif [ "$ostype" == "Ubuntu" ]
then
    [ `which apt` ] && echo "Package manager is apt" || { echo "Fail to find package manager. Manually install below dependencies"; echo $dep_list_debian; exit 1; }
    for dep in $dep_list_debian
    do
        echo "$dep"
        sudo apt install $dep
    done 
else
    echo "Unkown OS. Manually install below dependencies."
    echo $dep_list_redhat
    echo "Make sure to find equivalent name of above packages for you OS or contact at support@rapidsilicon.com"
    exit 1
fi