/*
 Navicat Premium Data Transfer

 Source Server         : local_history
 Source Server Type    : SQLite
 Source Server Version : 3035005 (3.35.5)
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005 (3.35.5)
 File Encoding         : 65001

 Date: 29/07/2025 22:02:03
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Table structure for file_info
-- ----------------------------
DROP TABLE IF EXISTS "file_info";
CREATE TABLE "file_info" (
  "file_id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
  "user_id" INTEGER NOT NULL,
  "file_hash" TEXT(64) NOT NULL,
  "storage_path" TEXT(1024) NOT NULL,
  "upload_time" TEXT(6) NOT NULL DEFAULT CURRENT_TIMESTAMP,
  "logical_name" TEXT(510) NOT NULL,
  "process_type" TEXT(10) NOT NULL,
  FOREIGN KEY ("user_id") REFERENCES "user" ("uid") ON DELETE CASCADE ON UPDATE NO ACTION
);

-- ----------------------------
-- Auto increment value for file_info
-- ----------------------------

-- ----------------------------
-- Indexes structure for table file_info
-- ----------------------------
CREATE INDEX "idx_hash"
ON "file_info" (
  "file_hash" ASC
);
CREATE UNIQUE INDEX "idx_unique_file"
ON "file_info" (
  "user_id" ASC,
  "file_hash" ASC
);

PRAGMA foreign_keys = true;
