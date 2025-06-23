# The Zappy - Server

## 🎯 Overview

The Zappy server is the core component of the multiplayer strategy game, managing all game logic, network communications, and state synchronization between AI clients and GUI viewers. It handles real-time gameplay where teams compete to elevate players to the maximum level.

## 🏗️ Architecture Overview

The server follows a modular architecture with clear separation of concerns:

```
📦 Zappy Server
├── 🌐 Network Layer     - Client connections and messaging
├── 🎮 Game Engine       - Core game logic and state management
├── 🗺️  Map System       - Tile-based world with resources
├── 👥 Team Management   - Player teams and coordination
├── 🥚 Egg System        - Player spawning mechanism
└── 📊 GUI Communication - Real-time game state broadcasting
```

## 🚀 Server Initialization

### Main Entry Point
The server starts in [`main.c`](server/src/main.c):

```c
int main(int argc, char **argv)
{
    zappy_t *zappy = init_server(argc, argv);  // Initialize server
    init_game(zappy);                          // Setup game world
    start_protocol(zappy);                     // Begin game loop
    free_zappy(zappy);                         // Cleanup
}
```

### Server Setup Process

1. **Parameter Parsing** - [`params.c`](server/src/params.c)
   - Validates command line arguments
   - Sets up port, map dimensions, teams, client limits, and frequency

2. **Network Initialization** - [`server.c`](server/src/server.c)
   - Creates socket and binds to specified port
   - Sets up polling for incoming connections
   - Prepares for multiple client handling

3. **Game World Creation** - [`game/game.c`](server/src/game/game.c)
   - Initializes map with specified dimensions
   - Distributes resources across tiles using density algorithms
   - Creates team structures
   - Spawns initial eggs for players

## 🌐 Network Architecture

### Connection Management
The server handles multiple connection types:

- **AI Clients**: Game players that send commands
- **GUI Clients**: Observers that receive game state updates

### Client Connection Flow
From [`accept.c`](server/src/accept.c):

1. Server accepts new connection
2. Sends "WELCOME" message
3. Reads team name or "GRAPHIC" identifier
4. Validates team name and availability
5. Adds client to appropriate team or GUI list
6. Sends connection confirmation with game parameters

```c
int accept_client(zappy_t *zappy)
{
    int new_sockfd = accept_connection(zappy->network->sockfd);
    char *message = check_team_name(zappy, new_sockfd);
    
    if (strcmp(message, "GRAPHIC") == 0) {
        // Add to GUI clients
    } else {
        // Add to team
    }
}
```

## 🗺️ Map System

### Map Structure
The game world consists of a 2D grid of tiles, each containing:

- **Resources**: Food, Linemate, Deraumere, Sibur, Mendiane, Phiras, Thystame
- **Players**: Multiple players can occupy the same tile
- **Eggs**: Spawning points for new players

### Resource Distribution
From [`game/game.c`](server/src/game/game.c):

```c
static void distribute_resources(zappy_t *server)
{
    float density[7] = {0.5, 0.3, 0.15, 0.1, 0.1, 0.08, 0.05};
    // Distributes resources based on map size and density ratios
}
```

Resources are distributed using:
- **Fisher-Yates shuffle** for random tile selection
- **Density-based allocation** ensuring balanced resource availability
- **Wraparound coordinates** for seamless world boundaries

## 👥 Team and Player Management

### Team Structure
Each team contains:
- **Name**: Unique identifier
- **Player List**: Linked list of active players
- **Player Count**: Current and maximum allowed players
- **Alive Players**: Track active participants

### Player Lifecycle
From [`game/player.c`](server/src/game/player.c):

1. **Connection**: Player joins team through egg hatching
2. **ID Assignment**: Unique ID generation across all teams
3. **Positioning**: Random spawn location assignment
4. **Inventory**: Initial resource allocation
5. **Status Tracking**: Level, direction, and alive state

## 🥚 Egg System

### Purpose
Eggs serve as spawn points for new players, distributed randomly across the map at server startup.

### Egg Lifecycle
From [`game/init_egg.c`](server/src/game/init_egg.c):

1. **Creation**: Generated during server initialization
2. **Positioning**: Random placement using shuffled coordinates
3. **Assignment**: Connected to players upon team joining
4. **Hatching**: Converts to active player
5. **Cleanup**: Removed after player spawning

## 🎮 Game Loop and Protocol

### Main Game Loop
From [`protocol.c`](server/src/protocol.c):

