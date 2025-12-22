-- ==========================================
-- 插入测试数据 (Enhanced Version)
-- 适用：Backend 功能测试
-- 特性：时间自动推算为未来时间，数据量更大
-- ==========================================

USE flightsystem;
-- 数据库字符集
ALTER DATABASE flightsystem CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci;

-- 表字符集
ALTER TABLE city CONVERT TO CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
ALTER TABLE airport CONVERT TO CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
ALTER TABLE airplane CONVERT TO CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
ALTER TABLE flight CONVERT TO CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
ALTER TABLE ticket CONVERT TO CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;


-- 1. 清空现有数据（按外键依赖顺序删除）
DELETE FROM ticket;
DELETE FROM flight;
DELETE FROM airplane;
DELETE FROM airport;
DELETE FROM city;

-- 2. 重置自增ID (确保ID从1开始，方便调试)
ALTER TABLE ticket AUTO_INCREMENT = 1;
ALTER TABLE flight AUTO_INCREMENT = 1;
ALTER TABLE airplane AUTO_INCREMENT = 1;
ALTER TABLE airport AUTO_INCREMENT = 1;
ALTER TABLE city AUTO_INCREMENT = 1;

-- ==========================================
-- 1. 插入城市数据
-- ==========================================
INSERT INTO city (name, code, country) VALUES
('广州', 'GZ', '中国'),    
('北京', 'BJ', '中国'),    
('上海', 'SH', '中国'),    
('深圳', 'SZ', '中国'),    
('成都', 'CD', '中国'),    
('杭州', 'HGH', '中国'),   
('西安', 'XIY', '中国'),   
('武汉', 'WUH', '中国'),   
('长沙', 'CSX', '中国');   

-- ==========================================
-- 2. 插入机场数据
-- ==========================================
INSERT INTO airport (name, code, city_id, terminal_count) VALUES
('广州白云国际机场', 'CAN', 1, 2), 
('北京首都国际机场', 'PEK', 2, 3), 
('北京大兴国际机场', 'PKX', 2, 1), 
('上海浦东国际机场', 'PVG', 3, 2), 
('上海虹桥国际机场', 'SHA', 3, 2), 
('深圳宝安国际机场', 'SZX', 4, 1), 
('成都双流国际机场', 'CTU', 5, 2), 
('成都天府国际机场', 'TFU', 5, 2), 
('杭州萧山国际机场', 'HGH', 6, 3), 
('西安咸阳国际机场', 'XIY', 7, 3), 
('武汉天河国际机场', 'WUH', 8, 2), 
('长沙黄花国际机场', 'CSX', 9, 2);

-- ==========================================
-- 3. 插入飞机数据
-- ==========================================
INSERT INTO airplane (model, seats_economy, seats_business, seats_first) VALUES
('A320', 150, 20, 10),
('A330', 250, 36, 12),
('B737', 160, 18, 8),
('B777', 300, 42, 14),
('A350', 260, 36, 20),
('C919', 158, 8, 0),
('B787', 240, 30, 10),
('A380', 400, 70, 14),
('ARJ21', 90, 0, 0);

-- ==========================================
-- 4. 插入固定航班数据
-- ==========================================
-- 注意：使用 TIMESTAMP(DATE_ADD(...), 'HH:MM:SS') 修正时间

INSERT INTO flight (flight_no, airplane_id, depart_airport_id, arrive_airport_id, depart_time, arrive_time, status) VALUES
('CZ3101', 1, 1, 2, TIMESTAMP(DATE_ADD(CURDATE(), INTERVAL 1 DAY), '08:00:00'), TIMESTAMP(DATE_ADD(CURDATE(), INTERVAL 1 DAY), '11:00:00'), 'normal'),
('CZ3102', 1, 2, 1, TIMESTAMP(DATE_ADD(CURDATE(), INTERVAL 1 DAY), '14:00:00'), TIMESTAMP(DATE_ADD(CURDATE(), INTERVAL 1 DAY), '17:00:00'), 'normal'),
('MU9191', 6, 5, 6, TIMESTAMP(DATE_ADD(CURDATE(), INTERVAL 1 DAY), '09:00:00'), TIMESTAMP(DATE_ADD(CURDATE(), INTERVAL 1 DAY), '11:30:00'), 'normal'),
('MU9192', 6, 6, 5, TIMESTAMP(DATE_ADD(CURDATE(), INTERVAL 1 DAY), '13:00:00'), TIMESTAMP(DATE_ADD(CURDATE(), INTERVAL 1 DAY), '15:30:00'), 'delayed'),
('CA1701', 5, 9, 2, TIMESTAMP(DATE_ADD(CURDATE(), INTERVAL 2 DAY), '07:30:00'), TIMESTAMP(DATE_ADD(CURDATE(), INTERVAL 2 DAY), '09:40:00'), 'normal'),
('CA1702', 5, 2, 9, TIMESTAMP(DATE_ADD(CURDATE(), INTERVAL 2 DAY), '11:00:00'), TIMESTAMP(DATE_ADD(CURDATE(), INTERVAL 2 DAY), '13:10:00'), 'normal'),
('CZ3403', 7, 1, 8, TIMESTAMP(DATE_ADD(CURDATE(), INTERVAL 2 DAY), '10:00:00'), TIMESTAMP(DATE_ADD(CURDATE(), INTERVAL 2 DAY), '12:45:00'), 'normal'),
('HU7801', 3, 10, 11, TIMESTAMP(DATE_ADD(CURDATE(), INTERVAL 3 DAY), '08:00:00'), TIMESTAMP(DATE_ADD(CURDATE(), INTERVAL 3 DAY), '09:30:00'), 'normal'),
('CZ3988', 2, 12, 4, TIMESTAMP(DATE_ADD(CURDATE(), INTERVAL 3 DAY), '15:00:00'), TIMESTAMP(DATE_ADD(CURDATE(), INTERVAL 3 DAY), '17:00:00'), 'cancelled'),
('CZ3555', 4, 1, 5, TIMESTAMP(DATE_ADD(CURDATE(), INTERVAL 7 DAY), '09:00:00'), TIMESTAMP(DATE_ADD(CURDATE(), INTERVAL 7 DAY), '11:15:00'), 'normal'),
('CZ3556', 4, 5, 1, TIMESTAMP(DATE_ADD(CURDATE(), INTERVAL 7 DAY), '13:00:00'), TIMESTAMP(DATE_ADD(CURDATE(), INTERVAL 7 DAY), '15:15:00'), 'normal'),
('KN5858', 3, 3, 6, TIMESTAMP(DATE_ADD(CURDATE(), INTERVAL 7 DAY), '22:00:00'), TIMESTAMP(DATE_ADD(CURDATE(), INTERVAL 8 DAY), '01:10:00'), 'normal');

