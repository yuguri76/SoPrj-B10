# SoPrj-B10 (POS)

SoPrj-B10 (POS) uses CMake for its project configuration and generation. In theory, it should be compatible with any project generator supported by CMake, C11 compatible compiler and an IDE with CMake support. 

## Windows

### Visual Studio 
- On the Start Menu, open the `x64 Native Tools Command Prompt for Visual Studio`.

- Clone the repository:
  ```cmd
  git clone --recursive https://github.com/KhoraLee/SoPrj-B10
  cd SoPrj-B10
  ```

- Generate the project:
  ```cmd
  cmake -S . -B build
  ```
  The line above will generate a Visual Studio project inside a folder called `build`.

## macOS (Xcode)

- Install Xcode at App Store.

- Install [`brew`](https://brew.sh).

- Install dependencies with `brew`.

  ```sh
  brew install git cmake
  ```

- Clone this repo.
  ```sh
  git clone --recursive https://github.com/KhoraLee/SoPrj-B10
  cd SoPrj-B10
  ```

- Generate Xcode project.

  ```
  cmake -GXcode -S . -B build
  ```
  This example will generate a Xcode project inside a folder called `build`.

- Open Xcode project `vita3k.xcodeproj` generated in `build` directory.

- Build the project using the Xcode UI. If needed, the build process can be invoked as well the same way as with the other platforms using a terminal:
  ```sh
  cmake --build build
  ```