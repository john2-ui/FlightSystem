-- ==========================================
-- 机票表 ticket
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

-- 示例数据（可删）
-- INSERT INTO ticket (flight_id, class, price, total_seats, remain_seats) VALUES
-- (1, 'economy', 899.00, 150, 120),
-- (1, 'business', 1999.00, 20, 15),
-- (2, 'economy', 750.50, 160, 140);
