readonly RED='\E[31;1m'
readonly GREEN='\E[32;1m'
readonly MAGENTA='\E[35;1m'
readonly CYAN='\E[36;1m'
readonly CEND='\E[0;m'

# $1: color
# $2: message
color_output() {
    echo -e "$1$2$CEND"
}

# $1: message
log() {
    color_output "$GREEN" "$1"
}

# $1: message
mark() {
    color_output "$CYAN" "\n>>> $1"
}

# $1: error status
# $2: function or operation name
check_err() {
    if [ $1 -ne 0 ]; then
        color_output "$RED" "$2 fail"
        exit 1
    fi
}

