# The Zappy - AI

## 🎯 Overview

The Zappy AI client is an intelligent automated player system that connects to the Zappy server to compete in the strategic multiplayer game. Built in Python, it implements advanced gameplay strategies including resource management, team coordination, and automatic player spawning to achieve the victory condition of elevating 6 players to maximum level.

## 🏗️ Architecture Overview

The AI client follows a modular, multi-process architecture:

```
📦 Zappy AI
├── 🚀 App Management    - Process orchestration and lifecycle
├── 🤖 Player Intelligence - Game logic and decision making  
├── 🌐 Communication     - Server protocol and messaging
├── 📡 Broadcasting      - Team coordination and messaging
├── 🔐 Hash System       - Secure team communication
├── 🖥️  CLI Parser       - Command line argument handling
└── 📊 Logger System     - Comprehensive logging and debugging
```

## 🚀 Entry Point and Application Management

### Main Entry Point
The AI starts from [`main.py`](ai/src/main.py):

```python
def main():
    logger = Logger()
    
    try:
        cli = CLI()
        config = cli.parse_args(argv)
        app = App(config)
        return app.run()
    except CLIParsingException as e:
        logger.error(f"CLI Parsing Error: {e}")
        return FAILURE
```

### Application Orchestration
From [`App.py`](ai/src/App/App.py):

The App class manages the entire AI team lifecycle:

```python
class App:
    def __init__(self, config: dict[str]):
        self.port = config["port"]
        self.name = config["name"]
        self.ip = config["machine"]
        self.childs: list[int] = []
        self.running = True
        self.is_main_process = True
```

#### Multi-Process Architecture
The AI uses a fork-based multi-process system:

1. **Main Process**: Coordinates team and spawns child processes
2. **Child Processes**: Individual AI players with independent logic
3. **Signal Handling**: Graceful shutdown and process cleanup

```python
def create_new_player(self) -> int:
    pid: int = os.fork()
    if pid == 0:  # Child process
        p = Player(self.name, self.ip, self.port)
        p.is_child_process = True
        p.startComThread()
        p.loop()
    return pid  # Parent receives child PID
```

## 🤖 Player Intelligence System

### Player Architecture
From [`Player.py`](ai/src/Player/Player.py):

Each AI player is an autonomous agent with:

```python
class Player:
    def __init__(self, name: str, ip: str, port: int = 4242):
        self.communication = Communication(name, ip, port)
        self.level = 1
        self.inventory = {
            "food": 10, "linemate": 0, "deraumere": 0,
            "sibur": 0, "mendiane": 0, "phiras": 0, "thystame": 0
        }
        self.roombaState = {
            "forwardCount": 0, "targetForward": 10,
            "phase": "forward", "lastCommand": None
        }
```

### Level Progression System
The AI implements a sophisticated leveling system based on resource requirements:

```python
LVL_UPGRADES = {
    1: {"linemate": 1, "deraumere": 1, "sibur": 0, "mendiane": 0, "phiras": 0, "thystame": 0},
    2: {"linemate": 1, "deraumere": 1, "sibur": 1, "mendiane": 0, "phiras": 0, "thystame": 0},
    3: {"linemate": 2, "deraumere": 0, "sibur": 1, "mendiane": 0, "phiras": 2, "thystame": 0},
    4: {"linemate": 1, "deraumere": 1, "sibur": 2, "mendiane": 0, "phiras": 1, "thystame": 0},
    5: {"linemate": 1, "deraumere": 2, "sibur": 1, "mendiane": 3, "phiras": 0, "thystame": 0},
    6: {"linemate": 1, "deraumere": 2, "sibur": 3, "mendiane": 0, "phiras": 1, "thystame": 0},
    7: {"linemate": 2, "deraumere": 2, "sibur": 2, "mendiane": 2, "phiras": 2, "thystame": 1}
}
```

### Intelligent Behavior: Roomba Strategy
The AI implements a "Roomba" exploration strategy for efficient resource collection:

