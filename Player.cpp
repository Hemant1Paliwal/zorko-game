#include "Player.h"
#include <algorithm>

Player *Player::playerInstance = nullptr;

void Player::setCurrentRoom(Room* room) {
    currentRoom = room;
}

Room* Player::getCurrentRoom() const {
    return currentRoom;
}

void Player::addToInventory(std::shared_ptr<Item> item) {
    inventory.push_back(item);
}

void Player::removeFromInventory(const std::string& itemName) {
    inventory.erase(
        std::remove_if(
            inventory.begin(), 
            inventory.end(),
            [&itemName](const std::shared_ptr<Item>& item) { 
                return item->getName() == itemName; 
            }
        ),
        inventory.end()
    );
}

std::shared_ptr<Item> Player::getFromInventory(const std::string& itemName) {
    auto it = std::find_if(
        inventory.begin(), 
        inventory.end(),
        [&itemName](const std::shared_ptr<Item>& item) { 
            return item->getName() == itemName; 
        }
    );
    
    if (it != inventory.end()) {
        return *it;
    }
    return nullptr;
}

std::shared_ptr<Item> Player::retrieveFromInventory(const std::string& itemName) {
    auto item = getFromInventory(itemName);
    if (item) {
        removeFromInventory(itemName);
        return item;
    }
    return nullptr;
}

const std::vector<std::shared_ptr<Item>>& Player::getInventory() const {
    return inventory;
}