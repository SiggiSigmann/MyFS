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
        for (let i = 1; i < 5; i++) {
            token = crypto.randomBytes(5000).toString('hex');
            fs.writeFileSync(path.join(testPath, "file" + i + ".txt"), token, {
                flag: 'w'
            });

            fs.writeFileSync(path.join(basePath, "file" + i + ".txt"), token, {
                flag: 'w'
            });

        }
    });
    it("test if File System is mounted", function (done) {
        if (fs.existsSync(basePath)) {
            expect(true).to.be.true;
            done();
        } else {
            console.log(chalk.blue("\tmounting container.bin..."));
            shell.exec("cd .. && bash startmount.sh", {
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
    describe("test reading files", function () {
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
        describe("Read:", async () => {
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
        let largeString = crypto.randomBytes(20000).toString('hex');
        let files = fs.readdirSync(basePath);
        describe('append:', () => {


            let files = fs.readdirSync(basePath);
            it('append: short', () => {
                files.forEach(file => {
                    append(path.join(basePath, file), smallString);
                    append(path.join(testPath, file), smallString);
                });
                compareDirectorys();
            });
            it('append: long', () => {
                files.forEach(file => {
                    append(path.join(basePath, file), largeString);
                    append(path.join(testPath, file), largeString);
                });
                compareDirectorys();
            });
        });
        describe('write:', () => {
            it('short', () => {
                files.forEach(file => {
                    writeByByte(path.join(basePath, file), 0, smallString);
                    writeByByte(path.join(testPath, file), 0, smallString);
                });
                compareDirectorys();
            });
            it('block', () => {
                files.forEach(file => {
                    writeByByte(path.join(basePath, file), 0, oneBlockString);
                    writeByByte(path.join(testPath, file), 0, oneBlockString);
                });
                compareDirectorys();
            });
            it('long', () => {
                files.forEach(file => {
                    writeByByte(path.join(basePath, file), 0, largeString);
                    writeByByte(path.join(testPath, file), 0, largeString);
                });
                compareDirectorys();
            });
        });
        describe('Write one block offset', () => {
            it('short', () => {
                files.forEach(file => {
                    writeByByte(path.join(basePath, file), 512, smallString);
                    writeByByte(path.join(testPath, file), 512, smallString);
                });
                compareDirectorys();
            });
            it('block', () => {
                files.forEach(file => {
                    writeByByte(path.join(basePath, file), 512, oneBlockString);
                    writeByByte(path.join(testPath, file), 512, oneBlockString);
                });
                compareDirectorys();
            });
            it('long', () => {
                files.forEach(file => {
                    writeByByte(path.join(basePath, file), 0, largeString);
                    writeByByte(path.join(testPath, file), 0, largeString);
                });
                compareDirectorys();
            });
        });
        describe('Write offset', () => {
            it('short', () => {
                files.forEach(file => {
                    writeByByte(path.join(basePath, file), 2000, smallString);
                    writeByByte(path.join(testPath, file), 2000, smallString);
                });
                compareDirectorys();
            });
            it('block', () => {
                files.forEach(file => {
                    writeByByte(path.join(basePath, file), 1, oneBlockString);
                    writeByByte(path.join(testPath, file), 1, oneBlockString);
                });
                compareDirectorys();
            });
            it('long', () => {
                files.forEach(file => {
                    writeByByte(path.join(basePath, file), 511, largeString);
                    writeByByte(path.join(testPath, file), 511, largeString);
                });
                compareDirectorys();
            });
        });
    });
    describe('delete files:', () => {
        let files = fs.readdirSync(basePath);
        it('delete all files in directory', () => {
            files.forEach(file => {
                fs.unlinkSync(path.join(basePath, file));
                fs.unlinkSync(path.join(testPath, file));
            });
        });
        it("delete not xisting file",()=>{
            expect(function () {
                fs.unlinkSync(path.join(testPath, "none"));
            }).to.throw(
                "ENOENT: no such file or directory, unlink 'testFiles/none"
            );

        })
    });
    describe("create new files:",() =>{

    })
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