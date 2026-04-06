# xc_watermark_demo
信创水印demo - Qt-based screen watermark application

## 项目概述

这是一个基于Qt的屏幕水印应用程序，创建透明、始终置顶的覆盖窗口，在整个屏幕上重复显示文本内容。

- **跨平台支持**: Windows、Linux、macOS
- **Qt版本**: 支持Qt5和Qt6
- **C++标准**: C++17

## 屏幕水印 screen_watermark

### 方法一：使用CMake + VSCode（推荐）

#### 前置要求
- CMake 3.16+
- Qt5 或 Qt6
- VSCode（可选，用于开发）

#### 编译步骤

**使用CMake命令行：**
```bash
# 配置项目（Release模式）
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# 编译项目
cmake --build build --config Release -j

# 运行程序
./build/bin/screen_watermark
```

**使用VSCode：**
1. 打开VSCode，安装推荐插件：
   - CMake Tools
   - C/C++ Extension Pack
2. 使用快捷键 `Ctrl+Shift+P` 打开命令面板
3. 选择 `CMake: Configure` 配置项目
4. 选择 `CMake: Build` 编译项目
5. 按 `F5` 启动调试

**可用的VSCode任务：**
- `CMake: Configure` - 配置CMake项目
- `CMake: Build` - 编译项目（默认）
- `CMake: Clean` - 清理编译产物
- `CMake: Rebuild` - 重新编译
- `CMake: Configure Debug` - 配置Debug模式
- `CMake: Build Debug` - 编译Debug版本
- `Run screen_watermark` - 运行程序

### 方法二：使用QMake

```bash
cd screen_watermark
qmake
make
ls build/release/screen_watermark
```

## 代码架构

### 项目结构
```
xc_watermark_demo/
├── CMakeLists.txt              # CMake构建配置
├── xc_watermark_demo.code-workspace  # VSCode工作区配置
├── .vscode/                    # VSCode配置目录
│   ├── settings.json          # VSCode设置
│   ├── tasks.json             # 编译任务
│   ├── launch.json            # 调试配置
│   └── c_cpp_properties.json  # C++智能感知配置
└── screen_watermark/          # 主应用目录
    ├── main.cpp               # 程序入口
    ├── CTextWatermark.h/cpp   # 水印组件实现
    └── screen_watermark.pro   # qmake项目配置
```

### 核心组件：CTextWatermark

`CTextWatermark` 类（继承自 `QWidget`）负责创建和渲染水印覆盖窗口。

**主要特性：**
1. **透明覆盖窗口**：无框架、透明窗口，始终置顶
2. **Qt版本兼容性**：支持Qt 4.x、5.0+和5.10+
3. **水印渲染**：
   - `InitWaterMark()`: 预渲染水印文本图案到QPixmap
   - `paintEvent()`: 绘制带透明度的预渲染图案
4. **输入透明**：窗口对输入事件透明，允许点击穿透

### 水印常量

- `WM_TRANSPARENCY`: 0.6（60%不透明度）
- `WM_TEXT_COLOR`: 黑色 (0, 0, 0, 255)
- `WM_TEXT_FONT`: Arial, 12pt, Bold
- `WM_TEXT_ROW_SPACE`: 100像素（行间距）
- `WM_TEXT_COL_SPACE`: 100像素（列间距）

## 技术栈

- **语言**: C++17
- **框架**: Qt5/Qt6 (Core, Widgets)
- **构建系统**: CMake 3.16+ / QMake
- **IDE支持**: VSCode with CMake Tools

## 许可证

本项目为信创水印演示项目。