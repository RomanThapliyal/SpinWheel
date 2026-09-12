🎡 SpinWheel
An interactive spinning wheel built from scratch in C++ and SFML
<p align="center"> <strong>🎮 A small 2D application focused on procedural geometry, real-time animation, and SFML.</strong> </p> <p align="center"> <img src="https://img.shields.io/badge/C%2B%2B-17-blue?style=for-the-badge&logo=cplusplus&logoColor=white" alt="C++"> <img src="https://img.shields.io/badge/SFML-3.x-green?style=for-the-badge&logo=sfml&logoColor=white" alt="SFML"> <img src="https://img.shields.io/badge/Graphics-2D-purple?style=for-the-badge" alt="2D Graphics"> <img src="https://img.shields.io/badge/Status-Complete-brightgreen?style=for-the-badge" alt="Status"> </p> <p align="center"> <a href="#-features">Features</a> • <a href="#-controls">Controls</a> • <a href="#-how-it-works">How It Works</a> • <a href="#-technical-details">Technical Details</a> • <a href="#-project-structure">Project Structure</a> • <a href="#-building">Building</a> • <a href="#-future-plans">Future Plans</a> </p>
🎮 About

🎡 SpinWheel is an interactive spinning wheel developed from scratch using C++ and SFML.

The wheel is procedurally constructed from individual segments rather than using a pre-rendered wheel image.

Each segment contains a name and an associated image. When the wheel spins, the segments, names, and images remain synchronized using a shared rotation state and trigonometric positioning.

The spinning animation uses elapsed frame time and friction-based deceleration to create smooth, frame-rate-independent motion.

✨ Features
Feature	Description
🎡 Procedural Wheel	Wheel segments are generated programmatically using SFML shapes
📐 Trigonometric Positioning	Names and images are positioned using sin() and cos()
🔄 Smooth Rotation	Wheel rotation is calculated using elapsed frame time
🛑 Friction-Based Deceleration	Spin force gradually decreases until the wheel stops
👤 Dynamic Names	Names rotate around the wheel based on their angular position
🖼️ Dynamic Images	Images remain synchronized with their corresponding segments
🖱️ Mouse Input	Click the Spin button to start the wheel
⌨️ Keyboard Input	Press S to start the wheel
🎨 Custom Colors	Each wheel segment uses a distinct color
⚡ Frame-Rate Independent	Animation is based on delta time rather than frame count
📦 Resource Loading	Fonts and images are loaded dynamically
🎮 Controls
Input	Action
🖱️ Left Click	Spin the wheel
⌨️ S	Spin the wheel

The wheel cannot be restarted while it is already spinning.

🧮 How It Works
🎡 Procedural Wheel Generation

The wheel is divided into a fixed number of segments.

For N segments:

Segment Angle = 360° / N


With seven segments:

360° / 7 ≈ 51.43°


Each segment is represented by a triangle whose first point is the center of the wheel.

The other two points are calculated using polar coordinates:

x = radius * cos(angle)
y = radius * sin(angle)


Conceptually:

                    P2
                   / \
                  /   \
                 /     \
                /       \
               /         \
              ●───────────
           Center         P1


Seven of these triangles form the complete wheel.

🔄 Rotation System

The wheel uses a rotational force that decreases over time.

The current spin force acts as the angular velocity:

float rotationAmount = spinForce * dt;


The wheel's rotation is then updated:

wheelRotation += rotationAmount;


Friction gradually reduces the spin force:

spinForce -= friction * dt;


Once the force reaches zero, the wheel stops:

          Start Spin
              │
              ▼
      ┌──────────────┐
      │ High Spin    │
      │    Force     │
      └──────┬───────┘
             │
             ▼
      ┌──────────────┐
      │   Rotation   │
      │      +       │
      │  Friction    │
      └──────┬───────┘
             │
             ▼
      ┌──────────────┐
      │ Lower Spin   │
      │    Force     │
      └──────┬───────┘
             │
             ▼
      ┌──────────────┐
      │    Stop      │
      └──────────────┘

⏱️ Delta Time

The animation uses the time elapsed between frames:

float dt = frameClock.restart().asSeconds();


Instead of rotating by a fixed amount every frame, the rotation amount depends on dt:

float rotationAmount = spinForce * dt;


This makes the animation independent of the frame rate.

Conceptually:

Higher FPS
    ↓
Smaller dt
    ↓
Smaller movement per frame

Lower FPS
    ↓
Larger dt
    ↓
Larger movement per frame


The overall movement remains approximately consistent over time.

👤 Dynamic Name Positioning

Names are positioned relative to the center of the wheel using their segment's middle angle.

float middleAngle =
    startAngle +
    (i + 0.5f) * segmentAngle +
    wheelRotation;


The position is then calculated using:

x = center.x + radius * cos(angle);
y = center.y + radius * sin(angle);


This means the names don't need to be manually repositioned during rotation.

