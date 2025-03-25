
## Documentation

### Basic-Command-Pattern

This code implements the Command design pattern, which is a behavioral design pattern that turns a request into a stand-alone object that contains all information about the request. This allows for parameterization of clients with queues, requests, and operations, as well as the ability to support undoable operations.

Here's a breakdown of the code:

### 1. Includes and Class Definitions

```cpp
#include <iostream>
#include <vector>
#include <memory>
```
- The code includes necessary headers: `<iostream>` for standard input/output, `<vector>` for using dynamic arrays, and `<memory>` for smart pointers to manage memory automatically.

### 2. Command Interface

```cpp
class Command {
public:
    virtual ~Command() {}
    virtual void execute() = 0;
    virtual void undo() = 0;
};
```
- The `Command` class is an abstract base class that declares two pure virtual functions: `execute()` and `undo()`. Any concrete command must implement these methods.

### 3. Receiver Class

```cpp
class Light {
public:
    void turnOn() {
        std::cout << "The light is ON." << std::endl;
    }

    void turnOff() {
        std::cout << "The light is OFF." << std::endl;
    }
};
```
- The `Light` class represents the receiver of the commands. It has methods to turn the light on and off.

### 4. Concrete Command Classes

```cpp
class LightOnCommand : public Command {
private:
    Light& light;

public:
    LightOnCommand(Light& l) : light(l) {}

    void execute() override {
        light.turnOn();
    }

    void undo() override {
        light.turnOff();
    }
};

class LightOffCommand : public Command {
private:
    Light& light;

public:
    LightOffCommand(Light& l) : light(l) {}

    void execute() override {
        light.turnOff();
    }

    void undo() override {
        light.turnOn();
    }
};
```
- `LightOnCommand` and `LightOffCommand` are concrete implementations of the `Command` interface.
- Each command class holds a reference to a `Light` object (the receiver) and implements the `execute()` and `undo()` methods to perform the respective actions.

### 5. Invoker Class

```cpp
class RemoteControl {
private:
    std::unique_ptr<Command> command;
    std::vector<std::unique_ptr<Command>> commandHistory;

public:
    void setCommand(std::unique_ptr<Command> cmd) {
        command = std::move(cmd);
    }

    void pressButton() {
        if (command) {
            command->execute();
            commandHistory.push_back(std::move(command)); // Store the command for undo
        }
    }

    void undoButton() {
        if (!commandHistory.empty()) {
            commandHistory.back()->undo();
            commandHistory.pop_back(); // Remove the last command from history
        }
    }
};
```
- The `RemoteControl` class acts as the invoker. It can set a command and execute it when a button is pressed.
- The `commandHistory` vector keeps track of executed commands to support undoing the last action.
- The `setCommand()` method takes a unique pointer to a command, and `pressButton()` executes the command and stores it in history.
- The `undoButton()` method undoes the last executed command by calling its `undo()` method and then removes it from the history.

### 6. Client Code

```cpp
int main() {
    Light livingRoomLight;

    RemoteControl remote;
    
    // Turn on the light
    remote.setCommand(std::make_unique<LightOnCommand>(livingRoomLight));
    remote.pressButton();
    
    // Turn off the light
    remote.setCommand(std::make_unique<LightOffCommand>(livingRoomLight));
    remote.pressButton();
    
    // Undo the last command (turn off the light)
    remote.undoButton(); // This will turn the light back on

    return 0;
}
```
- In the `main()` function, the client code creates an instance of `Light` and a `RemoteControl`.
- It uses the remote to set commands to turn the light on and off.
- After turning the light off, it calls `undoButton()` to turn the light back on, demonstrating the undo functionality.

### Summary
This code demonstrates how the Command pattern can be used to encapsulate requests as objects, allowing for flexible command execution and easy implementation of features like undo functionality. The separation of concerns between the invoker, command, and receiver promotes cleaner and more maintainable code.
