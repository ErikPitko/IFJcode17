#!/usr/bin/env bash

START=$(date +%s)

help() {
printf \\n%s\\n "Spusteni testu:
	$0		Spusti skript dirgraph pomoci shellu bash
	$0 -h		Vypise tento help
	$0 -c		Uklid souboru vytvorenych v prubehu testovani
				maze soubory *-my a *-error	
Jak to funguje:
	Pro kazde JMENO testu (pouzivaji se cisla) vytvori nazvy souboru:
		testJMENO-out
		testJMENO-my
		testJMENO-error"
}

file='IFJcode17'
interpret='ic17int'


# Output settings
TEXT_BOLD=`tput bold`
TEXT_GREEN=`tput setaf 2`
TEXT_RED=`tput setaf 1`
TEXT_RESET=`tput sgr0`
TEXT_BLUE=`tput setaf 4`
TEXT_BROWN=`tput setaf 3`

# IO file names
function errorFileName () {
    printf "${1}-error"
}
function inFileName () {
    printf "${1}.code"
}
function outFileName () {
    printf "${1}-out"
}
function myFileName () {
    printf "${1}.stdout"
}
function intOutFileName (){
	printf "${1}-interpret"
}
function diffFileName (){
	printf "${1}-diff"
}

# Test initialization
function initTest () {
    testNumber=$1
    testError=$(errorFileName $testNumber)
    testOut=$(outFileName $testNumber)
    testIn=$(inFileName $testNumber)
    testMy=$(myFileName $testNumber)
	testIntOut=$(intOutFileName $testNumber)
	testDiff=$(diffFileName $testNumber)
}


function green() {
    printf %s "${TEXT_GREEN}$1${TEXT_RESET}"
}

function red() {
    printf %s "${TEXT_RED}$1${TEXT_RESET}"
}


function isOk () {
    local testNumber=$1
    local   testExit=$2
    local  testError=$3
	local  returnErr=$4

	#echo "$testNumber"
	#echo "${testNumber#0}"
    printf "Test %02d	.." "${testNumber#0}"

    if [ -s ${testError} ]; then
        printf %s "isFail ExitCode: " 
        [ $testExit -eq $returnErr ] && green $testExit || red $testExit
#        printf %s ", error output: $([ -s $testError ] && green "found" || red "not found")"
        
        #err=1
        #[ $testExit != 0 ] && [ -s $testError ] && err=0 # True, if <FILE> exists and has size bigger than 0 (not empty).

		#[ $err -eq 0 ] && printf "$TEXT_BROWN" && /bin/cat $testError
    
    else
        printf %s "isOK   ExitCode: "
        [ $testExit -eq $returnErr ] && green $testExit || red $testExit
		
		if [ $testExit -eq 0 ] 
		then
			`./${interpret} ${testOut} > ${testIntOut} 2>> ${testError}`
        	`$diff --strip-trailing-cr -u ${testIntOut} ${testMy} > ${testDiff}`
        	local diffResult=$?
        	printf %s\\n ",       output: $([ $diffResult -eq 0 ] && green 'not diff' || red 'diff')"
			
			if [ $diffResult -ne 0 ]
			then
        		$cat ${testIntOut} ${testDiff}
			fi
		fi

        #err=1
        #[ $testExit == 0 ] && [ $diffResult == 0 ] && err=0

    fi

	printf " - "
    [ $testExit -eq $returnErr ] && green "ok" || red "fail"
	printf "\n"

}



test() {
    no=$1
	ret=$2

    initTest ${no}

	`./${file} < ${testIn} > ${testOut} 2> ${testError}`

	isOk $testNumber $? $testError $ret
    
#    printf \\t%s\\n\\n "${TEXT_BLUE}$info${TEXT_RESET}"

}


file_exists() {
	if [ -e "$1" ]
	then
    		printf "${TEXT_BOLD}Testing file ${1}${TEXT_RESET}\n"
	else
    		printf "${TEXT_RED}Cannot run test without file ${1}.${TEXT_RESET}\n"
    		exit 1
	fi
}



# Cleaning
if [ "$1" == "-c" ]
then
	ls | grep '\.\(out\|error\|diff\|interpret\)$' | xargs -d "\n" rm
	exit 0
fi

file_exists "$file"
file_exists "$interpret"

diff=$(which diff)
cat=$(which cat)

#info="Zadany 2 parametry bez -i a -n."
test 01 0
test 02 0
test 03 0
test 04 0
test 05 0
test 06 0
test 07 0
test 08 0
test 09 0
test 10 0
test 11 0
test 12 0
test 13 0
test 14 0
test 15 0
test 16 0
test 17 0
test 18 0
test 19 0
test 20 0
test 21 0
test 22 0
test 23 0
test 24 0
test 25 0
test 26 0
test 27 0
test 28 0
test 29 0
test 30 0
test 31 0
test 32 0
test 33 0
test 34 0
test 35 0
test 36 0
test 37 0
test 38 0
test 39 0
test 40 0
test 41 0
test 42 0
test 43 0
test 44 0
test 45 0
test 46 0
test 47 0
test 48 0
test 49 0
test 50 0
test 51 0
test 52 0
test 53 0
test 54 0
test 55 0
test 56 0
test 57 0
test 58 0
test 59 0
test 60 0
test 61 4
test 62 4
test 63 0
test 64 0
test 65 0
test 66 0
test 67 0
test 68 2
test 69 0
test 70 0
test 71 0
test 72 0
test 73 0
test 74 3
test 75 3
test 76 0
test 77 0
test 78 0
test 79 0
test 80 0
test 81 0
test 82 0
test 83 0
test 84 0
test 85 0
test 86 0
test 87 0
test 88 0
test 89 0
test 90 0
test 91 0
test 92 0
test 93 0
test 94 0
test 95 0
test 96 0
test 97 0
test 98 0
test 99 0
test 100 0
test 101 0
test 102 0
test 103 0
test 104 0
test 105 0
test 106 0
test 107 0
test 108 0
test 109 0
test 110 0
test 111 1
test 112 1
test 113 1
test 114 1
test 115 1
test 116 1
test 117 1
test 118 1
test 119 1
test 120 1
test 121 1
test 122 1
test 123 1
test 124 1
test 125 1
test 126 1
test 127 2
test 128 2
test 129 2
test 130 2
test 131 2
test 132 2
#test 133 2
test 134 2
test 135 2
test 136 2
test 137 2
test 138 2
test 139 2
test 140 2
#test 141 2
# ---------------------------

END=$(date +%s)
printf "Test trval vterin: $(( $END - $START ))\n"
