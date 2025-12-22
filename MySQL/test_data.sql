-- ==========================================
-- 最终优化版：基于机型容量的自动化数据生成
-- 特性：容量对齐、15架飞机多样性、高密度航班
-- ==========================================

USE flightsystem;

-- 1. 清空现有数据
SET FOREIGN_KEY_CHECKS = 0;
TRUNCATE TABLE ticket;
TRUNCATE TABLE flight;
TRUNCATE TABLE airplane;
TRUNCATE TABLE airport;
TRUNCATE TABLE city;
SET FOREIGN_KEY_CHECKS = 1;

-- 2. 重置自增ID
ALTER TABLE ticket AUTO_INCREMENT = 1;
ALTER TABLE flight AUTO_INCREMENT = 1;
ALTER TABLE airplane AUTO_INCREMENT = 1;
ALTER TABLE airport AUTO_INCREMENT = 1;
ALTER TABLE city AUTO_INCREMENT = 1;

-- ==========================================
-- 1. 插入核心城市
-- ==========================================
INSERT INTO city (name, code, country) VALUES
('北京', 'BJ', '中国'), ('上海', 'SH', '中国'), 
('广州', 'GZ', '中国'), ('深圳', 'SZ', '中国'), 
('成都', 'CD', '中国'), ('杭州', 'HGH', '中国'), 
('昆明', 'KMG', '中国'), ('西安', 'XIY', '中国');

-- ==========================================
-- 2. 插入核心机场
-- ==========================================
INSERT INTO airport (name, code, city_id, terminal_count) VALUES
('北京首都国际机场', 'PEK', 1, 3), ('上海浦东国际机场', 'PVG', 2, 2), 
('广州白云国际机场', 'CAN', 3, 2), ('深圳宝安国际机场', 'SZX', 4, 1), 
('成都天府国际机场', 'TFU', 5, 2), ('杭州萧山国际机场', 'HGH', 6, 3), 
('昆明长水国际机场', 'KMG', 7, 1), ('西安咸阳国际机场', 'XIY', 8, 3);

-- ==========================================
-- 3. 扩展飞机数据 (增加至15架，涵盖大中小型号)
-- ==========================================
INSERT INTO airplane (model, seats_economy, seats_business, seats_first) VALUES
('A320neo', 150, 20, 8),   ('A330-300', 260, 30, 12), ('B737-800', 162, 12, 0),
('B777-300ER', 310, 42, 16), ('A350-900', 280, 36, 12), ('C919', 158, 8, 0),
('B787-9', 250, 35, 10),    ('A380-800', 420, 80, 20), ('ARJ21-700', 90, 0, 0),
('B747-8', 340, 50, 20),    ('CRJ900', 78, 6, 0),      ('A321neo', 180, 24, 12),
('B737-MAX8', 170, 16, 0),  ('A330-900', 280, 30, 10), ('E190', 98, 8, 0);

-- ==========================================
-- 4. 自动化航班与动态机票生成存储过程
-- ==========================================
DROP PROCEDURE IF EXISTS GenerateSmartData;
DELIMITER $$
CREATE PROCEDURE GenerateSmartData()
BEGIN
    DECLARE i INT DEFAULT 1;
    DECLARE v_depart_id, v_arrive_id, v_airplane_id, v_flight_id INT;
    DECLARE v_base_date DATETIME DEFAULT '2025-12-23 00:00:00';
    DECLARE v_depart_time DATETIME;
    
    -- 用于读取飞机容量的变量
    DECLARE v_cap_economy, v_cap_business, v_cap_first INT;

    WHILE i <= 500 DO
        -- 1. 随机基础信息
        SET v_depart_id = FLOOR(1 + RAND() * 8);
        REPEAT
            SET v_arrive_id = FLOOR(1 + RAND() * 8);
        UNTIL v_arrive_id != v_depart_id END REPEAT;

        SET v_airplane_id = FLOOR(1 + RAND() * 15);
        SET v_depart_time = DATE_ADD(v_base_date, INTERVAL FLOOR(RAND() * 21600) MINUTE);

        -- 2. 插入航班
        INSERT INTO flight (flight_no, airplane_id, depart_airport_id, arrive_airport_id, depart_time, arrive_time, status)
        VALUES (
            CONCAT(ELT(FLOOR(1 + RAND() * 5), 'CA', 'MU', 'CZ', 'HU', 'MF'), LPAD(i, 4, '0')),
            v_airplane_id, v_depart_id, v_arrive_id, v_depart_time,
            DATE_ADD(v_depart_time, INTERVAL FLOOR(100 + RAND() * 150) MINUTE),
            'normal'
        );
        SET v_flight_id = LAST_INSERT_ID();

        -- 3. 获取该航班所用飞机的座位配置
        SELECT seats_economy, seats_business, seats_first 
        INTO v_cap_economy, v_cap_business, v_cap_first
        FROM airplane WHERE id = v_airplane_id;

        -- 4. 根据飞机配置动态生成机票
        -- 经济舱 (始终生成)
        IF v_cap_economy > 0 THEN
            INSERT INTO ticket (flight_id, class, price, total_seats, remain_seats)
            VALUES (v_flight_id, 'economy', FLOOR(400 + RAND()*800), v_cap_economy, FLOOR(RAND() * v_cap_economy));
        END IF;

        -- 商务舱 (仅当飞机有配置时生成)
        IF v_cap_business > 0 THEN
            INSERT INTO ticket (flight_id, class, price, total_seats, remain_seats)
            VALUES (v_flight_id, 'business', FLOOR(1500 + RAND()*1500), v_cap_business, FLOOR(RAND() * v_cap_business));
        END IF;

        -- 头等舱 (仅当飞机有配置时生成)
        IF v_cap_first > 0 THEN
            INSERT INTO ticket (flight_id, class, price, total_seats, remain_seats)
            VALUES (v_flight_id, 'first', FLOOR(3500 + RAND()*3000), v_cap_first, FLOOR(RAND() * v_cap_first));
        END IF;

        SET i = i + 1;
    END WHILE;
END$$
DELIMITER ;

-- 5. 执行生成
CALL GenerateSmartData();