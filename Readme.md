# 🌐 HtmlParser

[![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

A lightweight and fast C++ library for parsing HTML documents. It allows you to load HTML from strings or files, build a DOM tree, and easily extract data without external dependencies.

## Features
- 📦 Parse HTML strings
- 📝 Extract text content, attributes, and inner HTML
- 🌍 Cross-platform: Linux, Windows, macOS
- ⚡ Zero external dependencies (uses only the C++ Standard Library)

##  Requirements
- Compiler with **C++17** support or newer
- **CMake 3.14+**

## 🛠 Build & Installation

```bash
# Clone the repository
git clone https://github.com/ZaxxorBunnyNya/HtmlParser.git
cd HtmlParser

# Build using CMake
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

After building, you can integrate the library into your project using `find_package(HtmlParser)` or by adding the `include/` directory to your compiler's include path and linking the generated library.

## Usage Example

```cpp
#include <Parser.h>
#include <iostream>

int main() {
    auto parser = HtmlParser::Parser();

    parser.Parse("<p>TEST</p>");

    const auto root = parser.getRoot();
    const auto element = root->getChildren()[0];
    std::cout << element->getChildren()->getText() << std::endl;

    return 0;
}
```

## 📄 License
This project is licensed under the [MIT License](LICENSE). See the `LICENSE` file for details.
