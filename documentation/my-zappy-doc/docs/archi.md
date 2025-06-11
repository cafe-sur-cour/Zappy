# The Zappy - Project Architecture
## 🎯 Overview

Zappy is a sophisticated multiplayer network strategy game built with a distributed architecture consisting of three main components: a C-based server, a C++ GUI client, and Python AI bots. The project demonstrates advanced software engineering principles including network programming, real-time graphics, and artificial intelligence.

## 🏗️ Global Architecture

```
                    🌐 ZAPPY ECOSYSTEM
    ┌─────────────────────────────────────────────────────────┐
    │                                                         │
    │  ┌─────────────┐    ┌─────────────┐    ┌─────────────┐  │
    │  │             │    │             │    │             │  │
    │  │   🖥️ SERVER   │◄──►│  🎮 GUI      │    │  🤖 AI BOT   │  │
    │  │     (C)     │    │   CLIENT    │    │  (Python)   │  │
    │  │             │    │   (C++)     │    │             │  │
    │  └─────────────┘    └─────────────┘    └─────────────┘  │
    │         │                   │                   │       │
    │         │                   │                   │       │
    │    ┌────▼────┐         ┌────▼────┐         ┌────▼────┐  │
    │    │ Network │         │Graphics │         │  AI     │  │
    │    │Protocol │         │Rendering│         │Strategy │  │
    │    └─────────┘         └─────────┘         └─────────┘  │
    └─────────────────────────────────────────────────────────┘
```

## 🖥️ Server Component (C)

### Architecture Overview
The server is the central authority managing all game logic, state, and communications.

```
📦 Zappy Server Architecture
├── 🌐 Network Layer
│   ├── Socket Management
│   ├── Client Connection Handling
│   ├── Message Protocol
│   └── Polling System
├── 🎮 Game Engine
│   ├── Game State Management
│   ├── Rule Enforcement
│   ├── Victory Condition Checking
│   └── Time Management
├── 🗺️ Map System
│   ├── Tile-based World
│   ├── Resource Distribution
│   ├── Coordinate System
│   └── Wraparound Logic
├── 👥 Player Management
│   ├── Team Organization
│   ├── Player Lifecycle
│   ├── Action Queuing
│   └── State Synchronization
└── 📊 Communication Hub
    ├── AI Client Protocol
    ├── GUI Broadcasting
    ├── Command Processing
    └── Response Management
```

### Key Technologies
- **Language**: C
- **Networking**: BSD Sockets
- **I/O Model**: Polling (poll/select)
- **Memory Management**: Manual allocation/deallocation
- **Data Structures**: Linked lists, arrays
- **Build System**: Make

### Core Responsibilities
1. **Game State Authority**: Maintains authoritative game state
2. **Network Hub**: Manages all client connections
3. **Rule Enforcement**: Validates all actions and commands
4. **Resource Management**: Distributes and tracks map resources
5. **Real-time Broadcasting**: Sends updates to GUI clients

## 🎮 GUI Client (C++)

### Architecture Overview
The GUI provides real-time visualization of the game state with advanced 3D graphics.

```
📦 GUI Client Architecture
├── 🎨 Graphics Layer
│   ├── RayLib Wrapper
│   ├── 3D Rendering Engine
│   ├── Model Management
│   └── Shader System
├── 🎵 Audio System
│   ├── SFML Integration
│   ├── Sound Effects
│   ├── Background Music
│   └── Audio Management
├── 🌐 Network Client
│   ├── Server Communication
│   ├── Message Parsing
│   ├── Protocol Implementation
│   └── Connection Management
├── 🎮 Game Visualization
│   ├── Map Rendering
│   ├── Player Representation
│   ├── Resource Display
│   └── Animation System
├── 🖱️ User Interface
│   ├── HUD Components
│   ├── Control Panels
│   ├── Information Display
│   └── Interactive Elements
└── 📹 Camera System
    ├── 3D Camera Control
    ├── View Management
    ├── Perspective Handling
    └── Movement System
```

### Key Technologies
- **Language**: C++20
- **Graphics**: RayLib (3D rendering)
- **Audio**: SFML (Sound and Music Library)
- **GUI Framework**: Custom UI system
- **Build System**: Make with modular libraries
- **Testing**: Google Test (GTest)

