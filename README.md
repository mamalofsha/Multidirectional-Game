Games made using C++ and GLSL.

There are two branches currently:

## main
Contains a 8-directional top-down game; sofar it has movement and collision detection.

https://www.youtube.com/watch?v=7mZ2RtSr9e8&list=PLwdJR1FaJbXXzOIdcbY-tOFUzTIiQoB6x&index=1
## UBI
This project is a technical implementation of The Castle Builder, developed to demonstrate game development skills in C++ with a focus on custom rendering, grid-based gameplay mechanics, and modular system design. The project uses OpenGL for rendering and XML for data persistence and configuration.

**Features and Highlights:
**
- * Custom Rendering Pipeline: Implements grid, textures, and UI rendering using OpenGL. Includes dynamic text rendering powered by FreeType and a shader-based approach for visual effects.
- * Grid-Based Gameplay: Supports isometric grid systems for building placement, with interactive workshops and decorations. Grid configuration and state are dynamically loaded from XML files.
- * UI Framework: Includes buttons, popups, and detailed info windows with hover, selection, and text display mechanics, designed for modularity and extensibility.
- * Data Persistence: Utilizes XML to save and load map states, including building placements and configurations, ensuring persistent game states.
- * Zoom and Pan Controls: Implements zooming and panning for better navigation, with constraints to ensure seamless user interaction.
    
**Game Systems:
**
- * Workshop System: Generates in-game currency (gold) with defined properties.
- * Decoration System: Enhances kingdom happiness with associated attributes.
- * Shop System: Paginated UI for buying workshops and decorations, populated dynamically from XML files.
- * Technical Challenges Addressed:
- * Developed a custom API for mouse interaction with game elements.
- * Designed modular XML parsers for flexible data management.
- * Implemented scalable rendering logic, including vertex buffer and texture management, to support interactive and dynamic game elements.
- * Ensured compatibility with different window sizes and scaling factors for a polished UI/UX.

- Note: Some game features, such as gold generation logic and level progression, were abstracted for simplicity, focusing on implementing the core systems and functionalities.

![photo_2025-01-07_13-06-37](https://github.com/user-attachments/assets/0f5de83c-48e0-45d3-875c-ab234ffd73cd)
