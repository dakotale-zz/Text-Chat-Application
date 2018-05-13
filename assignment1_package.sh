#!/bin/bash
       
#Title           :assignment1_package.sh
#description     :This script will package assignment1 for submission.
#Author		     :Swetank Kumar Saha <swetankk@buffalo.edu>
#Version         :1.0
#===================================================================================================

# https://gist.github.com/davejamesmiller/1965569
function ask {
    while true; do
 
        if [ "${2:-}" = "Y" ]; then
            prompt="Y/n"
            default=Y
        elif [ "${2:-}" = "N" ]; then
            prompt="y/N"
            default=N
        else
            prompt="y/n"
            default=
        fi
 
        # Ask the question
        read -p "$1 [$prompt] " REPLY
 
        # Default?
        if [ -z "$REPLY" ]; then
            REPLY=$default
        fi
 
        # Check if the reply is valid
        case "$REPLY" in
            Y*|y*) return 0 ;;
            N*|n*) return 1 ;;
        esac
 
    done
}

echo
echo -n "Enter your UBIT username (without the @buffalo.edu part) and press [ENTER]: "
read ubitname

if [ -d "./${ubitname}" ]; 
then
    echo "Directory with given UBITname exists"
else
    echo "No directory named ${ubitname} found. Try again!"
    exit 0
fi

echo "Verifying contents ..."

echo
echo "C/C++ file with main function: "
FILE=`find ./$ubitname/src/ -name "${ubitname}_assignment1.c" -o -name "${ubitname}_assignment1.cpp"`
if [ -n "$FILE" ];
then
    echo "File $FILE exists"
    if grep -q "int main[ ]*([^\)]*)" $FILE
    then
        echo "File $FILE contains a 'int main()' function definition"
    else
        echo "File $FILE does NOT contain the 'int main()' function definition"
        exit 0
    fi
else
    echo "Missing main C file or file named incorrectly!"
    exit 0
fi

echo
echo "Makefile: "
FILE=./$ubitname/Makefile
if [ -f $FILE ];
then
    echo "File $FILE exists"
else
    echo "Missing Makefile or file named incorrectly!"
    exit 0
fi

echo
echo "Packaging ..."
cd ${ubitname}/ && tar --exclude='./logs' -zcvf ../${ubitname}_pa1.tar * && cd ..
echo "Done!"
echo "IMPORTANT: Your submission is NOT done!"
echo "You need to use the submit_cse489/submit_cse589 (available on CSE servers) script to submit this tarball."
