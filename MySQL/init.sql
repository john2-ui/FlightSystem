-- ==========================================
-- 初始化 FlightSystem 数据库
-- ==========================================

DROP DATABASE IF EXISTS FlightSystem;
CREATE DATABASE FlightSystem CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
USE FlightSystem;

SOURCE city.sql;
SOURCE airport.sql;
SOURCE airplane.sql;
SOURCE flight.sql;
SOURCE ticket.sql;

-- 完成
