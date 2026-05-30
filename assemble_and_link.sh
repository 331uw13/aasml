
#if ! nasm -f elf64 -o out.obj $1 ; then
#    exit
#fi

if ! as $1 ; then
    exit
fi


if ld -o out a.out; then
    ./out
    echo "Exit code: $?"
fi
rm a.out



