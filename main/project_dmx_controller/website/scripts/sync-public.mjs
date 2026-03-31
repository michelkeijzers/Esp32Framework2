import fs from "node:fs";
import path from "node:path";

const websiteRoot = process.cwd();
const publicDir = path.join(websiteRoot, "public");

if (!fs.existsSync(publicDir)) {
    fs.mkdirSync(publicDir, { recursive: true });
}

const keepExtensions = new Set([".html", ".css", ".js"]);

for (const entry of fs.readdirSync(websiteRoot, { withFileTypes: true })) {
    if (!entry.isFile()) {
        continue;
    }

    const ext = path.extname(entry.name).toLowerCase();
    if (!keepExtensions.has(ext)) {
        continue;
    }

    const from = path.join(websiteRoot, entry.name);
    const to = path.join(publicDir, entry.name);
    fs.copyFileSync(from, to);
}

const imagesSource = path.join(websiteRoot, "images");
const imagesDest = path.join(publicDir, "images");

if (fs.existsSync(imagesSource)) {
    fs.cpSync(imagesSource, imagesDest, { recursive: true, force: true });
}


// Always copy dmx_controller_custom.css as custom.css for /custom.css links
const customCssSrc = path.join(websiteRoot, "dmx_controller_custom.css");
const customCssAlias = path.join(publicDir, "custom.css");
if (fs.existsSync(customCssSrc)) {
    fs.copyFileSync(customCssSrc, customCssAlias);
}

console.log("Public website assets synchronized.");
