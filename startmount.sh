#bin/sh

echo build programm
make -f Makefile

./mkfs.myfs container.bin /MyFS/fuseTests/testFiles/file1.txt /MyFS/fuseTests/testFiles/file2.txt /MyFS/fuseTests/testFiles/file3.txt /MyFS/fuseTests/testFiles/file4.txt
echo $?
#xxd -c 32 container.bin > hex.txt

mkdir fs

./mount.myfs container.bin log.txt fs

#cd fs
#dd if=file1.txt ibs=1 skip=10 count=10
#tail -n +5 file1.txt
