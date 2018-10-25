const expect = require("chai").expect;
const chai = require("chai");
const fs = require("fs");
const readExactly = require("fs-read-exactly");
const path = require("path");
const shell = require("shelljs");
const chalk = require("chalk");

let basePath = "../fs/";
let testPath = "./testFiles";

describe("Myfs", async function () {
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
    it("check if containe.bin has files", function () {
        let files = fs.readdirSync(basePath);
        expect(files).to.be.not.empty;
        //fs.writeFile('file', new Buffer(1024*1024*54));
    });
    describe("test reading files", function () {
        it("Compare container bin to testFiles", function () {
            fs.readdirSync(basePath).forEach(file => {
                let stats = fs.statSync(path.join(basePath, file));
                expect(stats.blksize).to.be.equal(512);
                //compared container.bin files
                expect(fs.readFileSync(path.join(basePath, file), "utf8")).to.be.equal(
                    fs.readFileSync(path.join(testPath, file), "utf8")
                );
            });
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
            it("without offset: Exact", async () => {
                await compareTestToContainer(txtFile,txtTest,0,512);
            });
            it("without offset: Half", async () => {
                await compareTestToContainer(txtFile,txtTest,0,256);
            });
            it("without offset: Alot", async () => {
                await compareTestToContainer(txtFile,txtTest,0,20*512);
            });
            it("without offset: nothing", async () => {
                await compareTestToContainer(txtFile,txtTest,0,0);
            });
        });
    });
});

async function compareTestToContainer(file, testFile, pos, length) {
    let chunk1 = await exactly(file, pos, length);
    let chunk2 = await exactly(testFile, pos, length);
}

function exactly(file, pos, length) {
    return new Promise((resolve, reject) => {
        readExactly(file, pos, length, function (err, chunk) {
            if (err) reject(err);
            resolve(chunk);
        });
    });
}