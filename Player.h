#ifndef ZOORK_PLAYER_H
#define ZOORK_PLAYER_H

#include "Character.h"
#include "Location.h"
#include "NullRoom.h"
#include "Item.h"
#include <vector>
#include <memory>

class Player : public Character {
public:
    static Player *instance() {
        if (!playerInstance) {
            playerInstance = new Player();
        }
        return Player::playerInstance;
    }

    void setCurrentRoom(Room*);
    Room* getCurrentRoom() const;
    
    void addToInventory(std::shared_ptr<Item>);
    void removeFromInventory(const std::string&);
    std::shared_ptr<Item> getFromInventory(const std::string&);
    std::shared_ptr<Item> retrieveFromInventory(const std::string&);
    const std::vector<std::shared_ptr<Item>>& getInventory() const;

    Player(const Player &) = delete;
    Player &operator=(const Player &) = delete;

private:
    static Player *playerInstance;
    Room* currentRoom;
    std::vector<std::shared_ptr<Item>> inventory;

    Player() : Character("You", "You are a person, alike in dignity to any other, but uniquely you."),
               currentRoom(new NullRoom()) {}
};

#endif //ZOORK_PLAYER_H