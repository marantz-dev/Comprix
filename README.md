# 🎧 Comprix — Broadband Compressor Plugin

**Comprix** is a broadband compressor plugin built using the powerful [JUCE](https://juce.com) framework. Designed for smooth, transparent dynamic range control, Comprix is ideal for mixing, mastering, or even creative sound design.

---

## 🚀 Features

- 🌀 Smooth broadband compression
- 🎚️ Adjustable Threshold, Ratio, Attack, Release, and Makeup Gain
- 📉 Real-time gain reduction metering
- 💻 Built with C++ and JUCE for high performance
- 🎛️ Clean and responsive GUI

---

## 🔧 Installation

### Prerequisites

- [JUCE Framework](https://juce.com/get-juce) (tested with JUCE vX.X.X)
- C++17 compatible compiler
- CMake 3.15 or newer
- A supported DAW that loads VST3 or AU plugins

### Build Instructions

1. Clone the repository:

   ```bash
   git clone https://github.com/marantz-dev/Comprix.git
   cd Comprix
   ```
2.	Build with CMake:
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```
3.	Copy the resulting plugin to your DAW’s plugin folder if needed.

⸻

🖥️ Plugin Formats
	•	✅ VST3 (Windows & macOS)
	•	✅ AU (macOS only)

Other formats can be enabled via JUCE Projucer or CMake options.

🧪 Status

📦 Current State: Functional prototype
🚧 In Progress: GUI refinements, oversampling support, presets system
🔜 Coming Soon: Sidechain support, optional multiband mode

⸻

🤝 Contributing

Found a bug or have a feature request? Open an issue or submit a pull request! All contributions are welcome and appreciated.

⸻

📄 License

This project is licensed under the MIT License.
See the LICENSE file for more details.

⸻

✨ Credits
	•	Developed with JUCE 
	•	Big thanks to the open-source DSP community

⸻

📬 Contact

For suggestions, feedback, or collaborations:
📨 Open an issue on GitHub: https://github.com/marantz-dev/Comprix
