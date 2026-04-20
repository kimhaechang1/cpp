# Session Checkpoint: 2026-04-20

## 🎯 Current Goal
- Mastering Network Protocol basics (Winsock2) and moving towards TCP communication.

## ✅ Completed Tasks
- **Module 12 Step 1**: Winsock2 environment setup completed.
- **RAII Implementation**: Created `NetworkManager` class with `WSAStartup`/`WSACleanup` in constructor/destructor.
- **CS Knowledge**: Deeply understood `WORD`, `MAKEWORD`, `0xFF` bitmasking, and DLL loading mechanics.
- **Tooling**: Updated `tasks.json` for Module 12 and configured Allman-style formatting in `.clang-format`.

## 🚀 Next Steps
- **Module 12 Step 2**: TCP Protocol (3-Way Handshake theory + `socket()` function).
- **Socket Creation**: Adding socket management to the `NetworkManager` class.

## ⚠️ Key Constraints & Lessons
- **Initialization Order**: `WSAStartup` must be called before ANY other Winsock functions (like `socket()`).
- **Bitwise Logic**: `(value >> 8) & 0xFF` extracts the high byte, `value & 0xFF` extracts the low byte.
- **RAII is King**: Managing network resources via class lifetimes prevents DLL resource leaks.