Their positions are recalculated from the wheel's current rotation.

🖼️ Dynamic Image Positioning

Images use the same angular positioning system as the names.

The only difference is their distance from the center.

                 🖼️
             👤       👤

          🖼️    🎡       🖼️

             👤       👤
                 🖼️


Separate radii allow the names and images to occupy different regions around the wheel while remaining synchronized.

🏗️ Application Architecture

SpinWheel follows a simple real-time application structure:

                  ┌──────────────┐
                  │    Input     │
                  │              │
                  │ Keyboard     │
                  │ Mouse        │
                  └──────┬───────┘
                         │
                         ▼
                  ┌──────────────┐
                  │    Wheel     │
                  │              │
                  │ State        │
                  │ Animation    │
                  │ Geometry     │
                  │ Positioning  │
                  └──────┬───────┘
                         │
                         ▼
                  ┌──────────────┐
                  │   Renderer   │
                  │              │
                  │ Segments     │
                  │ Names        │
                  │ Images       │
                  │ Button       │
                  └──────────────┘


The Wheel class currently manages the wheel state, input handling, animation, resource loading, positioning, and rendering.

🔄 Main Loop

The application follows a traditional real-time loop:

       ┌─────────────────┐
       │  Handle Events  │
       └────────┬────────┘
                │
                ▼
       ┌─────────────────┐
       │     Update      │
       └────────┬────────┘
                │
                ▼
       ┌─────────────────┐
       │      Draw       │
       └────────┬────────┘
                │
                ▼
       ┌─────────────────┐
       │     Display     │
       └────────┬────────┘
                │
                └───────────────► Repeat


The three stages are:

🎮 Input

Processes:

Window events
Keyboard input
Mouse input
Spin button interaction
⚙️ Update

Handles:

Delta time
Rotation
Spin force
Friction
Name positions
Image positions
Spin state
🎨 Draw

Renders:

Wheel segments
Names
Images
Spin button
🧱 Class Design

The main application logic is contained in the:

class Wheel


The class is responsible for:

Wheel geometry
Segment configuration
Rotation state
Spin animation
Input handling
Resource loading
Name positioning
Image positioning
UI rendering

The project intentionally keeps the implementation compact while separating major operations into dedicated member functions.

🛠️ Technical Details
Language

C++

The project uses modern C++ features including:

std::array
std::optional
Member initializer lists
References
Strongly typed SFML event handling
Graphics

SFML 3.x

Used for:

Window management
2D shapes
Text rendering
Textures
Sprites
Input events
Transformations
Timing
Mathematics

The wheel uses:

Degrees → radians conversion
sin()
cos()
Polar coordinates
Angular displacement
Time-based motion
📁 Project Structure
SpinWheel/
│
├── Assets/
│   ├── Fonts/
│   │   └── PressStart2P-Regular.ttf
│   │
│   └── Photos/
│       ├── Roman.png
│       ├── Rudra.png
│       ├── Shreya.png
│       ├── Anurag.png
│       ├── Pratyush.png
│       ├── Ram.png
│       └── Himanshu.png
│
├── main.cpp
├── CMakeLists.txt
├── .gitignore
└── README.md


Adjust the structure above if your actual source files are organized differently.

🚀 Building
Requirements
C++ compiler
C++17 or later
SFML 3.x
CMake
Clone
git clone <your-repository-url>
cd SpinWheel

Build

Using CMake:

mkdir build
cd build
cmake ..
cmake --build .


Make sure the Assets directory is available relative to the executable.

🔮 Future Plans

Possible improvements:

 🎯 Add a fixed pointer to determine the selected segment
 🏆 Display the selected name after the wheel stops
 🎲 Add actual randomization of the starting spin
 ⚙️ Allow a variable number of segments
 📝 Load names from a configuration file
 🖼️ Load images dynamically from a configuration file
 🔊 Add spinning and stopping sound effects
 ✨ Add visual effects while spinning
 📐 Support dynamic window resizing
 🎨 Improve UI and wheel styling
🧠 What I Learned

This project was built to explore C++ and SFML through a small interactive graphics application.

The main concepts explored were:

🧮 Procedural 2D geometry
📐 Trigonometric positioning
🎡 Rotational transformations
⏱️ Delta-time-based animation
🛑 Friction-based deceleration
🎮 Keyboard and mouse input
🖼️ Sprite and texture management
🔤 Dynamic text positioning
🧱 C++ class design
🔄 Real-time application loops

The project helped reinforce how mathematical models can be directly translated into interactive 2D graphics.

👨‍💻 About

SpinWheel is a personal C++ project built using SFML.

The project focuses on understanding the fundamentals behind procedural 2D graphics and real-time animation rather than relying on a pre-rendered wheel.

Built with C++, SFML, mathematics, and a little bit of spinning. 🎡

<p align="center"> <strong>🎡 Thanks for checking out SpinWheel!</strong> </p>
