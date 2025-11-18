-- ==========================================
-- 飞机表 airplane
-- ==========================================

CREATE TABLE IF NOT EXISTS airplane (
    id INT PRIMARY KEY AUTO_INCREMENT,
    model VARCHAR(50) NOT NULL COMMENT '机型，如 A320',
    seats_economy INT DEFAULT 0,
    seats_business INT DEFAULT 0,
    seats_first INT DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- 示例数据（可删）
INSERT INTO airplane (model, seats_economy, seats_business, seats_first) VALUES
('A320', 150, 20, 10),
('B737', 160, 18, 8),
('A350', 260, 36, 20);