### Core Responsibilities
1. **Real-time Visualization**: Renders game state in 3D
2. **User Interface**: Provides intuitive controls and information
3. **Audio Experience**: Manages sound effects and music
4. **Camera Control**: Handles 3D navigation and viewing
5. **State Synchronization**: Keeps visual state in sync with server

## 🤖 AI Bot (Python)

### Architecture Overview
The AI system implements intelligent automated players with multi-process architecture.

```
📦 AI Bot Architecture
├── 🚀 Process Management
│   ├── Multi-Process System
│   ├── Fork-based Players
│   ├── Signal Handling
│   └── Resource Cleanup
├── 🧠 Intelligence Engine
│   ├── Decision Making
│   ├── Strategy Implementation
│   ├── Resource Prioritization
│   └── Level Progression
├── 🌐 Communication System
│   ├── Socket Management
│   ├── Protocol Implementation
│   ├── Command Queuing
│   └── Response Handling
├── 📡 Team Coordination
│   ├── Encrypted Broadcasting
│   ├── Message Hashing
│   ├── Team Communication
│   └── Coordination Protocols
├── 🎮 Behavior System
│   ├── Exploration (Roomba Strategy)
│   ├── Resource Collection
│   ├── Survival Management
│   └── Goal Achievement
└── 🔧 Utility Systems
    ├── CLI Argument Parsing
    ├── Logging System
    ├── Error Handling
    └── Configuration Management
```

### Key Technologies
- **Language**: Python 3.x
- **Networking**: Standard socket library
- **Process Management**: os.fork() for multi-processing
- **Threading**: For concurrent communication
- **Encryption**: Custom hash system for team communication
- **Testing**: Pytest framework

### Core Responsibilities
1. **Autonomous Gameplay**: Makes intelligent decisions independently
2. **Team Coordination**: Collaborates with other AI instances
3. **Resource Management**: Efficiently collects and uses resources
4. **Strategic Planning**: Implements long-term victory strategies
5. **Process Management**: Handles multiple player instances

## 🌐 Network Protocol

### Communication Architecture
The three components communicate through a custom TCP-based protocol.

```
    🖥️ SERVER (Port 4242)
         │
    ┌────┴────┐
    │         │
    ▼         ▼
🎮 GUI    🤖 AI BOTS
(Observer) (Players)
```

### Protocol Types

#### AI ↔ Server Protocol
- **Connection**: Team name identification
- **Commands**: Movement, resource management, information queries
- **Responses**: Action results, game state updates
- **Format**: Text-based command protocol

#### GUI ↔ Server Protocol
- **Connection**: "GRAPHIC" identifier
- **Updates**: Real-time game state broadcasts
- **Commands**: Map size, tile contents, player info
- **Format**: Structured message protocol

### Message Flow
1. **Client Connection**: Handshake with team name or "GRAPHIC"
2. **Authentication**: Server validates and assigns client type
3. **Game Communication**: Commands and responses flow
4. **State Broadcasting**: Server pushes updates to GUI clients
5. **Disconnection Handling**: Graceful cleanup of resources

## 🔧 Build System Architecture

### Modular Build Structure
```
📦 Build System
├── 🖥️ Server Build
│   ├── Main Makefile
│   ├── Network Library
│   ├── My Library (utilities)
│   └── Test Integration
├── 🎮 GUI Build
│   ├── Main Makefile
│   ├── RayLib Wrapper Library
│   ├── Communication Library
│   ├── Audio Library
│   └── Test Framework
├── 🤖 AI Build
│   ├── Python Script Linking
│   ├── Permission Management
│   └── Test Suite Integration
└── 🧪 Testing Infrastructure
    ├── Unit Tests (Criterion, GTest, Pytest)
    ├── Functional Tests
    ├── Coverage Reports
    └── Integration Testing
```

### Build Targets
- **Individual Components**: `make` in each directory
- **Global Build**: `make` in root directory
- **Testing**: `make tests_run` for comprehensive testing
- **Coverage**: `make coverage` for coverage reports
- **Cleanup**: `make clean/fclean` for cleanup

## 📊 Data Flow Architecture

