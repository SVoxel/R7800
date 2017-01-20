# stainfo.awk

$1 !~ /ADDR/ {
    printf "%s\t%sbps\t%s\n", $1, $4, $6
}
