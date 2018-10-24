const expect = require("chai").expect;
const chai = require("chai");
const fs = require("fs");
const path = require("path");
const shell = require('shelljs');
const chalk = require('chalk');

let basePath ="../fs/"

describe("Myfs", async function () {
    it("test if File System is mounted", function (done) {

        if (fs.existsSync(basePath)) {
            expect(true).to.be.true;
        }else{
            console.log(chalk.blue('\t\tmounting container.bin...'));
            shell.exec("cd .. && bash startmount.sh", {silent:true});
            expect(fs.existsSync(basePath)).to.be.true;
            done()
        }
    }).timeout(50000)
    it("getFiles", function () {
        let files = fs.readdirSync(basePath);
        expect(files).to.be.not.empty

    });
});

