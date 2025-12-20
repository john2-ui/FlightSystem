-- ==========================================
-- 0. 补全刚才未写完的 CZ3988 机票数据
-- ==========================================
-- 接您上一段代码的断点...
INSERT INTO ticket (flight_id, class, price, total_seats, remain_seats) VALUES
(9, 'economy', 600.00, 250, 250), -- 航班取消，座位全空
(9, 'business', 1200.00, 36, 36),
(9, 'first', 2000.00, 12, 12);

-- ==========================================
-- 1. 扩展：插入国际城市 & 更多国内节点
-- ==========================================
INSERT INTO city (name, code, country) VALUES
('昆明', 'KMG', '中国'),    -- ID: 10
('厦门', 'XMN', '中国'),    -- ID: 11
('东京', 'TYO', '日本'),    -- ID: 12 (国际)
('首尔', 'SEL', '韩国'),    -- ID: 13 (国际)
('新加坡', 'SIN', '新加坡'), -- ID: 14 (国际)
('伦敦', 'LON', '英国'),    -- ID: 15 (跨洲)
('纽约', 'NYC', '美国');    -- ID: 16 (跨洲)

-- ==========================================
-- 2. 扩展：插入对应的新机场
-- ==========================================
INSERT INTO airport (name, code, city_id, terminal_count) VALUES
('昆明长水国际机场', 'KMG', 10, 1),    -- ID: 13
('厦门高崎国际机场', 'XMN', 11, 2),    -- ID: 14
('东京成田国际机场', 'NRT', 12, 3),    -- ID: 15
('东京羽田国际机场', 'HND', 12, 3),    -- ID: 16
('首尔仁川国际机场', 'ICN', 13, 2),    -- ID: 17
('新加坡樟宜机场', 'SIN', 14, 4),      -- ID: 18
('伦敦希思罗机场', 'LHR', 15, 5),      -- ID: 19
('纽约肯尼迪国际机场', 'JFK', 16, 6);  -- ID: 20

-- ==========================================
-- 3. 扩展：插入特殊机型
-- ==========================================
INSERT INTO airplane (model, seats_economy, seats_business, seats_first) VALUES
('A380', 400, 70, 14),   -- ID: 8 (巨无霸)
('ARJ21', 90, 0, 0);     -- ID: 9 (国产支线，全经济舱)

-- ==========================================
-- 4. 扩展：海量航班数据 (覆盖未来30天)
-- ==========================================
INSERT INTO flight (flight_no, airplane_id, depart_airport_id, arrive_airport_id, depart_time, arrive_time, status) VALUES

-- --- 场景A：热门国际航线 (价格高，时间长) ---

-- 上海PVG -> 东京NRT (明日出发)
('NH920', 7, 4, 15, 
 DATE_ADD(DATE(NOW()), INTERVAL '1 13:00' DAY_MINUTE), 
 DATE_ADD(DATE(NOW()), INTERVAL '1 16:50' DAY_MINUTE), 'normal'),

-- 东京NRT -> 上海PVG
('NH919', 7, 15, 4, 
 DATE_ADD(DATE(NOW()), INTERVAL '1 18:00' DAY_MINUTE), 
 DATE_ADD(DATE(NOW()), INTERVAL '1 20:30' DAY_MINUTE), 'normal'),

-- 北京PEK -> 伦敦LHR (长途11小时，A350)
('CA937', 5, 2, 19, 
 DATE_ADD(DATE(NOW()), INTERVAL '3 14:00' DAY_MINUTE), 
 DATE_ADD(DATE(NOW()), INTERVAL '3 18:00' DAY_MINUTE), 'normal'), -- 注意：到达时间是当地时间，这里简化处理，主要测时长逻辑

-- 伦敦LHR -> 北京PEK
('CA938', 5, 19, 2, 
 DATE_ADD(DATE(NOW()), INTERVAL '4 20:00' DAY_MINUTE), 
 DATE_ADD(DATE(NOW()), INTERVAL '5 13:00' DAY_MINUTE), 'normal'),

-- 广州CAN -> 新加坡SIN (A380执飞)
('CZ3039', 8, 1, 18, 
 DATE_ADD(DATE(NOW()), INTERVAL '5 12:30' DAY_MINUTE), 
 DATE_ADD(DATE(NOW()), INTERVAL '5 16:30' DAY_MINUTE), 'normal'),

