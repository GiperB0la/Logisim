# 🔌 Logisim

**Logisim** — an application for building digital circuits from basic logic gates.  
The graphical interface is built with **SFML**.

## ✨ Features
- Add and connect logic elements:
  - `And`, `Or`, `Xor`, `Nand`, `Nor`, `Xnor`
- Input (`In`) and output (`Out`) pins
- Visual rendering of connections
- Toggle inputs with instant circuit update
- Scalable workspace

### 🖼️ Screenshot
<picture>
  <source media="(prefers-color-scheme: dark)" srcset="https://github.com/GiperB0la/Logisim/blob/main/Screen.jpg">
  <img alt="Screen" src="https://github.com/GiperB0la/Logisim/blob/main/Screen.jpg">
</picture>

---

## 🛠️ Build

### 🔹 Prerequisites
- **CMake ≥ 3.16**  
- **C++17 compiler** (MSVC, g++, clang++)  
- **SFML 2.6.2** ([download](https://www.sfml-dev.org/download/sfml/2.6.2/))

---

```powershell
# Clone project
git clone https://github.com/GiperB0la/Logisim.git
cd Logisim
mkdir build && cd build

# Configure (set SFML_DIR to your SFML cmake folder)
cmake .. -DSFML_DIR="path/to/SFML/lib/cmake/SFML"

# Build (Release version)
cmake --build . --config Release
