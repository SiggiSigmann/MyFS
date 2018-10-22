#bin/sh

echo make files
mkdir -p folder1
echo buuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu ich bin der Pape >> folder1/file1.txt
echo buuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu ich bin der Pape >> folder1/file1.txt
echo buuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu ich bin der Pape >> folder1/file1.txt
echo buuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu ich bin der Pape >> folder1/file1.txt
echo buuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu ich bin der Pape >> folder1/file1.txt
echo buuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu ich bin der Pape >> folder1/file1.txt
echo buuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu ich bin der Pape >> folder1/file1.txt
echo buuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu ich bin der Pape >> folder1/file1.txt
echo buuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu ich bin der Pape >> folder1/file1.txt
echo buuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu ich bin der Pape >> folder1/file1.txt
echo buuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu ich bin der Pape >> folder1/file1.txt
echo buuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu ich bin der Pape >> folder1/file1.txt
echo buuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu ich bin der Pape >> folder1/file1.txt
echo buuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu ich bin der Pape >> folder1/file1.txt

echo sie haben keinen code hochgeladen >> file2.txt
echo sie haben keinen code hochgeladen >> file2.txt
echo sie haben keinen code hochgeladen >> file2.txt
echo sie haben keinen code hochgeladen >> file2.txt
echo sie haben keinen code hochgeladen >> file2.txt
echo sie haben keinen code hochgeladen >> file2.txt
echo sie haben keinen code hochgeladen >> file2.txt

echo wir brauchen mehr tests >> file3.txt
echo wir brauchen mehr tests >> file3.txt
echo wir brauchen mehr tests >> file3.txt
echo wir brauchen mehr tests >> file3.txt
echo wir brauchen mehr tests >> file3.txt
echo wir brauchen mehr tests >> file3.txt
echo wir brauchen mehr tests >> file3.txt
echo wir brauchen mehr tests >> file3.txt

echo bild programm
make -f Makefile

echo =======>execute
./mkfs.myfs container.bin /MyFS/folder1/file1.txt /MyFS/file2.txt /MyFS/file3.txt
echo $?
echo =======>execute
#xxd -c 32 container.bin > hex.txt

mkdir fs

./mount.myfs container.bin log.txt fs

cd fs
cat file1.txt
cat /MyFS/log.txt
