-- ==========================================
-- 用户 user
-- ==========================================

CREATE TABLE IF NOT EXISTS user (
    id INT PRIMARY KEY AUTO_INCREMENT,
    username VARCHAR(255) NOT NULL UNIQUE,
    password VARCHAR(255) NOT NULL,
    tickets_id TEXT NOT NULL,     -- 存 "1,3,5" 这样的字符串
    isSuper TINYINT(1) NOT NULL DEFAULT 0   -- 0=普通用户 1=管理员
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

INSERT INTO user (username, password, tickets_id, isSuper) VALUES ("admin", "123", "", 1);