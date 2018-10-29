#bin/sh
pwd
cd /MyFS/
make -f Makefile
./mkfs.myfs container.bin /MyFS/fuseTests/testFiles/file1.txt /MyFS/fuseTests/testFiles/file2.txt /MyFS/fuseTests/testFiles/file3.txt /MyFS/fuseTests/testFiles/file4.txt
./mount.myfs container.bin log.txt fs -s