-- ==========================================
-- 5. 插入机票数据
-- ==========================================
INSERT INTO ticket (flight_id, class, price, total_seats, remain_seats) VALUES
(1, 'economy', 899.00, 150, 2), (1, 'business', 1999.00, 20, 15), (1, 'first', 3999.00, 10, 8),
(2, 'economy', 950.00, 150, 130), (2, 'business', 2100.00, 20, 18), (2, 'first', 4200.00, 10, 10),
(3, 'economy', 650.00, 158, 100), (3, 'business', 1200.00, 8, 2),
(4, 'economy', 650.00, 158, 158), (4, 'business', 1200.00, 8, 8),
(5, 'economy', 1100.00, 260, 200), (5, 'business', 2500.00, 36, 30), (5, 'first', 5800.00, 20, 18),
(6, 'economy', 1100.00, 260, 210), (6, 'business', 2500.00, 36, 32), (6, 'first', 5800.00, 20, 20),
(7, 'economy', 920.00, 240, 180), (7, 'business', 1800.00, 30, 15), (7, 'first', 3200.00, 10, 5),
(8, 'economy', 450.00, 160, 80), (8, 'business', 900.00, 18, 10), (8, 'first', 1500.00, 8, 4),
(9, 'economy', 600.00, 250, 250), (9, 'business', 1200.00, 36, 36), (9, 'first', 2000.00, 12, 12);

-- ==========================================
-- 6. 扩展国际城市及机场
-- ==========================================
INSERT INTO city (name, code, country) VALUES
('昆明', 'KMG', '中国'), ('厦门', 'XMN', '中国'),
('东京', 'TYO', '日本'), ('首尔', 'SEL', '韩国'), ('新加坡', 'SIN', '新加坡'),
('伦敦', 'LON', '英国'), ('纽约', 'NYC', '美国');

INSERT INTO airport (name, code, city_id, terminal_count) VALUES
('昆明长水国际机场', 'KMG', 10, 1), ('厦门高崎国际机场', 'XMN', 11, 2),
('东京成田国际机场', 'NRT', 12, 3), ('东京羽田国际机场', 'HND', 12, 3),
('首尔仁川国际机场', 'ICN', 13, 2), ('新加坡樟宜机场', 'SIN', 14, 4),
('伦敦希思罗机场', 'LHR', 15, 5), ('纽约肯尼迪国际机场', 'JFK', 16, 6);

-- ==========================================
-- 7. 批量随机航班生成存储过程
-- ==========================================
DROP PROCEDURE IF EXISTS GenerateMassiveData;
DELIMITER $$
CREATE PROCEDURE GenerateMassiveData()
BEGIN
    DECLARE i INT DEFAULT 1;
    DECLARE v_depart_id INT;
    DECLARE v_arrive_id INT;
    DECLARE v_airplane_id INT;
    DECLARE v_depart_time DATETIME;
    DECLARE v_flight_id INT;

    WHILE i <= 500 DO
        SET v_depart_id = FLOOR(RAND()*20)+1;

        REPEAT
            SET v_arrive_id = FLOOR(RAND()*20)+1;
        UNTIL v_arrive_id != v_depart_id END REPEAT;

        SET v_airplane_id = FLOOR(RAND()*9)+1;
        SET v_depart_time = DATE_ADD(NOW(), INTERVAL FLOOR(RAND()*30*24*60) MINUTE);

        INSERT INTO flight (flight_no, airplane_id, depart_airport_id, arrive_airport_id, depart_time, arrive_time, status)
        VALUES (
            CONCAT('RND', LPAD(i,4,'0')),
            v_airplane_id,
            v_depart_id,
            v_arrive_id,
            v_depart_time,
            DATE_ADD(v_depart_time, INTERVAL FLOOR(120 + RAND()*180) MINUTE),
            'normal'
        );

        SET v_flight_id = LAST_INSERT_ID();

        -- 经济舱
        INSERT INTO ticket (flight_id, class, price, total_seats, remain_seats)
        VALUES (v_flight_id, 'economy', FLOOR(500 + RAND()*1000), 150, LEAST(FLOOR(RAND()*150), 150));

        -- 商务舱
        IF RAND() > 0.3 THEN
            INSERT INTO ticket (flight_id, class, price, total_seats, remain_seats)
            VALUES (v_flight_id, 'business', FLOOR(1500 + RAND()*2000), 20, LEAST(FLOOR(RAND()*20), 20));
        END IF;

        SET i = i + 1;
    END WHILE;
END$$
DELIMITER ;

-- ==========================================
-- 8. 执行随机航班生成
-- ==========================================
CALL GenerateMassiveData();
