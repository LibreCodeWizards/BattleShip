# Battleship Game 🚢💥

Welcome to the Battleship game project! This README will guide you through navigating the project, compiling it, and running the game.

---

## Project Structure 📂

The project is organized as follows:

- **`/src`**: Contains the source code files for the game.
  - **`main.c`**: The entry point and driver code for the game.
  - **`/src/ast`**: Contains essential game functions, including the bot logic and other core components.

---

## How to Compile the Game ⚙️

We use **CMake** to manage the build process and **make** as the build tool. Follow the steps below to set up your environment and compile the game.

---

### 1. Install CMake 🛠️

To install CMake, visit the [CMake Download Page](https://cmake.org/download/) and download the version appropriate for your operating system.

---

### 2. Install `make` 📦

#### On **Linux** (Ubuntu/Debian):
1. Open a terminal and run:
   ```bash
   sudo apt update
   sudo apt install build-essential
   ```

#### On **Linux** (Fedora/RHEL):
1. Open a terminal and run:
   ```bash
   sudo dnf update
   sudo dnf groupinstall "Development Tools"
   ```

#### On **macOS**:
1. Install the Xcode Command Line Tools, which include `make`, by running:
   ```bash
   xcode-select --install
   ```

#### On **Windows**:
1. Install `make` using **MinGW**:
  - Download and install [MinGW](https://sourceforge.net/projects/mingw/).
  - During the installation, ensure you select the `mingw32-make` package.
2. Alternatively, you can install **WSL** (Windows Subsystem for Linux):
  - Follow the [official guide](https://learn.microsoft.com/en-us/windows/wsl/install) to set up WSL.
  - Use a Linux distribution in WSL where `make` is included.

---

### 3. Set Up Your Environment 🖥️

Make sure you have the following:
- **C Compiler**: A C compiler such as GCC (on Linux/macOS) or MinGW (on Windows).
- **CMake**: Installed as per step 1.

---

### 4. Build the Game 🔨

Follow these steps to build the game:

#### 1. Clone the Repository
Clone the repository to your local machine:
```bash
git clone https://github.com/LibreCodeWizards/BattleShip.git
cd BattleShip
```

#### 2. Create a Build Directory
It’s best to separate build files from the source code. Create a `build` directory:
```bash
mkdir build
cd build
```

#### 3. Run CMake
Run CMake from the `build` directory to configure the project:

- **Linux/macOS**:
  ```bash
  cmake ..
  ```

- **Windows**:
  ```bash
  cmake -G "MinGW Makefiles" ..
  ```

#### 4. Compile the Game
Use `make` to compile the project:

- **Linux/macOS**:
  ```bash
  make
  ```

- **Windows**:
  ```bash
  C:\MinGW\bin\mingw32-make.exe
  ```

---

### 5. Run the Game 🏁

After building the game, you can run it:

- **Linux/macOS**:
  ```bash
  ./Battleship
  ```

- **Windows**:
  ```bash
  Battleship.exe
  ```

Enjoy the game! 🚀