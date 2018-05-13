#!/bin/bash

#Title           :assignment1_init_script.sh
#description     :This script will download and install the template for assignment1.
#Author          :Swetank Kumar Saha <swetankk@buffalo.edu>
#Version         :1.1
#====================================================================================

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

echo -n "Enter your UBIT username (without the @buffalo.edu part) and press [ENTER]: "
read ubitname
echo -n "Enter your Full name and press [ENTER]: "
read fullname

while true; do
    echo -n "Enter 1 (for C) OR 2 (for C++): "
    read -n 1 lang_choice

    if [ -z $lang_choice ]; then
        continue
    elif [ $lang_choice == "1" ]; then
            language="C"
            lang_option="c"
        break
    elif [ $lang_choice == "2" ]; then
            language="C++"
            lang_option="cpp"
        break
    else
        continue
    fi
done

echo
echo
echo "UBIT username: $ubitname"
echo "Full name: $fullname"
echo "Programming language: $language"

if ask "Do you want to continue?" Y; then
    if [ -d "$ubitname" ]; then
        echo
          echo "I see a directory with your ubitname already exists."
          echo "Running the init script again will over-write all your existing work."
          if ask "Are you sure, you want to continue?" N; then
              continue
          else
              exit 0
          fi
    fi

    wget --no-check-certificate https://ubwins.cse.buffalo.edu/cse-489_589/assignment1_package.sh
    chmod +x assignment1_package.sh

    wget --no-check-certificate https://ubwins.cse.buffalo.edu/cse-489_589/assignment1_update_grader.sh
    chmod +x assignment1_update_grader.sh

    wget --no-check-certificate https://ubwins.cse.buffalo.edu/cse-489_589/assignment1_template_${lang_option}.tar
    tar -xvf assignment1_template_${lang_option}.tar

    mv ./ubitname $ubitname

    sed -i "s/ubitname/$ubitname/g" ./$ubitname/Makefile

    mv ./$ubitname/src/ubitname_assignment1.${lang_option} ./$ubitname/src/${ubitname}_assignment1.${lang_option}
    sed -i "s/ubitname/$ubitname/g" ./$ubitname/src/${ubitname}_assignment1.${lang_option}
    sed -i "s/Fullname/$fullname/g" ./$ubitname/src/${ubitname}_assignment1.${lang_option}

    rm assignment1_template_${lang_option}.tar

    mkdir grader
    cd grader
    wget --no-check-certificate -r --no-parent -nH --cut-dirs=3 -R index.html https://ubwins.cse.buffalo.edu/cse-489_589/grader/
    chmod +x grader_controller
    cd ..

    echo
    echo "Installation ... Done!"
else
    exit 0
fi
