# **CPP Project OOP Design**
## **Classes**

---

### **GameManager**  
Handles main menu, game initialization, and pausing  
*Note for later - maybe should be implemented as a singleton to prevent looping when pausing and resuming.*

#### **Attributes**:
- `- isRunning`: bool  
- `- isPaused`: bool  

#### **Methods**:
- `+ startGame()`
- `+ pauseGame()`
- `+ resumeGame()`
- `+ update()`
- `+ render()` // Maybe can be united with update  
- `+ gameOver()` // Maybe useless  

---

### **GameObject (Abstract Class)**  
Describes all attributes and methods a game object should implement  

#### **Attributes**:
- `- symbol`: char  
- `- isAlive`: bool  
- `- xPos`: int  
- `- yPos`: int  
- `- xVel`: int  
- `- yVel`: int  

#### **Methods**:
- `+ getSymbol() : char`  
- `+ setSymbol(symbol : char) : void`  
- `+ getXPosition() : int`  
- `+ setXPosition(x : int) : void`  
- `+ getXVelocity() : int`  
- `+ setXVelocity(x : int) : void`  
- `+ getYPosition() : int`  
- `+ setYPosition(y : int) : void`  
- `+ getYVelocity() : int`  
- `+ setYVelocity(y : int) : void`  
- `+ isAlive() : bool`  
- `+ setState(state : bool) : void`  

---

### **Tank** *(inherits GameObject, implements IMoveable, IDamageable)*  
Represents a tank in the game  

#### **Attributes**:
- `- cannon`: Cannon  

#### **Methods**:
- `+ fire() : void`  
- `+ move() : void` // Implementation from IMoveable  
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

#### **Methods**:
- `+ move() : void` // Implementation from IMoveable  

---

### **Wall** *(inherits GameObject, implements IDamageable)*  
Represents a wall  

#### **Attributes**:
- `- health`: int  

#### **Methods**:
- `+ takeDamage() : void` // Implementation from IDamageable  
- `- destroyWall() : void` // Maybe should implement otherwise  
