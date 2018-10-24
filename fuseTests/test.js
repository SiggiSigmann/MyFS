const expect = require("chai").expect;
const chai = require("chai");
const fs = require("fs");
const path = require("path");
const shell = require('shelljs');
let basePath ="../fs/"

describe("Myfs", async function () {
    it("test if File System is mounted", function (done) {
        
    });
    it("getFiles", function () {
        let files = fs.readdirSync(basePath);
        expect(files).to.be.not.empty

    });
});

