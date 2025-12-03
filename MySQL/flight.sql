-- ==========================================
-- 航班表 flight
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

-- 示例数据（可删）
-- INSERT INTO flight (flight_no, airplane_id, depart_airport_id, arrive_airport_id, depart_time, arrive_time, status)
-- VALUES
-- ('CZ3101', 1, 1, 2, '2025-03-01 08:00:00', '2025-03-01 10:30:00', 'normal'),
-- ('MU5123', 2, 2, 3, '2025-03-01 12:00:00', '2025-03-01 14:20:00', 'delayed');
