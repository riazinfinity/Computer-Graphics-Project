# 🏙️ Dhaka City Simulation - 3D Computer Graphics Project

A dynamic 3D computer graphics application showcasing a realistic simulation of Dhaka City's Motijheel commercial area with day/night cycles, animated metro trains, traffic, and environmental effects.

![Day Drone View](images/Day%20drone%20view.png)

## 📋 Overview

This project is a comprehensive 3D visualization application built with **OpenGL** and **GLUT** (OpenGL Utility Toolkit) in **C++**. It simulates key features of Dhaka's urban landscape, including:

- **Modern high-rise buildings** with realistic window patterns
- **Elevated metro rail system** with animated trains
- **Traffic simulation** with multiple cars in a traffic jam scenario
- **Dynamic lighting** with full day/night cycle support
- **Weather effects** including rain visualization
- **Iconic landmarks** like the Shapla Chattar monument
- **Smooth camera controls** for interactive exploration

## ✨ Features

### Visual Elements
- **Buildings**: Multi-story structures with detailed window patterns resembling Motijheel's office towers
- **Metro System**: Elevated rail with moving trains featuring 6-car compositions
- **Road Network**: Multi-lane roads with lane markings and vehicle traffic
- **Shapla Sculpture**: Stylized representation of Bangladesh's national flower at a roundabout
- **Sky System**: Sun/Moon and starfield for atmospheric depth

### Dynamic Simulation
- 🚊 **Animated Metro Train**: Continuously moving elevated train system
- 🚗 **Traffic Simulation**: Multiple cars on two-directional roads with realistic inching motion
- 💡 **Headlights & Taillights**: Visible vehicle lights during night mode
- ⭐ **Star Field**: 150 randomly placed stars visible at night
- 🌙 **Moon with Crescent**: Realistic moon rendering with subtle details

### Lighting & Atmosphere
- **Day Mode**:
  - Bright directional sunlight from realistic angle
  - Soft fill lights for balanced illumination
  - Clear blue sky background
  
- **Night Mode**:
  - Dim moonlight as primary light source
  - Dark ambient atmosphere
  - Vehicle headlights and taillights
  - Visible starfield

### Camera System
- **Free-roaming camera** with intuitive controls
- **Adjustable zoom** for detailed inspection
- **Smooth camera rotation** via pitch and yaw
- **Multiple viewing angles** for comprehensive scene exploration

## 🎮 Controls

| Key | Action |
|-----|--------|
| **D** | Set Day mode |
| **N** | Set Night mode |
| **Space** | Toggle Day/Night |
| **Arrow Keys** | Rotate camera view |
| **W/S** | Move camera forward/backward |
| **J/R** | Move camera left/right |
| **+/-** | Zoom in/out |
| **Esc** | Exit application |

## 🛠️ Building & Running

### Prerequisites
- **C++ compiler** (g++ or clang with C++11 support)
- **OpenGL** development libraries
- **GLUT** (FreeGLUT) library
- **libm** (math library)

### Linux Compilation
```bash
cd Dhaka_city
g++ main.cpp -o dhaka_city -lGL -lGLU -lglut -lm
./dhaka_city
```

### Installation on Ubuntu/Debian
```bash
# Install required packages
sudo apt-get install freeglut3-dev libglu1-mesa-dev mesa-common-dev

# Navigate to project directory and compile
cd Dhaka_city
g++ main.cpp -o dhaka_city -lGL -lGLU -lglut -lm
./dhaka_city
```

### Installation on macOS
```bash
# Using Homebrew
brew install glut

# Compile with macOS GLUT framework
g++ main.cpp -o dhaka_city -framework GLUT -framework OpenGL -lm
./dhaka_city
```

## 📸 Showcase

### Day View
![Day Drone View](images/Day%20drone%20view.png)

### Night View
![Night Drone View](images/Night%20drone%20view.png)

### Rainy Day Conditions
![Rain Day Time](images/Rain_day%20time.png)

### Rain with Aircraft
![Rain Day with Aircraft](images/Rain_day%20time%20with%20aroplane.png)

### Rainy Night
![Rain Night Time](images/Rain_night%20time.png)

### Night with Aircraft
![Rain Night with Aircraft](images/Rain_night%20time%20with%20aroplane.png)

## 🏗️ Project Structure

```
Computer-Graphics-Project/
├── Dhaka_city/
│   ├── main.cpp                 # Main application source code
│   ├── Dhaka_city.cbp           # Code::Blocks project file
│   ├── Dhaka_city.depend        # Dependency information
│   ├── Dhaka_city.layout        # IDE layout configuration
│   ├── bin/                     # Compiled executables
│   └── obj/                     # Object files
└── images/                      # Showcase screenshots
    ├── Day drone view.png
    ├── Night drone view.png
    ├── Rain_day time.png
    ├── Rain_day time with aroplane.png
    ├── Rain_night time.png
    └── Rain_night time with aroplane.png
```

## 💻 Technical Implementation

### Architecture
- **Object-Oriented Design**: Modular functions for different scene components
- **State Management**: Efficient scene state tracking (day/night, camera position, animation state)
- **Matrix Transformations**: Hierarchical transformation stacks for complex object placement

### Rendering Pipeline
1. **Setup Phase**: Initialize OpenGL context and lighting
2. **Draw Phase**: Render sky, buildings, roads, vehicles, and metro system
3. **Animation Phase**: Update entity positions based on elapsed time
4. **Display Phase**: Swap buffers for smooth animation (~60 FPS)

### Key Data Structures
- **Car Structure**: Tracks position, heading, and visual offset for traffic
- **Star Vector**: Dynamically generated random star positions
- **Material System**: Custom material properties for realistic shading

### Performance
- **Frame Rate**: ~60 FPS using GLUT timer callbacks
- **Optimization**: Disabled lighting during certain rendering passes
- **Animation**: Smooth motion via time-based incremental updates

## 📚 Learning Outcomes

This project demonstrates:
- **OpenGL Core Concepts**: Vertex transformations, lighting models, material properties
- **3D Graphics Techniques**: Perspective projection, depth testing, color blending
- **Camera Systems**: Free-roaming camera with Euler angles
- **Scene Animation**: Time-based animation and physics-like behavior
- **Real-world Application**: Practical use of graphics for urban simulation

## 🌟 Notable Features

### Realistic Lighting Model
- Multi-light system with ambient, diffuse, and specular components
- Context-aware lighting (day uses directional sun, night uses moon)
- Dynamic material properties for varied surface appearances

### Traffic Simulation
- Independent car movement with lane-based positioning
- Realistic traffic jam behavior with inching motion
- Vehicle-specific coloring for visual variety

### Atmospheric Effects
- Seamless day/night transition
- Sky color gradients (blue sky, dark night)
- Weather simulation hints through visual design

## 🚀 Future Enhancements

Potential improvements for future versions:
- Advanced weather effects (rain, snow, fog)
- Particle systems for smoke and dust
- Sound design and ambient audio
- More detailed building architectures
- Procedural city generation
- Real-time shadow mapping
- Advanced material texturing

## 📝 License

This project is open source and available for educational and personal use.

## 👤 Author

**Riaz Infinity** - [GitHub Profile](https://github.com/riazinfinity)

## 🤝 Contributing

Contributions are welcome! Feel free to submit issues, fork the repository, or create pull requests to improve this project.

---

**Enjoy exploring the 3D simulation of Dhaka City! 🇧🇩**
