const expect = require("chai").expect;
const chai = require("chai");
const fs = require("fs");
const path = require("path");
const shell = require("shelljs");
const chalk = require("chalk");
const crypto = require("crypto");

let basePath = "../fs/";
let testPath = "./testFiles";

describe("Myfs", async function () {
    it('generate 4 testfiles', () => {
        if (!fs.existsSync(basePath)) {
            fs.mkdirSync(basePath);
        }
        if (!fs.existsSync(testPath)) {
            fs.mkdirSync(testPath);
        }
        for (let i = 1; i < 5; i++) {
            token = crypto.randomBytes(5000).toString('hex');
            fs.writeFileSync(path.join(testPath, "file" + i + ".txt"), token, {
                flag: 'w'
            });
            /*
            fs.writeFileSync(path.join(basePath, "file" + i + ".txt"), token, {
                flag: 'w'
            });
            */

        }
    });
    it("test if File System is mounted", function (done) {
        if (fs.readdirSync(basePath).length != 0) {
            expect(true).to.be.true;
            done();
        } else {
            console.log(chalk.blue("\tmounting container.bin..."));
            shell.exec("bash mount.sh", {
                silent: true
            });
            expect(fs.existsSync(basePath)).to.be.true;
            done();
        }
    }).timeout(25000);
    it("check if container.bin has files", function () {
        let files = fs.readdirSync(basePath);
        expect(files).to.be.not.empty;
        //fs.writeFile('file', new Buffer(1024*1024*54));
    });
    describe.skip("test reading files", function () {
        it("Compare container bin to testFiles", function () {
            compareDirectorys()
        });
        it("read non-existing files", function () {
            expect(function () {
                fs.readFileSync(path.join(basePath, "not Existing"));
            }).to.throw(
                "ENOENT: no such file or directory, open '../fs/not Existing'"
            );
        });
        it("bulk read files", async function () {
            let testContents = [];
            fs.readdirSync(testPath).forEach(file => {
                testContents.push(fs.readFileSync(path.join(testPath, file), "utf8"));
            });
            let containerFiles = fs.readdirSync(basePath);
            for (let j = 0; j < 100; j++) {
                for (const i in containerFiles) {
                    let content = fs.readFileSync(path.join(basePath, containerFiles[i]), "utf8");
                    expect(content).to.be.equal(testContents[i]);
                }
            }
        });
        describe.skip("Read:", async () => {
            let txtFile = path.join(basePath, "file1.txt");
            let txtTest = path.join(testPath, "file1.txt");
            describe('without offset:', () => {
                it("Exact", async () => {
                    compareTestToContainer(txtFile, txtTest, 0, 512);
                });
                it("Half", async () => {
                    compareTestToContainer(txtFile, txtTest, 0, 256);
                });
                it("Long", async () => {
                    compareTestToContainer(txtFile, txtTest, 0, 20 * 512);
                });
                it("short", async () => {
                    compareTestToContainer(txtFile, txtTest, 0, 20 * 600);
                });
                it("nothing", async () => {
                    compareTestToContainer(txtFile, txtTest, 0, 0);
                });
            });
            describe('with offset:', () => {
                it("Exact", async () => {
                    compareTestToContainer(txtFile, txtTest, 512, 512);
                });
                it("Half", async () => {
                    compareTestToContainer(txtFile, txtTest, 256, 256);
                });
                it("One", async () => {
                    compareTestToContainer(txtFile, txtTest, 500, 12);
                });
                it("Long", async () => {
                    compareTestToContainer(txtFile, txtTest, 512 * 2, 20 * 512);
                });
                it("short", async () => {
                    compareTestToContainer(txtFile, txtTest, 20, 20);
                });
            });
            describe('with oneBlock offset:', () => {
                it("Exact", async () => {
                    compareTestToContainer(txtFile, txtTest, 512, 512);
                });
                it("Half", async () => {
                    compareTestToContainer(txtFile, txtTest, 512, 256);
                });
                it("Long", async () => {
                    compareTestToContainer(txtFile, txtTest, 512, 20 * 512);
                });
                it("short", async () => {
                    compareTestToContainer(txtFile, txtTest, 512, 20);
                });
                it("nothing", async () => {
                    compareTestToContainer(txtFile, txtTest, 512, 0);
                });
            });
        });

    });

    describe('Write:', () => {
        let smallString = "test";
        let oneBlockString = new Array(512 + 1).join("#");
        let largeString = crypto.randomBytes(20).toString('hex');
        describe('append:', () => {
            it('append: short', () => {
                let files = fs.readdirSync(basePath);
                files.forEach(file => {
                    append(path.join(basePath, file), smallString);
                    append(path.join(testPath, file), smallString);
                });
                compareDirectorys();
            });
            it('append: long', () => {
                let files = fs.readdirSync(basePath);
                files.forEach(file => {
                    append(path.join(basePath, file), largeString);
                    append(path.join(testPath, file), largeString);
                });
                compareDirectorys();
            });
        });
        describe('write:', () => {
            it('short', () => {
                let files = fs.readdirSync(basePath);
                files.forEach(file => {
                    writeByByte(path.join(basePath, file), 0, smallString);
                    writeByByte(path.join(testPath, file), 0, smallString);
                });
                compareDirectorys();
            });
            it('block', () => {
                let files = fs.readdirSync(basePath);
                files.forEach(file => {
                    writeByByte(path.join(basePath, file), 0, oneBlockString);
                    writeByByte(path.join(testPath, file), 0, oneBlockString);
                });
                compareDirectorys();
            });
            it('long', () => {
                let files = fs.readdirSync(basePath);
                files.forEach(file => {
                    writeByByte(path.join(basePath, file), 0, largeString);
                    writeByByte(path.join(testPath, file), 0, largeString);
                });
                compareDirectorys();
            });
        });
        describe('Write one block offset', () => {
            it('short', () => {
                let files = fs.readdirSync(basePath);
                files.forEach(file => {
                    writeByByte(path.join(basePath, file), 512, smallString);
                    writeByByte(path.join(testPath, file), 512, smallString);
                });
                compareDirectorys();
            });
            it('block', () => {
                let files = fs.readdirSync(basePath);
                files.forEach(file => {
                    writeByByte(path.join(basePath, file), 512, oneBlockString);
                    writeByByte(path.join(testPath, file), 512, oneBlockString);
                });
                compareDirectorys();
            });
            it('long', () => {
                let files = fs.readdirSync(basePath);
                files.forEach(file => {
                    writeByByte(path.join(basePath, file), 512, largeString);
                    writeByByte(path.join(testPath, file), 512, largeString);
                });
                compareDirectorys();
            });
        });
        describe('Write offset', () => {
            it('short', () => {
                //let files = fs.readdirSync(basePath);
                let files = ['file1.txt', 'file2.txt', 'file3.txt'];
                files.forEach(file => {
                    writeByByte(path.join(basePath, file), 2000, smallString);
                    writeByByte(path.join(testPath, file), 2000, smallString);
                });
                compareDirectorys();
            });
            it('block', () => {
                let files = fs.readdirSync(basePath);
                files.forEach(file => {
                    writeByByte(path.join(basePath, file), 2000, oneBlockString);
                    writeByByte(path.join(testPath, file), 2000, oneBlockString);
                });
                compareDirectorys();
            });
            it('long', () => {
                let files = fs.readdirSync(basePath);

                files.forEach(file => {
                    writeByByte(path.join(basePath, file), 2000, largeString);
                    writeByByte(path.join(testPath, file), 2000, largeString);
                });
                compareDirectorys();
            });
        });
    });
    describe.skip('delete files:', () => {
        it('delete all files in directory', () => {
            let files = fs.readdirSync(basePath);
            files.forEach(file => {
                fs.unlinkSync(path.join(basePath, file));
                fs.unlinkSync(path.join(testPath, file));
            });
        });
        it("delete not existing file", () => {
            expect(function () {
                fs.unlinkSync(path.join(testPath, "none"));
            }).to.throw(
                "ENOENT: no such file or directory, unlink 'testFiles/none"
            );

        })
    });
    describe.skip("create new files:", () => {
        it('generate 64 small test files', () => {
            for (let i = 1; i < 65; i++) {
                token = crypto.randomBytes(513).toString('hex');
                fs.writeFileSync(path.join(testPath, "file" + i + ".txt"), token, {
                    flag: 'w'
                });

                fs.writeFileSync(path.join(basePath, "file" + i + ".txt"), token, {
                    flag: 'w'
                });
            }
            let file1 = fs.readdirSync(basePath);
            let file2 = fs.readdirSync(testPath);
            expect(file1).to.have.same.members(file2);
        });
    });
    describe.skip('test time', () => {
        describe('atime:', () => {
            it('read:', () => {
                let file = path.join(basePath, "file1.txt");
                let atime = fs.statSync(file).atimeMs;
                readByByte(file, 0, 512);
                expect(atime).to.be.not.equal(fs.statSync(file).atimeMs);
            });
        });
        describe('mtime:', () => {
            it('append:', () => {
                let file = path.join(basePath, "file1.txt");
                let mtime = fs.statSync(file).mtimeMs;
                append(file, "#kmd")
                expect(mtime).to.be.not.equal(fs.statSync(file).mtimeMs);
            });
            it('write:', () => {
                let file = path.join(basePath, "file1.txt");
                let mtime = fs.statSync(file).mtimeMs;
                writeByByte(file, 0, "test")
                expect(mtime).to.be.not.equal(fs.statSync(file).mtimeMs);
            });
        });
        describe('ctime', () => {
            it('append:', () => {
                let file = path.join(basePath, "file1.txt");
                let ctimeMs = fs.statSync(file).ctimeMs;
                append(file, "#kmd")
                expect(ctimeMs).to.be.not.equal(fs.statSync(file).ctimeMs);
            });
            it('write:', () => {
                let file = path.join(basePath, "file1.txt");
                let ctime = fs.statSync(file).ctimeMs;
                writeByByte(file, 0, "test")
                expect(ctime).to.be.not.equal(fs.statSync(file).ctimeMs);
            });
        });
    });
    describe.skip('cleanup', () => {
        it('delete all files in base directory', () => {
            let files = fs.readdirSync(basePath);
            files.forEach(file => {
                fs.unlinkSync(path.join(basePath, file));
            });
        });
        it('delete all files in test directory', () => {
            let files = fs.readdirSync(testPath);
            files.forEach(file => {
                fs.unlinkSync(path.join(testPath, file));
            });
        });
    });
});