```python
def roombaAction(self) -> None:
    if self.roombaState["phase"] == "forward":
        # Look for resources on current tile
        self.communication.sendLook()
        
        # Collect food and needed stones
        if "food" in self.look[0].keys():
            self.communication.sendTakeObject("food")
        
        neededStones = self.getNeededStonesByPriority()
        for stone in neededStones:
            if stone in self.look[0].keys():
                self.communication.sendTakeObject(stone)
        
        # Move forward or turn
        if self.roombaState["forwardCount"] < self.roombaState["targetForward"]:
            self.communication.sendForward()
        else:
            self.roombaState["phase"] = "turn"
```

### Resource Prioritization
The AI prioritizes resources based on current level requirements:

```python
def getNeededStonesByPriority(self) -> list[str]:
    needed = LVL_UPGRADES.get(self.level, {})
    priority_stones = []
    
    for stone, required in needed.items():
        if self.inventory.get(stone, 0) < required:
            priority_stones.append(stone)
    
    return priority_stones
```

## 🌐 Communication System

### Network Protocol Handler
From [`Communication.py`](ai/src/Communication/Communication.py):

The communication system handles all server interactions:

```python
class Communication:
    def __init__(self, name: str, ip: str, port: int):
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.name = name
        self.ip = ip
        self.port = port
        self.responseQueue = queue.Queue()
```

### Command System
The AI supports all server commands:

- **Movement Commands**: `sendForward()`, `sendLeft()`, `sendRight()`
- **Resource Commands**: `sendTakeObject(obj)`, `sendSetObject(obj)`
- **Information Commands**: `sendLook()`, `sendInventory()`, `sendConnectNbr()`
- **Advanced Commands**: `sendIncantation()`, `sendFork()`, `sendEject()`
- **Communication**: `sendBroadcast(message)`

### Response Handling
The AI processes server responses with a command-response mapping system:

```python
def handleCommandResponse(self, response: str) -> None:
    switcher = {
        "inventory": self.handleResponseInventory,
        "look": self.handleResponseLook,
        "ko": self.handleResponseKO,
        "ok": self.handleResponseOK,
    }
    handler = switcher.get(response.strip(), None)
    if handler:
        handler()
```

## 📡 Broadcasting and Team Coordination

### Secure Team Communication
From [`Broadcaster.py`](ai/src/Broadcaster/Broadcaster.py):

The broadcasting system enables secure team coordination:

```python
class Broadcaster:
    def __init__(self, com: Communication, team: str):
        self.com = com
        self.hash = Hash(team)
        self.lastIndex = 0
    
    def revealMessage(self, message: str) -> str:
        data = self.hash.unHashMessage(message.strip())
        if "/" not in data:
            return ""
        
        message, index = data.split("/")
        index = int(index.strip())
        
        if index <= self.lastIndex:
            return ""  # Ignore old messages
        
        self.lastIndex = index
        return message.strip()
```

### Message Encryption
From [`Hash.py`](ai/src/Hash/Hash.py):

Team messages are encrypted to prevent enemy teams from intercepting communications:

- **Team-specific encryption**: Each team has unique hash algorithms
- **Message indexing**: Prevents replay attacks and ensures message ordering
- **Secure broadcasting**: Only team members can decrypt messages

## 🖥️ Command Line Interface

### Argument Parsing
From [`CLI.py`](ai/src/CLI/CLI.py):

The CLI system handles command line arguments with comprehensive validation:

```python
class CLI:
    def parse_args(self, args):
        config = {"port": None, "name": None, "machine": "127.0.0.1"}
        
        for i in range(1, len(args)):
            if args[i] == "-p" and i + 1 < len(args):
                config["port"] = self.parse_port(args[i + 1])
            elif args[i] == "-n" and i + 1 < len(args):
                config["name"] = self.parse_name(args[i + 1])
            elif args[i] == "-h" and i + 1 < len(args):
                config["machine"] = self.parse_machine(args[i + 1])
        
        return config
```

### Input Validation
The CLI performs comprehensive input validation:

