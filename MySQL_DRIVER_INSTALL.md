# Qt 6.10.0 MySQL 驱动安装指南

## 问题
Qt 6.10.0 默认不包含 QMYSQL 驱动

## 解决方案

### 需要的文件
1. **qsqlmysql.dll** - Qt MySQL 驱动插件
2. **libmysql.dll** - MySQL 客户端库

### 文件位置

#### qsqlmysql.dll 复制到：
```
C:\Qt\6.10.0\mingw_64\plugins\sqldrivers\qsqlmysql.dll
```

#### libmysql.dll 复制到：
```
C:\Qt\6.10.0\mingw_64\bin\libmysql.dll
```

### 获取 qsqlmysql.dll 的方法

#### 方法1: 下载预编译版本（推荐）

访问以下链接下载：
https://github.com/thecodemonkey86/qt_mysql_driver/releases

选择：qt-6.x.x-mingw-x64-mysql-driver.zip (选择最接近 6.10 的版本)

解压后找到 qsqlmysql.dll 复制到上述目录

#### 方法2: 从其他 Qt 安装复制

如果你有其他 Qt 版本(如 Qt 5.15)包含 MySQL 驱动：
1. 找到旧版本的 plugins/sqldrivers/qsqlmysql.dll
2. 复制到 Qt 6.10.0 (可能不兼容，不推荐)

#### 方法3: 自己编译

需要 Qt 源码，运行项目根目录下的 install_mysql_driver.ps1

### 获取 libmysql.dll

从 MySQL 安装目录复制：
```
C:\Program Files\MySQL\MySQL Server 8.0\lib\libmysql.dll
```

### 验证安装

运行测试程序：
```
test/check_mysql_driver.exe
```

应该看到输出：
```
✅ QMYSQL 驱动已安装！
```

### 快速复制命令 (PowerShell)

```powershell
# 复制 MySQL 客户端库
Copy-Item "C:\Program Files\MySQL\MySQL Server 8.0\lib\libmysql.dll" `
          "C:\Qt\6.10.0\mingw_64\bin\"

# 创建插件目录（如果不存在）
New-Item -ItemType Directory -Path "C:\Qt\6.10.0\mingw_64\plugins\sqldrivers" -Force

# 手动下载 qsqlmysql.dll 后：
# Copy-Item "下载目录\qsqlmysql.dll" `
#           "C:\Qt\6.10.0\mingw_64\plugins\sqldrivers\"
```

### 常见问题

#### Q: 找不到 qsqlmysql.dll 下载链接
A: 
1. 访问 https://github.com/thecodemonkey86/qt_mysql_driver/releases
2. 下载最新版本（Qt 6.8 或 6.9 的驱动也可能兼容 6.10）
3. 或者自己编译（运行 install_mysql_driver.ps1）

#### Q: 运行程序时提示"无法启动程序，因为缺少libmysql.dll"
A: 将 libmysql.dll 复制到：
   - C:\Qt\6.10.0\mingw_64\bin\
   - 或项目的 exe 所在目录

#### Q: QMYSQL 驱动仍然未找到
A: 
   1. 确认文件路径正确
   2. 重启 Qt Creator
   3. 检查 Qt 版本是否匹配（6.10.0）
   4. 检查编译器是否匹配（MinGW 64-bit）