```c
int start_protocol(zappy_t *zappy)
{
    int tick_duration_ms = 1000 / zappy->params->freq;
    
    while (*get_running_state()) {
        // Poll for new connections
        if (zappy->network->pollserver.revents & POLLIN)
            accept_client(zappy);
            
        // Send GUI updates
        send_gui_message(zappy);
        
        // Process player actions
        smart_poll_players(zappy);
        
        // Handle GUI client requests
        poll_graphic_clients(zappy);
    }
}
```

### Timing System
- **Frequency-based ticks**: Actions execute based on server frequency
- **Action queuing**: Commands are queued and executed in priority order
- **Time-based execution**: Different actions have different execution times

## 🤖 AI Command Processing

### Command System
The server supports various AI commands:

- **Movement**: `Forward`, `Left`, `Right`
- **Resource Management**: `Take`, `Set`
- **Information**: `Look`, `Inventory`, `Connect_nbr`
- **Communication**: `Broadcast`
- **Advanced**: `Fork`, `Incantation`, `Eject`

### Command Execution Flow
From [`aiMessage/`](server/src/aiMessage/) modules:

1. **Command Reception**: Read from player socket
2. **Validation**: Check command format and parameters
3. **Queuing**: Add to player's action queue with priority
4. **Timing**: Execute based on action time requirements
5. **Response**: Send result back to AI client
6. **GUI Notification**: Broadcast changes to GUI clients

### Look Command Example
From [`aiMessage/look.c`](server/src/aiMessage/look.c):

```c
char *look_up(player_t *player, zappy_t *zappy)
{
    // Creates vision cone based on player level
    // Returns formatted string of visible tiles and contents
    // Handles wraparound coordinates
}
```

## 📊 GUI Communication

### Real-time Updates
The server continuously broadcasts game state to GUI clients:

- **Map State**: Tile contents and resource distribution
- **Player Updates**: Position, level, inventory changes
- **Egg Status**: Creation, hatching, and removal
- **Game Events**: Broadcasts, deaths, level-ups

### Message Types
From [`guiMessage/`](server/src/guiMessage/) modules:

- `msz`: Map size
- `bct`: Tile content
- `pnw`: New player
- `ppo`: Player position
- `plv`: Player level
- `pin`: Player inventory
- `enw`: New egg
- `ebo`: Egg hatching
- `edi`: Egg death

> 📖 **For detailed protocol specification**, see [Network Protocol](NetworkProtocol.md)

## 🔧 Memory Management

### Resource Cleanup
The server implements comprehensive memory management:

- **Structured Deallocation**: [`free.c`](server/src/free.c) handles all cleanup
- **Linked List Management**: Proper node removal and memory freeing
- **Error Handling**: Graceful cleanup on allocation failures

### Safety Measures
- **Null Pointer Checks**: Consistent validation before operations
- **Exit Handling**: Signal-based cleanup on server termination
- **Memory Leak Prevention**: Systematic resource tracking

## 🚨 Error Handling and Logging

### Message System
From [`messages.c`](server/src/messages.c):

```c
void error_message(char const *message)
{
    // Timestamped error logging with color coding
}

void valid_message(char const *message)
{
    // Success message logging
}
```

### Debug Mode
When `is_debug` is enabled:
- **Detailed Logging**: All GUI messages and operations
- **Map Visualization**: Tile contents and resource distribution
- **Connection Tracking**: Client join/leave notifications

## 🎯 Victory Conditions

The game ends when a team achieves:
- **6 players** at **maximum level** (level 8)
- **End game notification** sent to all clients
- **Server shutdown** or continued operation for new games

## 🔧 Configuration

### Server Parameters
```bash
./zappy_server -p <port> -x <width> -y <height> -n <team1> <team2> ... -c <nb_clients> -f <freq>
```

- **Port**: Network listening port
- **Dimensions**: Map width and height
- **Teams**: Team names for competition
- **Clients**: Maximum players per team
- **Frequency**: Game tick rate (actions per second)

## 🚀 Performance Considerations

### Scalability Features
- **Non-blocking I/O**: Polling-based network handling
- **Efficient Data Structures**: Linked lists for dynamic sizing
- **Memory Pool Management**: Structured allocation patterns
- **Event-driven Architecture**: Responsive to client actions

### Optimization Strategies
- **Resource Distribution Caching**: Pre-computed tile layouts
- **Command Prioritization**: Critical actions processed first
- **Batched GUI Updates**: Efficient state broadcasting
- **Connection Pooling**: Reusable network resources

The Zappy server provides a robust, scalable foundation for multiplayer strategic gameplay, handling complex game logic while maintaining real-time responsiveness for all connected clients.