-- 纽约JFK -> 上海PVG (东航跨太平洋)
('MU588', 4, 20, 4, 
 DATE_ADD(DATE(NOW()), INTERVAL '10 16:00' DAY_MINUTE), 
 DATE_ADD(DATE(NOW()), INTERVAL '11 19:00' DAY_MINUTE), 'normal'),


-- --- 场景B：国内支线与旅游线 (测试小飞机和不同城市) ---

-- 昆明 -> 厦门 (旅游热线)
('MF8405', 3, 13, 14, 
 DATE_ADD(DATE(NOW()), INTERVAL '2 09:00' DAY_MINUTE), 
 DATE_ADD(DATE(NOW()), INTERVAL '2 11:40' DAY_MINUTE), 'normal'),

-- 成都TFU -> 昆明 (ARJ21 支线)
('EU2201', 9, 8, 13, 
 DATE_ADD(DATE(NOW()), INTERVAL '2 14:00' DAY_MINUTE), 
 DATE_ADD(DATE(NOW()), INTERVAL '2 15:30' DAY_MINUTE), 'normal'),

-- 西安 -> 乌鲁木齐 (模拟长距离国内) - 假设之前数据里没有乌鲁木齐，用现有城市代替逻辑
('HU7123', 2, 10, 2, -- 西安飞北京
 DATE_ADD(DATE(NOW()), INTERVAL '4 07:00' DAY_MINUTE), 
 DATE_ADD(DATE(NOW()), INTERVAL '4 09:00' DAY_MINUTE), 'normal'),


-- --- 场景C：红眼航班 (跨夜测试) ---

-- 深圳 -> 北京PKX (深夜起飞，次日到达)
('ZH9111', 1, 6, 3, 
 DATE_ADD(DATE(NOW()), INTERVAL '5 23:30' DAY_MINUTE), 
 DATE_ADD(DATE(NOW()), INTERVAL '6 02:45' DAY_MINUTE), 'normal'),


-- --- 场景D：密集班次 (用于分页测试) ---
-- 假设是一条京沪快线，每隔2小时一班

('MU5101', 5, 5, 2, DATE_ADD(DATE(NOW()), INTERVAL '10 08:00' DAY_MINUTE), DATE_ADD(DATE(NOW()), INTERVAL '10 10:15' DAY_MINUTE), 'normal'),
('MU5103', 5, 5, 2, DATE_ADD(DATE(NOW()), INTERVAL '10 10:00' DAY_MINUTE), DATE_ADD(DATE(NOW()), INTERVAL '10 12:15' DAY_MINUTE), 'normal'),
('MU5105', 5, 5, 2, DATE_ADD(DATE(NOW()), INTERVAL '10 12:00' DAY_MINUTE), DATE_ADD(DATE(NOW()), INTERVAL '10 14:15' DAY_MINUTE), 'normal'),
('MU5107', 5, 5, 2, DATE_ADD(DATE(NOW()), INTERVAL '10 14:00' DAY_MINUTE), DATE_ADD(DATE(NOW()), INTERVAL '10 16:15' DAY_MINUTE), 'normal'),
('MU5109', 5, 5, 2, DATE_ADD(DATE(NOW()), INTERVAL '10 16:00' DAY_MINUTE), DATE_ADD(DATE(NOW()), INTERVAL '10 18:15' DAY_MINUTE), 'normal');

-- ==========================================
-- 5. 扩展：批量插入机票 (对应上面的 ID 13 - 28)
-- ==========================================

-- 13. NH920 (上海-东京)
INSERT INTO ticket (flight_id, class, price, total_seats, remain_seats) VALUES
(13, 'economy', 2500.00, 240, 100),
(13, 'business', 5500.00, 30, 10),
(13, 'first', 8800.00, 10, 2);

-- 14. NH919 (东京-上海)
INSERT INTO ticket (flight_id, class, price, total_seats, remain_seats) VALUES
(14, 'economy', 2400.00, 240, 240),
(14, 'business', 5400.00, 30, 30),
(14, 'first', 8500.00, 10, 10);

