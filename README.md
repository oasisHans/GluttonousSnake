# Gluttonous Snake (OOP Version)

这是一个基于 C++ 面向对象思想开发的贪吃蛇游戏。

## 功能特点

- 完整的面向对象架构 (Manager-Entity-System)
- 支持多级菜单设置
- 预判式碰撞检测系统
- 基于 EasyX 图形库开发

## 环境依赖

- 编译器：MinGW (支持 C++11)
- 图形库：EasyX for MinGW
- 构建工具：CMake

## 如何编译

1. 确保安装了 EasyX 并在 CMakeLists.txt 中配置好路径。
2. 创建 build 目录：`mkdir build && cd build`
3. 执行 CMake：`cmake ..`
4. 编译：`mingw32-make`
