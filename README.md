# Battleship Game 🚢💥

Welcome to the Battleship game project! This README will guide you through navigating the project, compiling it, and running the game.

## Project Structure 📂

The project is organized as follows:

- **`/src`**: Contains the source code files for the game.
  - **`main.c`**: The entry point and driver code for the game.
  - **`/src/ast`**: Contains essential game functions, including the bot logic and other core components.

## How to Compile the Game ⚙️

To make our lives easier (actually, this was done by me, @aza161), we decided to use **CMake** for managing the build process.

### 1. Install CMake 🛠️
To get started, you’ll need to install CMake. Visit the [CMake Download Page](https://cmake.org/download/) and download the appropriate version for your operating system.

### 2. Install `make` 📦
You’ll also need to install the **make** tool to compile the project. Here are the instructions based on your operating system:

#### On **Linux** (Ubuntu/Debian):
1. Open a terminal and run:
   ```bash
   sudo apt update
   sudo apt install build-essential
   ```
   This will install `make` and other required development tools.

#### On **Linux** (Fedora/RHEL):
1. Open a terminal and run:
   ```bash
   sudo dnf update
   sudo dnf install build-essential
   ```
   This will install `make` and other required development tools.

#### On **macOS**:
1. Install the Xcode Command Line Tools, which include `make`, by running:
   ```bash
   xcode-select --install
   ```
   If you already have Xcode installed, `make` should already be available.

#### On **Windows**:
1. On Windows, you can install `make` by installing **MinGW** or **Cygwin** (which provides a Unix-like environment). Alternatively, you can install **WSL** (Windows Subsystem for Linux) for a more native Unix experience.
  - **MinGW**: Download and install [MinGW](https://sourceforge.net/projects/mingw/), and make sure to select the `mingw32-make` package.
  - **WSL**: Follow the instructions to [install WSL](https://docs.microsoft.com/en-us/windows/wsl/install) and then use a Linux environment where `make` is already available.

### 3. Set Up Your Development Environment 🖥️

Make sure you have the necessary tools installed:
- **C Compiler**: Make sure you have a C compiler (like GCC on Linux/macOS or MinGW on Windows).
- **CMake**: Install CMake as mentioned in step 1.

### 4. Build the Game 🔨

After installing the necessary tools, follow these steps to build the project:

1. **Clone the Repository**: Clone the repository to your local machine:
   ```bash
   git clone https://github.com/LibreCodeWizards/BattleShip.git
   cd BattleShip
   ```

2. **Create a Build Directory**: It's a good practice to keep build files separate from your source code. Create a `build` directory:
   ```bash
   mkdir build
   cd build
   ```

3. **Run CMake**: Run CMake from within the `build` directory to generate the necessary build files:
   ```bash
   cmake ..
   ```

4. **Compile the Project**: After CMake configures the project, you can build it using `make`:
   ```bash
   make
   ```

### 5. Run the Game 🏁

After building the game, you can run it with the following command:
```bash
./Battleship
```

This will start the game and you can begin playing.