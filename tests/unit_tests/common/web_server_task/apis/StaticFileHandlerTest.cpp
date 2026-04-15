#include <gtest/gtest.h>

#include "StaticFileHandler.hpp"

TEST(StaticFileHandlerTest, NullPathReturnsOctetStream) {
    EXPECT_STREQ("application/octet-stream", StaticFileHandler::get_mime_type(nullptr));
}

TEST(StaticFileHandlerTest, HtmlExtensionsReturnHtmlMimeType) {
    EXPECT_STREQ("text/html", StaticFileHandler::get_mime_type("/index.html"));
    EXPECT_STREQ("text/html", StaticFileHandler::get_mime_type("/index.htm"));
}

TEST(StaticFileHandlerTest, CssExtensionReturnsCssMimeType) {
    EXPECT_STREQ("text/css", StaticFileHandler::get_mime_type("/assets/site.css"));
}

TEST(StaticFileHandlerTest, JavaScriptExtensionReturnsJavaScriptMimeType) {
    EXPECT_STREQ("application/javascript", StaticFileHandler::get_mime_type("/assets/app.js"));
}

TEST(StaticFileHandlerTest, JsonExtensionReturnsJsonMimeType) {
    EXPECT_STREQ("application/json", StaticFileHandler::get_mime_type("/api/data.json"));
}

TEST(StaticFileHandlerTest, ImageExtensionsReturnExpectedMimeTypes) {
    EXPECT_STREQ("image/png", StaticFileHandler::get_mime_type("/img/logo.png"));
    EXPECT_STREQ("image/jpeg", StaticFileHandler::get_mime_type("/img/photo.jpg"));
    EXPECT_STREQ("image/jpeg", StaticFileHandler::get_mime_type("/img/photo.jpeg"));
    EXPECT_STREQ("image/gif", StaticFileHandler::get_mime_type("/img/anim.gif"));
    EXPECT_STREQ("image/svg+xml", StaticFileHandler::get_mime_type("/img/icon.svg"));
    EXPECT_STREQ("image/x-icon", StaticFileHandler::get_mime_type("/favicon.ico"));
}

TEST(StaticFileHandlerTest, TextAndFontExtensionsReturnExpectedMimeTypes) {
    EXPECT_STREQ("text/plain", StaticFileHandler::get_mime_type("/notes/readme.txt"));
    EXPECT_STREQ("font/woff", StaticFileHandler::get_mime_type("/fonts/main.woff"));
    EXPECT_STREQ("font/woff2", StaticFileHandler::get_mime_type("/fonts/main.woff2"));
}

TEST(StaticFileHandlerTest, UnknownExtensionReturnsOctetStream) {
    EXPECT_STREQ("application/octet-stream", StaticFileHandler::get_mime_type("/blob.bin"));
}