-- ==========================================
-- 城市表 city
-- ==========================================

CREATE TABLE IF NOT EXISTS city (
    id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(50) NOT NULL COMMENT '城市名称',
    code VARCHAR(10) COMMENT '城市代码',
    country VARCHAR(50) COMMENT '国家'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- 示例数据（可删）
INSERT INTO city (name, code, country) VALUES
('广州', 'GZ', '中国'),
('北京', 'BJ', '中国'),
('上海', 'SH', '中国');