function compareTestToContainer(file, testFile, pos, length) {
    let chunk1 = readByByte(file, pos, length);
    let chunk2 = readByByte(testFile, pos, length);
    expect(chunk1.toString('hex')).to.be.equal(chunk2.toString('hex'));

}

function readByByte(file, pos, length) {
    let fd = fs.openSync(file, "rs");
    let buffer = Buffer.alloc(length);
    fs.readSync(fd, buffer, 0, length, pos);
    fs.fsyncSync(fd);
    fs.closeSync(fd);
    return buffer
}

function writeByByte(file, pos, string) {
    let fd = fs.openSync(file, "w");
    let buffer = Buffer.from(string, 'utf8');
    fs.writeSync(fd, buffer, 0, string.length, pos);
    fs.fsyncSync(fd);
    fs.closeSync(fd);
}

function append(file, string) {
    let fd = fs.openSync(file, "a");
    let buffer = Buffer.from(string, 'utf8');
    fs.writeSync(fd, buffer, 0, buffer.length, null);
    fs.fsyncSync(fd);
    fs.closeSync(fd);
}

function compareDirectorys() {
    fs.readdirSync(basePath).forEach(file => {
        expect(fs.readFileSync(path.join(basePath, file), "utf8")).to.be.equal(
            fs.readFileSync(path.join(testPath, file), "utf8")
        );
    });
}
