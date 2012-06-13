UPLOAD="googlecode_upload.py"

die() {
  echo >&2 "$@"
  exit 1
}

upload_file() {
    codename=$1
    message="$2"
    labels="$3"
    filename="$4"
    user=$(hg showconfig auth.$codename.username)
    pass=$(hg showconfig auth.$codename.password)
    $UPLOAD -s "$message" --project=$codename --user=$user --password=$pass \
        -l "$labels" $filename || die "Unable to upload $filename"
}

check_devkitpro() {
    if [ -z "$DEVKITPRO" ] ; then
        echo "No DEVKITPRO variable set. Set the path to devkit pro."
        exit 1
    fi
    if [ ! -d $DEVKITPRO ] ; then
        mkdir -p $DEVKITPRO || die "Unable to create $DEVKITPRO directory"
    fi
}
