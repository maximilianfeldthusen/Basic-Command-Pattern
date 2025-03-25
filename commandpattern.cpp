#include <iostream>
#include <vector>
#include <memory>

// Command Interface
class Command {
public:
    virtual ~Command() {}
    virtual void execute() = 0;
    virtual void undo() = 0;
};

// Receiver class
class Light {
public:
    void turnOn() {
        std::cout << "The light is ON." << std::endl;
    }

    void turnOff() {
        std::cout << "The light is OFF." << std::endl;
    }
};

// Concrete Command for turning on the light
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

// Concrete Command for turning off the light
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

// Invoker class
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

// Client code
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
