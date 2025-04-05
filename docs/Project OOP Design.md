# **CPP Project OOP Design**
## **Classes**

---

### **MenuManager**  
Handles main menu, game initialization, pausing, resuming, and quitting the game.

#### **Attributes**:
- `- gameManager`: GameManager   

#### **Methods**:
- `- showInstructionsAndKeys()`
- `+ openMenu()`
- `+ intializeGame()`


---

### **GameManager**  
Handles game flow, game state, and game loop

#### **Attributes**:
- `- isRunning`: bool  
- `- isPaused`: bool  
- `- gameObjects`: List<GameObject>
- `- winner`: Tank

#### **Methods**:
- `+ startGame()`
- `+ pauseGame()`
- `+ resumeGame()`
- `+ update()` 
- `+ gameOver()`


---

### **GameObject (Abstract Class)**  
Describes all attributes and methods a game object should implement  

#### **Attributes**:
- `- symbol`: char  
- `- isAlive`: bool  
- `- xPos`: int  
- `- yPos`: int  
- `- direction`: Direction    

#### **Methods**:
- `+ getSymbol() : char`  
- `+ setSymbol(symbol : char) : void`  
- `+ getXPosition() : int`  
- `+ setXPosition(x : int) : void`  
- `+ getYPosition() : int`  
- `+ setYPosition(y : int) : void`  
- `+ getDirection() : Direction`  
- `+ setDirection(direction : Direction) : void`  
- `+ isAlive() : bool`  
- `+ setState(state : bool) : void`
- `+ draw() : void`
- `+ collidesWith(GameObject object) : bool`

---

### **Tank** *(inherits GameObject, implements IMoveable, IDamageable)*  
Represents a tank in the game  

#### **Attributes**:
- `- cannon`: Cannon 
- `- name`: string

#### **Methods**:
- `+ fire() : void`  
- `+ move(Direction direction) : void` // Implementation from IMoveable  
- `+ takeDamage() : void` // Implementation from IDamageable  

---

### **Cannon** *(inherits GameObject)*  
Represents a cannon in the game  

#### **Attributes**:
- `- isReady`: bool  

#### **Methods**:
- `+ fire() : bool`  
- `+ getIsReady() : bool`  
- `+ setIsReady() : bool`  

---

### **Shell** *(inherits GameObject, implements IMoveable)*  
Represents a tank shell  

#### **Attributes**:
- `- direction`: Direction  

#### **Methods**:
- `+ move(Direction direction) : void` // Implementation from IMoveable  

---

### **Wall** *(inherits GameObject, implements IDamageable)*  
Represents a wall  

#### **Attributes**:
- `- health`: int  

#### **Methods**:
- `+ takeDamage() : void` // Implementation from IDamageable  
- `- destroyWall() : void` // Maybe should implement otherwise  
