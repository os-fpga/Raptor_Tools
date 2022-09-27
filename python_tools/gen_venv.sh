#!/bin/bash

usage()
{
  echo "Usage:      $0      [ -h | --help]             show the help
                             [ -s | --stage ]           Stage of cmake build or install.
                             [ -w | --work-dir  ]       Specify the Directory where build or installation is happening."
  exit 2
}

if [ $# -eq 0 ]; then
    echo "No arguments provided"
    usage
    exit 1
fi

PARSED_ARGUMENTS=$(getopt -a -n gen_venv -o hs:w: --long help,stage:,work-dir: -- "$@")
VALID_ARGUMENTS=$?
if [ "$VALID_ARGUMENTS" != "0" ]; then
  usage
fi
#echo "PARSED_ARGUMENTS is $PARSED_ARGUMENTS"
eval set -- "$PARSED_ARGUMENTS"
while :
do
  case "$1" in
    -w | --work-dir)  w_dir="$2";   shift 2 ;;
    -s | --stage)     point="$2";   shift 2 ;;   
    -h | --help)        usage  ;;
    # -- means the end of the arguments; drop this, and break out of the while loop
    --) shift; break ;;
    # If invalid options were passed, then getopt should have reported an error,
    # which we checked as VALID_ARGUMENTS when getopt was called...
    *) echo "Unexpected option: $1 - this should not happen."
       usage ;;
  esac
done


git_urls="${@:1}"
for var in "$git_urls"
do
    echo "$var"
done
#exit

[[ -z $w_dir ]] && { echo "ERROR: Missing working directory"; exit 1; } || echo "Given Work Dir is $w_dir"
[[ -z $point ]] && { echo "ERROR: Missing CMake stage"; exit 1; }       || echo "Given Stage is $point"

# check pipenv is present
command -v pipenv >/dev/null 2>&1  || { echo >&2; echo "ERROR: pipenv not found. Can be installed by doing python3 -m pip install --user pipenv"; }
# stage is build
if [ $point == "build" ]
then
# create envs/litex in build/share and echo absolute path in it
[[ ! -d $w_dir/share/envs/litex ]] && mkdir -p $w_dir/share/envs/litex 
cd $w_dir/share/envs/litex && echo "$(pwd)" > .venv && python3 -m pipenv install --no-site-packages
# create temp directory, clone and build wheel file
[[ ! -d $w_dir/litex_temp ]] && mkdir -p $w_dir/litex_temp
i=1
for repo in $git_urls
do
cd $w_dir/litex_temp && git clone --recursive $repo $i && cd $i && python3 setup.py bdist_wheel && wheel_file=$(realpath -s dist/*.whl)
# install wheel files in virtual env
cd $w_dir/share/envs/litex && python3 -m pipenv install --skip-lock $wheel_file
# next repo
i=$(expr $i + 1)
done
fi
# stage is install
if [ $point == "install" ]
then
echo "Install directory is $w_dir"
cd $w_dir/share/envs/litex && echo "$(pwd)" > .venv
fi