-- 15. CA937 (北京-伦敦) - 昂贵
INSERT INTO ticket (flight_id, class, price, total_seats, remain_seats) VALUES
(15, 'economy', 6800.00, 260, 50),
(15, 'business', 18000.00, 36, 5),
(15, 'first', 32000.00, 20, 1); -- 仅剩1张

-- 16. CA938 (伦敦-北京)
INSERT INTO ticket (flight_id, class, price, total_seats, remain_seats) VALUES
(16, 'economy', 7200.00, 260, 120),
(16, 'business', 18500.00, 36, 20),
(16, 'first', 33000.00, 20, 10);

-- 17. CZ3039 (广州-新加坡 A380)
INSERT INTO ticket (flight_id, class, price, total_seats, remain_seats) VALUES
(17, 'economy', 1500.00, 400, 380),
(17, 'business', 3500.00, 70, 60),
(17, 'first', 6000.00, 14, 14);

-- 18. MU588 (纽约-上海) - 极度紧俏
INSERT INTO ticket (flight_id, class, price, total_seats, remain_seats) VALUES
(18, 'economy', 12000.00, 300, 0),   -- 售罄
(18, 'business', 35000.00, 42, 2),    -- 剩2张
(18, 'first', 68000.00, 14, 0);       -- 售罄

-- 19. MF8405 (昆明-厦门)
INSERT INTO ticket (flight_id, class, price, total_seats, remain_seats) VALUES
(19, 'economy', 780.00, 160, 100),
(19, 'business', 1500.00, 18, 10);

-- 20. EU2201 (成都-昆明 ARJ21) - 只有经济舱
INSERT INTO ticket (flight_id, class, price, total_seats, remain_seats) VALUES
(20, 'economy', 450.00, 90, 85);

-- 21. HU7123 (西安-北京)
INSERT INTO ticket (flight_id, class, price, total_seats, remain_seats) VALUES
(21, 'economy', 560.00, 250, 200),
(21, 'business', 1100.00, 36, 30);

-- 22. ZH9111 (深圳-北京大兴 红眼)
INSERT INTO ticket (flight_id, class, price, total_seats, remain_seats) VALUES
(22, 'economy', 400.00, 150, 140), -- 便宜
(22, 'business', 900.00, 20, 20);

-- 23-27. 京沪快线批量 (MU5101 - MU5109)
-- 简单起见，批量插入
INSERT INTO ticket (flight_id, class, price, total_seats, remain_seats) VALUES
(23, 'economy', 1300.00, 260, 200), (23, 'business', 2800.00, 36, 30),
(24, 'economy', 1300.00, 260, 150), (24, 'business', 2800.00, 36, 15),
(25, 'economy', 1450.00, 260, 100), (25, 'business', 3000.00, 36, 10), -- 中午贵一点
(26, 'economy', 1300.00, 260, 220), (26, 'business', 2800.00, 36, 36),
(27, 'economy', 1600.00, 260, 50),  (27, 'business', 3200.00, 36, 2);  -- 晚高峰贵且票少

-- ==========================================
-- 6. (可选) 超级数据生成器 - 如果你需要 1000+ 条数据
-- ==========================================
-- 这是一个简单的存储过程，用于生成 1000 个随机航班
-- 如果需要运行，请取消注释
DELIMITER $$
CREATE PROCEDURE GenerateMassiveData()
BEGIN
    DECLARE i INT DEFAULT 1;
    WHILE i <= 1000 DO
        INSERT INTO flight (flight_no, airplane_id, depart_airport_id, arrive_airport_id, depart_time, arrive_time, status)
        VALUES (
            CONCAT('TEST', i), 
            FLOOR(1 + RAND() * 9), -- 随机飞机
            FLOOR(1 + RAND() * 20), -- 随机起飞机场
            FLOOR(1 + RAND() * 20), -- 随机到达机场
            DATE_ADD(NOW(), INTERVAL FLOOR(RAND() * 30) DAY), -- 未来30天内随机
            DATE_ADD(NOW(), INTERVAL FLOOR(RAND() * 30) + 1 DAY),
            'normal'
        );
        SET i = i + 1;
    END WHILE;
END$$
DELIMITER ;
-- CALL GenerateMassiveData();
