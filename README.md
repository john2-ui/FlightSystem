# FlightSystem

1. 项目目录结构
    
    ```markdown
    
    FlightSystem/
    │
    ├─ CMakeLists.txt                          # Qt 项目入口文件
    ├─ README.md                               # 项目说明
    ├─ config/                                 # 配置文件模块
    ├─ db/                                     # 数据库模块（DAO 层）
    ├─ model/                                  # 数据模型模块
    ├─ core/                                   # 核心业务逻辑（Service 层）
    ├─ ui_client/                               # 用户端 UI
    ├─ ui_admin/                                # 管理端 UI
    └─ resources/                               # 资源文件（图标、QSS 样式、图片）
    
    ```
    
2. 说明：空文件可以使用**.gitkeep**文件占位。后端和数据库端在完成开发前需要提供mock数据供其他板块开发
3. 分支说明：
    1. feature：开发分支
        1. feature/db：数据库分支(A负责)，管理开发/db，/model，/config
        2. feature/backend：后端分支，(B负责),负责核心业务逻辑，也就是Service层，管理开发/backend
        3. feature/ui-client：客户端UI分支，(C负责)负责客户端UI开发，不可直接修改/db或/backend下文件
        4. feature/ui-admin：管理端UI分支，(D负责)开发管理端UI，不可直接修改/db或/backend下文件
    2. develop：整合分支
        1. 整个各个feature分支，测试后合并到main分支
    3. main：分支稳定版本
        1. 稳定版本，用于最终展示
4. feature/ui-client以及feature/ui-admin分支共同使用resources文件夹,CMakelists.txt以及README.md所有人均可修改提交
5. 开发流程建议
    1. 每个人在自己的feature分支开发，完成后合并到develop分支
    2. 合并前确保自己的模块可以独立运行(UI可以用mock数据调用backend接口)
    3. develop分支功能整合完成后，测试无误由管理员合并到main分支
    4. 每次开发前先git pull origin develop，避免冲突