#!/bin/bash

# this check is needed as we need absolute path of work directory
is_w_dir_absolute () {

ok=0
[ "$1" != "${1#/}" ] || ok=1
if [ $ok -eq 1 ]
then
    echo "[ERROR]   Kindly provide absolute path to -w or --work-dir option"
    exit 1
fi

}

usage()
{
  echo "Usage:      $0      [ -h | --help]                 show the help
                             [ -s | --stage ]               Stage of cmake build or install.
                             [ -x | --python3-exe ]         Executable of python 
                             [ -i | --install-dir ]         Specify the Directory where install is happening.
                             [ -g | --git-urls ]            GitHub HTTPs URLs separated by comma within double quotes.
                             [ -r | --requirement-file ]    requirements.txt file.
                             [ -w | --work-dir  ]           Specify the Directory where build is happening."
  exit 2
}

if [ $# -eq 0 ]; then
    echo "No arguments provided"
    usage
    exit 1
fi

PARSED_ARGUMENTS=$(getopt -a -n gen_venv -o hi:s:x:w:g:r: --long help,stage:,python3-exe:,git-urls:,requirement-file:,install-dir:,work-dir: -- "$@")
VALID_ARGUMENTS=$?
if [ "$VALID_ARGUMENTS" != "0" ]; then
  usage
fi
#echo "PARSED_ARGUMENTS is $PARSED_ARGUMENTS"
eval set -- "$PARSED_ARGUMENTS"
while :
do
  case "$1" in
    -w | --work-dir)          w_dir="$2";    shift 2 ;;
    -x | --python3-exe)        py_exe="$2";    shift 2 ;;
    -s | --stage)             point="$2";    shift 2 ;;
    -g | --git-urls)          g_urls="$2";   shift 2 ;;
    -r | --requirement-file)  r_file="$2";   shift 2 ;;
    -i | --install-dir)       i_dir="$2";    shift 2 ;;   
    -h | --help)        usage  ;;
    # -- means the end of the arguments; drop this, and break out of the while loop
    --) shift; break ;;
    # If invalid options were passed, then getopt should have reported an error,
    # which we checked as VALID_ARGUMENTS when getopt was called...
    *) echo "Unexpected option: $1 - this should not happen."
       usage ;;
  esac
done

# this for loop is for debugging purpose to print the given list of packages git URLs
#for url in $(echo $g_urls | sed "s/,/ /g")
#do
#   echo $url
#done

[[ -z $w_dir ]] && { echo "ERROR: Missing working directory"; exit 1; } || echo "Given Work Dir is $w_dir"
is_w_dir_absolute $w_dir
[[ -z $point ]] && { echo "ERROR: Missing CMake stage"; exit 1; }       || echo "Given Stage is $point"

# check pipenv is present
command -v pipenv >/dev/null 2>&1  || { echo >&2; echo "ERROR: pipenv not found. Can be installed by doing $py_exe -m pip install --user pipenv"; exit 1; }
# stage is build
if [ $point == "build" ]
then
  # create envs/litex in build/share and echo absolute path in it
  [[ ! -d $w_dir/share/envs/litex ]] && mkdir -p $w_dir/share/envs/litex 
  cd $w_dir/share/envs/litex && echo "$(pwd)" > .venv && $py_exe -m pipenv install --python 3.8 --no-site-packages
  cd $w_dir/share/envs/litex/bin && ./pip install pipenv
  # create temp directory, clone and build wheel file
  [[ ! -d $w_dir/litex_temp ]] && mkdir -p $w_dir/litex_temp

  if [ ! -z "$g_urls" ]
  then
      for repo in $(echo $g_urls | sed "s/,/ /g")
      do
        repo_name=${repo##*/}
        cd $w_dir/litex_temp && git clone --recursive $repo $repo_name && cd $repo_name && $py_exe setup.py bdist_wheel && wheel_file=$(realpath -s dist/*.whl)
        # install wheel files in virtual env
        #TODO nadeem 09-27-22 Fix the hash mismatch during re-installation of wheel file. HINT: pip.lock file
        cd $w_dir/share/envs/litex && $py_exe -m pipenv install --skip-lock $wheel_file
      done
  fi
  if [ ! -z "$r_file" ]
  then
    [[ ! -f "$r_file" ]] && { echo "$r_file does not exist. Aborting"; exit 1; } 
    cd $w_dir/share/envs/litex/bin && ./pip3 install -r $r_file 
  fi
fi
# stage is install
if [ $point == "install" ]
then
  [[ -z $i_dir ]] && { echo "ERROR: Missing install directory"; exit 1; } || echo "Given Install Dir is $i_dir"
  echo "Install directory is $i_dir"
  [[ ! -d $i_dir/share/envs/litex ]] && mkdir -p $i_dir/share/envs/ 
  $w_dir/share/envs/litex/bin/virtualenv-clone $w_dir/share/envs/litex $i_dir/share/envs/litex
  cd $i_dir/share/envs/litex && echo "$(pwd)" > .venv
fi