### Game State Synchronization
```
┌─────────────┐    Commands     ┌─────────────┐
│   AI Bots   │────────────────►│   Server    │
└─────────────┘                 │             │
                                 │ ┌─────────┐ │    Updates
┌─────────────┐    Requests     │ │  Game   │ │ ──────────┐
│ GUI Client  │◄────────────────│ │ Engine  │ │           │
└─────────────┘                 │ └─────────┘ │           ▼
       ▲                        └─────────────┘    ┌─────────────┐
       │                               │           │ GUI Client  │
       │          State Updates        │           │  (Visual)   │
       └───────────────────────────────┘           └─────────────┘
```

### Resource Management Flow
1. **Map Generation**: Server creates resource distribution
2. **Discovery**: AI bots explore and find resources
3. **Collection**: Bots collect resources based on strategy
4. **State Updates**: Server broadcasts changes to GUI
5. **Visualization**: GUI renders updated map state

## 🏆 Victory Condition Architecture

### Elevation System
```
Level 1 ──► Level 2 ──► Level 3 ──► ... ──► Level 8
   │           │           │                  │
   ▼           ▼           ▼                  ▼
Resources   Resources   Resources        🏆 VICTORY
Required    Required    Required      (6 players
                                      at level 8)
```

### Victory Detection
1. **Level Tracking**: Server monitors player levels
2. **Team Counting**: Counts players at maximum level per team
3. **Victory Trigger**: First team to achieve 6 players at level 8
4. **Game End**: Server broadcasts victory and handles cleanup

## 🔐 Security Architecture

### Team Communication Security
- **Encrypted Broadcasting**: AI teams use hash-based encryption
- **Message Authentication**: Prevents message replay attacks
- **Team Isolation**: Teams cannot intercept other teams' communications
- **Index-based Security**: Messages include sequence numbers

### Network Security
- **Input Validation**: Server validates all incoming commands
- **Resource Limits**: Prevents resource exhaustion attacks
- **Connection Management**: Limits concurrent connections
- **Protocol Compliance**: Strict adherence to defined protocols

## 🚀 Performance Architecture

### Scalability Features
- **Non-blocking I/O**: Server uses polling for efficient networking
- **Modular Design**: Components can be optimized independently
- **Resource Pooling**: Efficient memory management strategies
- **Event-driven Processing**: Responsive to client actions

### Optimization Strategies
- **Command Queuing**: Prioritized action processing
- **State Caching**: Efficient state management
- **Batched Updates**: Grouped GUI notifications
- **Memory Management**: Structured allocation patterns

## 🧪 Testing Architecture

### Multi-Language Testing Strategy
```
📦 Testing Framework
├── 🖥️ Server Tests (Criterion)
│   ├── Unit Tests
│   ├── Integration Tests
│   ├── Network Tests
│   └── Memory Leak Detection
├── 🎮 GUI Tests (Google Test)
│   ├── Component Tests
│   ├── Rendering Tests
│   ├── UI Tests
│   └── Integration Tests
├── 🤖 AI Tests (Pytest)
│   ├── Logic Tests
│   ├── Communication Tests
│   ├── Strategy Tests
│   └── Process Tests
└── 🔗 Functional Tests
    ├── End-to-End Testing
    ├── Protocol Validation
    ├── Performance Testing
    └── Stress Testing
```

### Quality Assurance
- **Code Coverage**: Comprehensive coverage reporting
- **Static Analysis**: Code quality validation
- **Memory Testing**: Leak detection and validation
- **Performance Profiling**: Optimization identification

## 🔧 Development Architecture

### Version Control Strategy
- **Git Workflow**: Feature branches with structured commits
- **Gitmoji Convention**: Standardized commit messaging
- **Code Review**: Pull request workflow
- **Documentation**: Comprehensive technical documentation

### Development Environment
- **Cross-platform Support**: Linux-focused development
- **IDE Integration**: VS Code compatibility
- **Build Automation**: Automated build and test pipelines
- **Documentation Generation**: Automated documentation updates

The Zappy project demonstrates a sophisticated understanding of distributed system design, network programming, real-time graphics, and artificial intelligence, showcasing advanced software engineering principles through its multi-language, multi-component architecture.