- **Port validation**: Range checking (1-65535)
- **Team name validation**: Non-empty string validation
- **Hostname validation**: IP address and hostname resolution
- **Required parameter checking**: Ensures all mandatory parameters are provided

## 📊 Logging and Debugging

### Comprehensive Logging System
From [`Logger.py`](ai/src/Logger/Logger.py):

The AI includes a sophisticated logging system with:

- **Colored output**: Different colors for different log levels
- **Timestamping**: Precise timing information for debugging
- **Multi-level logging**: Success, error, info, warning, and debug messages
- **Process identification**: Distinguishes between main and child process logs

## 🎮 Game Loop and Strategy

### Main Game Loop
Each AI player runs an continuous game loop:

```python
def loop(self) -> None:
    while True:
        # Handle server responses
        response = self.communication.getResponse()
        if response:
            self.handleCommandResponse(response)
        
        # Check for death condition
        if self.inventory.get("food", 0) <= 0:
            raise PlayerDead("Player died from starvation")
        
        # Execute survival and advancement strategies
        self.dropStonesForSurvival()
        
        # Execute roomba exploration behavior
        if not self.inIncantation:
            self.roombaAction()
```

### Survival Strategies
The AI implements intelligent survival mechanisms:

```python
def dropStonesForSurvival(self) -> None:
    if self.inventory.get("food", 0) < 5:  # Low food threshold
        # Drop less critical stones to make room for food
        for stone in ["thystame", "phiras", "mendiane"]:
            if self.inventory.get(stone, 0) > 0:
                self.communication.sendSetObject(stone)
                break
```

## 🚀 Advanced Features

### Multi-Process Management
The AI system manages multiple processes efficiently:

- **Process spawning**: Creates child processes for each available slot
- **Signal handling**: Graceful shutdown on interruption
- **Resource cleanup**: Proper memory and process management
- **Process monitoring**: Tracks child process status

### Error Handling and Recovery
Comprehensive error handling ensures robustness:

- **Network errors**: Automatic reconnection attempts
- **Server disconnection**: Graceful handling of connection loss
- **Resource exhaustion**: Intelligent resource management
- **Command failures**: Fallback strategies for failed actions

### Performance Optimization
The AI is optimized for performance:

- **Threaded communication**: Separate thread for network I/O
- **Efficient pathfinding**: Optimized movement strategies
- **Resource caching**: Cached information about map and resources
- **Priority-based actions**: Critical actions take precedence

## 🔧 Configuration and Usage

### Running the AI
```bash
./zappy_ai -p <port> -n <team_name> -h <hostname>
```

### Parameters
- **`-p port`**: Server port number
- **`-n team_name`**: Team name for identification
- **`-h hostname`**: Server hostname (optional, defaults to localhost)

### Team Coordination
Multiple AI instances can run for the same team:
- Each instance spawns multiple players based on server slots
- Players coordinate through encrypted broadcasting
- Shared strategy for optimal resource distribution

## 🎯 Victory Strategy

The AI's ultimate goal is achieving victory through:

1. **Resource Collection**: Efficient gathering of required stones
2. **Level Progression**: Strategic incantations to advance levels
3. **Team Coordination**: Synchronized efforts for maximum efficiency
4. **Survival Management**: Maintaining food supply for all players
5. **Player Spawning**: Creating new players through forking
6. **Victory Condition**: Getting 6 players to maximum level (8)

## 🧪 Testing and Quality Assurance

### Unit Testing
From [`tests/unit/ai/`](tests/unit/ai/):

The AI includes comprehensive unit tests:
- **CLI parsing tests**: Validation of command line argument handling
- **Communication tests**: Network protocol testing
- **Player behavior tests**: Strategy and decision-making validation
- **Hash system tests**: Encryption and decryption verification

### Test Coverage
- **Pytest integration**: Modern Python testing framework
- **Coverage reporting**: Detailed code coverage analysis
- **Continuous integration**: Automated testing pipeline

The Zappy AI client provides an intelligent, robust foundation for automated gameplay, implementing sophisticated strategies while maintaining clean, modular code architecture that can be extended and customized for different gameplay approaches.
