#! /bin/bash

#version 1.2

#usage: check_language.sh <www_patch>
#eg.: ./check_language.sh ~/git_home/net-cgi.git/www

DIRS="browser_hijack browser_hijack_3g browser_hijack_lte centria_cd_less"
WWW="$1"
LANGUAGE="$WWW/language"
INVALID_DIR="invalid"
IGNORE="$WWW/../files/ignore.txt"
result="success"

#check the equal error. The fail result will write into invalid/syntax.txt.
check_equal() {
	check=`grep -rnv '^\w* *= *".*" *$\|^\w* *= *".*" *//.*$\|^\s*//\|^\s*$\|^\xef\xbb' $LANGUAGE | tee -a $INVALID_DIR/syntax.txt`
	[ -n "$check" ] && result="fail" || rm "$INVALID_DIR/syntax.txt"
}

#check the quote error. The fail result will write into invalid/syntax.txt.
check_quote() {
	egrep -rn --exclude-dir='.git' '\".*[^\]\".*\"'  "$LANGUAGE" > ./check_language.tmp
	while read line ; do
		#num=$(echo $line | sed 's/\\"//g' | awk -F '"' '{print NF-1}')
		#if [ `expr $num % 2` != 0 ] ; then
		echo $line >> $INVALID_DIR/syntax.txt && result="fail"
		#fi
	done < ./check_language.tmp
	rm ./check_language.tmp
}

list_file() {
	for dir in $DIRS; do
		path="$WWW/$dir"
		[ -d $path ] && find $path -type f 
	done
}

list_variate() {
	list_file | xargs perl -ne '1 while(/document.(?:write|createTextNode)\((?:"[^+]+"\s*\+\s*)?([^+) ]+)(?:\s*\+\s*)?([^+ )"]+)?(?:\s*\+\s*"[^+]+")?\)(?{print "$1\n";print "$2\n" if $2})/g)' | grep -v '\W' | sort | uniq
}

#check the variable error. The fail result will write into invalid/invalid_*.js.
check_variable() {
	invalid_file="$INVALID_DIR/invalid.txt"
	#[ -f $invalid_file ] && rm $invalid_file
	#[ -f "$INVALID_DIR/invalid_English.js" ] && rm "$INVALID_DIR/invalid_English.js"
	list_variate > ./check_language.tmp
	while read line ; do
		grep -r "^\<$line\>" "$LANGUAGE/" > /dev/null
		if [ $? -eq 1 ]; then
		       	[ -f "$IGNORE" ] && grep "\<$line\>" "$IGNORE" >/dev/null && continue
			echo $line >> $invalid_file && result="fail" && continue 
		fi
		flag=3
		grep "^\<$line\>" "$LANGUAGE/English.js" > /dev/null && flag=1
		grep "^\<$line\>" "$LANGUAGE/RU_flow_en.js" > /dev/null && flag=2
		for file in `ls "$LANGUAGE" | grep -v "English.js\|RU_flow_en.js"`; do
			invalid_language_file="$INVALID_DIR/invalid_$file"
			#[ -f $invalid_language_file ] && rm $invalid_language_file
			grep "^\<$line\>" "$LANGUAGE/$file" > /dev/null
			if [ $? -eq 1 ] ; then
				[ "$flag" == "1" -a "$file" != "RU_flow.js" ] && echo "$line" >> "$invalid_language_file" && result="fail"
				[ "$flag" == "2" -a "$file" == "RU_flow.js" ] && echo "$line" >> "$invalid_language_file" && result="fail"
			fi
		done
		[ "$flag" == "3" ] && echo "$line" >> "$INVALID_DIR/invalid_English.js" && result="fail"
	done < ./check_language.tmp
	rm ./check_language.tmp
}

main() {
	[ -d $INVALID_DIR ] && rm -rf $INVALID_DIR
	mkdir $INVALID_DIR

	check_equal
	check_quote
	check_variable

	echo $result
}

main
