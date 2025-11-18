-- ==========================================
-- 机场表 airport
-- ==========================================

CREATE TABLE IF NOT EXISTS airport (
    id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(100) NOT NULL COMMENT '机场名称',
    code VARCHAR(10) NOT NULL COMMENT '三字码，如 CAN',
    city_id INT NOT NULL COMMENT '所属城市',
    terminal_count INT DEFAULT 1 COMMENT '航站楼数量',

    FOREIGN KEY (city_id) REFERENCES city(id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- 示例数据（可删）
INSERT INTO airport (name, code, city_id, terminal_count) VALUES
('广州白云国际机场', 'CAN', 1, 2),
('北京首都国际机场', 'PEK', 2, 3),
('上海浦东国际机场', 'PVG', 3, 2);
