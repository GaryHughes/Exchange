#! /bin/sh

# Some useful file-handling functions

remove_first_line () {
    # Remove the first line fo the file "$1"
    sed 1d < "$1" > "$1".new && mv "$1".new "$1"
}

replace_first_line () {
    # replace the first line of filr $1 with the value of $2
    echo $2 > $1.new
    sed 1d $1 >> $1.new
    mv $1.new $1
}

shell_copy () {
    while read line ; do echo $line ; done < $1 > $2
}

move () {
    mv $1 $2
}

move_if_not_empty () {
    # move $1 to $2, or delete both if $1 is empty
    if [ -s "$1" ]; then
        mv "$1" "$2"
    else
        rm "$1" "$2"
    fi
}
