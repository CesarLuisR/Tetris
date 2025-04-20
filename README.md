🎮 Tetris Clone  
A simple Tetris clone built in C++ because why not? Blocks fall, lines vanish, and time disappears. It’s everything you’d expect from a proper time sink.

## 🚀 Build Instructions  
This project was developed in C++ and runs best on Windows environments. Here’s how you can build and run it:

### 🛠️ Visual Studio build system (recommended for Windows)  
1. Make sure you have **Visual Studio** installed with the "Desktop development with C++" workload.  
2. Open the `Tetris.sln` file.  
3. Go to **Build > Build Solution** or press `Ctrl + Shift + B`.  
4. Run the project with `Debug > Start Without Debugging` (`Ctrl + F5`).  

### 💻 Or with a good ol’ terminal (Windows only)  
You can compile the project with `g++` if you have [MinGW](https://sourceforge.net/projects/mingw/) or [MSYS2](https://www.msys2.org/) installed:

```bash
g++ *.cpp -o tetris
```
  
  ```⚠️ Nice try running this on Linux or macOS environments :)```
- It was definitely born and raised on Windows. Cross-platform? Maybe later...

🕹️ Controls
- ↑ – Rotate block
- ↓ – Place block
- → – Move block right
- ← – Move block left

You can change game-in variables like (grid width, grid height, block speed) in the CONSTANTS file 

![image](https://github.com/user-attachments/assets/1ad65bba-2a95-417c-873e-bd2a96054299)


