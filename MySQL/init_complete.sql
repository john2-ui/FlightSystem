-- ==========================================
-- 完整初始化 flightsystem 数据库
-- 适用于 Navicat 直接执行
-- ==========================================

-- 设置字符集
SET NAMES utf8mb4;

-- 删除旧数据库（如果存在）
DROP DATABASE IF EXISTS flightsystem;

-- 创建新数据库
CREATE DATABASE flightsystem CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;

-- 切换到新数据库
USE flightsystem;

-- ==========================================
-- 1. 创建 city 表
-- ==========================================
CREATE TABLE IF NOT EXISTS city (
    id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(50) NOT NULL COMMENT '城市名称',
    code VARCHAR(10) COMMENT '城市代码',
    country VARCHAR(50) COMMENT '国家'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- ==========================================
-- 2. 创建 airport 表
-- ==========================================
CREATE TABLE IF NOT EXISTS airport (
    id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(100) NOT NULL COMMENT '机场名称',
    code VARCHAR(10) NOT NULL COMMENT '三字码，如 CAN',
    city_id INT NOT NULL COMMENT '所属城市',
    terminal_count INT DEFAULT 1 COMMENT '航站楼数量',
    FOREIGN KEY (city_id) REFERENCES city(id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- ==========================================
-- 3. 创建 airplane 表
-- ==========================================
CREATE TABLE IF NOT EXISTS airplane (
    id INT PRIMARY KEY AUTO_INCREMENT,
    model VARCHAR(50) NOT NULL COMMENT '机型，如 A320',
    seats_economy INT DEFAULT 0,
    seats_business INT DEFAULT 0,
    seats_first INT DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- ==========================================
-- 4. 创建 flight 表
-- ==========================================
CREATE TABLE IF NOT EXISTS flight (
    id INT PRIMARY KEY AUTO_INCREMENT,
    flight_no VARCHAR(10) NOT NULL COMMENT '航班号，如 CZ3101',
    airplane_id INT NOT NULL COMMENT '飞机ID',
    depart_airport_id INT NOT NULL COMMENT '出发机场',
    arrive_airport_id INT NOT NULL COMMENT '到达机场',
    depart_time DATETIME NOT NULL COMMENT '起飞时间',
    arrive_time DATETIME NOT NULL COMMENT '到达时间',
    status VARCHAR(20) DEFAULT 'normal' COMMENT 'normal/delayed/cancelled',
    FOREIGN KEY (airplane_id) REFERENCES airplane(id),
    FOREIGN KEY (depart_airport_id) REFERENCES airport(id),
    FOREIGN KEY (arrive_airport_id) REFERENCES airport(id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- ==========================================
-- 5. 创建 ticket 表
-- ==========================================
CREATE TABLE IF NOT EXISTS ticket (
    id INT PRIMARY KEY AUTO_INCREMENT,
    flight_id INT NOT NULL COMMENT '航班ID',
    class ENUM('economy','business','first') NOT NULL COMMENT '舱位类型',
    price DECIMAL(10,2) NOT NULL COMMENT '价格',
    total_seats INT NOT NULL,
    remain_seats INT NOT NULL,
    FOREIGN KEY (flight_id) REFERENCES flight(id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- ==========================================
-- 验证创建结果
-- ==========================================
SHOW TABLES;

SELECT 'Database flightsystem created successfully!' AS status;
