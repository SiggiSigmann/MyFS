#bin/sh

echo make files
mkdir -p test
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

echo execute
./mkfs.myfs container.bin folder1/file1.txt file2.txt file3.txt

#xxd -c 32 test.bin > hex.txt

mkdir fs

./mount.myfs container.bin log.txt fs
