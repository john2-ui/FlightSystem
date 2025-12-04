-- ==========================================
-- 初始化 flightsystem 数据库
-- ==========================================
-- 设置连接字符集为 utf8mb4
SET NAMES utf8mb4;

DROP DATABASE IF EXISTS flightsystem;
CREATE DATABASE flightsystem CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
USE flightsystem;

SOURCE city.sql;
SOURCE airport.sql;
SOURCE airplane.sql;
SOURCE flight.sql;
SOURCE ticket.sql;
SOURCE user.sql;

-- 完成
