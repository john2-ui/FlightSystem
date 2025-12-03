-- ==========================================
-- 插入测试数据
-- 用于 Backend 功能测试
-- ==========================================

USE flightsystem;

-- 清空现有数据（按依赖顺序）
DELETE FROM ticket;
DELETE FROM flight;
DELETE FROM airplane;
DELETE FROM airport;
DELETE FROM city;

-- 重置自增ID
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
('成都', 'CD', '中国');

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
('成都双流国际机场', 'CTU', 5, 2);

-- ==========================================
-- 3. 插入飞机数据
-- ==========================================
INSERT INTO airplane (model, seats_economy, seats_business, seats_first) VALUES
('A320', 150, 20, 10),
('A330', 250, 36, 12),
('B737', 160, 18, 8),
('B777', 300, 42, 14),
('A350', 260, 36, 20);

-- ==========================================
-- 4. 插入航班数据
-- ==========================================
INSERT INTO flight (flight_no, airplane_id, depart_airport_id, arrive_airport_id, depart_time, arrive_time, status) VALUES
-- 广州 <-> 北京
('CZ3101', 1, 1, 2, '2025-11-21 08:00:00', '2025-11-21 11:00:00', 'normal'),
('CZ3102', 1, 2, 1, '2025-11-21 14:00:00', '2025-11-21 17:00:00', 'normal'),

-- 广州 <-> 上海
('CZ3201', 3, 1, 4, '2025-11-21 09:30:00', '2025-11-21 12:00:00', 'normal'),
('CZ3202', 3, 4, 1, '2025-11-21 15:00:00', '2025-11-21 17:30:00', 'normal'),

-- 北京 <-> 上海
('CA1501', 2, 2, 4, '2025-11-21 10:00:00', '2025-11-21 12:30:00', 'normal'),
('CA1502', 2, 4, 2, '2025-11-21 16:00:00', '2025-11-21 18:30:00', 'delayed'),

-- 广州 <-> 深圳
('CZ3301', 1, 1, 6, '2025-11-21 07:00:00', '2025-11-21 07:50:00', 'normal'),

-- 广州 <-> 成都
('CZ3401', 4, 1, 7, '2025-11-21 11:00:00', '2025-11-21 13:30:00', 'normal');

-- ==========================================
-- 5. 插入机票数据
-- ==========================================

-- 航班1: CZ3101 (广州->北京, A320)
INSERT INTO ticket (flight_id, class, price, total_seats, remain_seats) VALUES
(1, 'economy', 899.00, 150, 120),
(1, 'business', 1999.00, 20, 15),
(1, 'first', 3999.00, 10, 8);

-- 航班2: CZ3102 (北京->广州, A320)
INSERT INTO ticket (flight_id, class, price, total_seats, remain_seats) VALUES
(2, 'economy', 950.00, 150, 130),
(2, 'business', 2100.00, 20, 18),
(2, 'first', 4200.00, 10, 10);

-- 航班3: CZ3201 (广州->上海, B737)
INSERT INTO ticket (flight_id, class, price, total_seats, remain_seats) VALUES
(3, 'economy', 750.00, 160, 100),
(3, 'business', 1800.00, 18, 12),
(3, 'first', 3500.00, 8, 6);

-- 航班4: CZ3202 (上海->广州, B737)
INSERT INTO ticket (flight_id, class, price, total_seats, remain_seats) VALUES
(4, 'economy', 780.00, 160, 110),
(4, 'business', 1850.00, 18, 15),
(4, 'first', 3600.00, 8, 7);

-- 航班5: CA1501 (北京->上海, A330)
INSERT INTO ticket (flight_id, class, price, total_seats, remain_seats) VALUES
(5, 'economy', 680.00, 250, 200),
(5, 'business', 1500.00, 36, 30),
(5, 'first', 3000.00, 12, 10);

-- 航班6: CA1502 (上海->北京, A330, 延误)
INSERT INTO ticket (flight_id, class, price, total_seats, remain_seats) VALUES
(6, 'economy', 720.00, 250, 180),
(6, 'business', 1600.00, 36, 28),
(6, 'first', 3200.00, 12, 9);

-- 航班7: CZ3301 (广州->深圳, A320)
INSERT INTO ticket (flight_id, class, price, total_seats, remain_seats) VALUES
(7, 'economy', 350.00, 150, 140),
(7, 'business', 800.00, 20, 19),
(7, 'first', 1500.00, 10, 10);

-- 航班8: CZ3401 (广州->成都, B777)
INSERT INTO ticket (flight_id, class, price, total_seats, remain_seats) VALUES
(8, 'economy', 850.00, 300, 250),
(8, 'business', 2000.00, 42, 35),
(8, 'first', 4500.00, 14, 12);

-- ==========================================
-- 验证数据
-- ==========================================

SELECT '城市数据' AS Table_Name, COUNT(*) AS Count FROM city
UNION ALL
SELECT '机场数据', COUNT(*) FROM airport
UNION ALL
SELECT '飞机数据', COUNT(*) FROM airplane
UNION ALL
SELECT '航班数据', COUNT(*) FROM flight
UNION ALL
SELECT '机票数据', COUNT(*) FROM ticket;

SELECT 'Data initialization completed!' AS Status;
