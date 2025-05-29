#include "ZOOrkEngine.h"
#include <utility>

ZOOrkEngine::ZOOrkEngine(std::shared_ptr<Room> start) {
    setupGame(start);
    player = Player::instance();
    player->setCurrentRoom(start.get());
    player->getCurrentRoom()->enter();
}

void ZOOrkEngine::setupGame(std::shared_ptr<Room> start) {
    // Create items for various rooms
    auto lantern = std::make_shared<Item>("lantern", "A brass lantern that provides light in dark places.");
    auto sword = std::make_shared<Item>("sword", "A gleaming sword with ancient runes on its blade.");
    auto key = std::make_shared<Item>("key", "A rusty iron key that might unlock something important.");
    
    // Create additional rooms
    auto kitchen = std::make_shared<Room>("kitchen", 
        "You are in a rustic kitchen. The smell of old meals lingers. A wooden table stands in the center.");
    auto cellar = std::make_shared<Room>("cellar", 
        "A dank cellar with shelves of dusty wine bottles. The air is musty and cool.");
    auto garden = std::make_shared<Room>("garden", 
        "A beautiful garden with blooming flowers and a small fountain.");
    auto library = std::make_shared<Room>("library", 
        "Walls lined with ancient books. The air is thick with the smell of old leather and parchment.");
    auto attic = std::make_shared<Room>("attic", 
        "A dusty attic filled with old furniture and cobwebs. Light streams through a small window.");
    auto dining_room = std::make_shared<Room>("dining-room", 
        "A grand dining room with a long table. Crystal chandeliers hang from the ceiling.");
    auto study = std::make_shared<Room>("study", 
        "A cozy study with a desk and comfortable armchair. Papers are scattered about.");
    auto bedroom = std::make_shared<Room>("bedroom", 
        "A large bedroom with a four-poster bed. Heavy curtains cover the windows.");
    auto bathroom = std::make_shared<Room>("bathroom", 
        "A vintage bathroom with a clawfoot tub. The mirror is slightly tarnished.");
    auto hallway = std::make_shared<Room>("hallway", 
        "A long hallway with portraits on the walls. Their eyes seem to follow you.");

    // Add items to rooms
    kitchen->addItem(lantern);
    library->addItem(sword);
    study->addItem(key);

    // Connect rooms
    Passage::createBasicPassage(start.get(), kitchen.get(), "north", true);
    Passage::createBasicPassage(kitchen.get(), cellar.get(), "down", true);
    Passage::createBasicPassage(start.get(), garden.get(), "west", true);
    Passage::createBasicPassage(start.get(), hallway.get(), "east", true);
    Passage::createBasicPassage(hallway.get(), library.get(), "north", true);
    Passage::createBasicPassage(hallway.get(), study.get(), "east", true);
    Passage::createBasicPassage(library.get(), attic.get(), "up", true);
    Passage::createBasicPassage(hallway.get(), dining_room.get(), "south", true);
    Passage::createBasicPassage(dining_room.get(), kitchen.get(), "west", true);
    Passage::createBasicPassage(hallway.get(), bedroom.get(), "northeast", true);
    Passage::createBasicPassage(bedroom.get(), bathroom.get(), "east", true);
}

void ZOOrkEngine::run() {
    while (!gameOver) {
        std::cout << "> ";

        std::string input;
        std::getline(std::cin, input);

        std::vector<std::string> words = tokenizeString(input);
        if (words.empty()) continue;

        std::string command = words[0];
        std::vector<std::string> arguments(words.begin() + 1, words.end());

        if (command == "go") {
            handleGoCommand(arguments);
        } else if ((command == "look") || (command == "inspect")) {
            handleLookCommand(arguments);
        } else if ((command == "take") || (command == "get")) {
            handleTakeCommand(arguments);
        } else if (command == "drop") {
            handleDropCommand(arguments);
        } else if (command == "inventory" || command == "i") {
            handleInventoryCommand();
        } else if (command == "quit") {
            handleQuitCommand(arguments);
        } else {
            std::cout << "I don't understand that command.\n";
        }
    }
}

void ZOOrkEngine::handleGoCommand(std::vector<std::string> arguments) {
    if (arguments.empty()) {
        std::cout << "Go where?\n";
        return;
    }

    std::string direction;
    if (arguments[0] == "n" || arguments[0] == "north") {
        direction = "north";
    } else if (arguments[0] == "s" || arguments[0] == "south") {
        direction = "south";
    } else if (arguments[0] == "e" || arguments[0] == "east") {
        direction = "east";
    } else if (arguments[0] == "w" || arguments[0] == "west") {
        direction = "west";
    } else if (arguments[0] == "u" || arguments[0] == "up") {
        direction = "up";
    } else if (arguments[0] == "d" || arguments[0] == "down") {
        direction = "down";
    } else {
        direction = arguments[0];
    }

    Room* currentRoom = player->getCurrentRoom();
    auto passage = currentRoom->getPassage(direction);
    player->setCurrentRoom(passage->getTo());
    passage->enter();
}

void ZOOrkEngine::handleLookCommand(std::vector<std::string> arguments) {
    if (arguments.empty()) {
        // Look at the current room
        std::cout << player->getCurrentRoom()->getDescription() << "\n";
        
        // List items in the room
        auto& items = player->getCurrentRoom()->getItems();
        if (!items.empty()) {
            std::cout << "You see: ";
            for (size_t i = 0; i < items.size(); ++i) {
                std::cout << items[i]->getName();
                if (i < items.size() - 1) std::cout << ", ";
            }
            std::cout << "\n";
        }
        return;
    }

    // Look at a specific item
    std::string itemName = arguments[0];
    auto roomItem = player->getCurrentRoom()->getItem(itemName);
    auto inventoryItem = player->getFromInventory(itemName);

    if (roomItem) {
        std::cout << roomItem->getDescription() << "\n";
    } else if (inventoryItem) {
        std::cout << inventoryItem->getDescription() << "\n";
    } else {
        std::cout << "You don't see that here.\n";
    }
}

void ZOOrkEngine::handleTakeCommand(std::vector<std::string> arguments) {
    if (arguments.empty()) {
        std::cout << "Take what?\n";
        return;
    }

    std::string itemName = arguments[0];
    auto item = player->getCurrentRoom()->retrieveItem(itemName);
    
    if (item) {
        player->addToInventory(item);
        std::cout << "Taken.\n";
    } else {
        std::cout << "You don't see that here.\n";
    }
}

void ZOOrkEngine::handleDropCommand(std::vector<std::string> arguments) {
    if (arguments.empty()) {
        std::cout << "Drop what?\n";
        return;
    }

    std::string itemName = arguments[0];
    auto item = player->retrieveFromInventory(itemName);
    
    if (item) {
        player->getCurrentRoom()->addItem(item);
        std::cout << "Dropped.\n";
    } else {
        std::cout << "You don't have that.\n";
    }
}

void ZOOrkEngine::handleInventoryCommand() {
    const auto& inventory = player->getInventory();
    if (inventory.empty()) {
        std::cout << "You are empty-handed.\n";
        return;
    }

    std::cout << "You are carrying:\n";
    for (const auto& item : inventory) {
        std::cout << "  " << item->getName() << "\n";
    }
}

void ZOOrkEngine::handleQuitCommand(std::vector<std::string> arguments) {
    std::string input;
    std::cout << "Are you sure you want to QUIT?\n> ";
    std::getline(std::cin, input);
    std::string quitStr = makeLowercase(input);

    if (quitStr == "y" || quitStr == "yes") {
        gameOver = true;
    }
}

std::vector<std::string> ZOOrkEngine::tokenizeString(const std::string &input) {
    std::vector<std::string> tokens;
    std::stringstream ss(input);
    std::string token;

    while (std::getline(ss, token, ' ')) {
        if (!token.empty()) {
            tokens.push_back(makeLowercase(token));
        }
    }

    return tokens;
}

std::string ZOOrkEngine::makeLowercase(std::string input) {
    std::string output = std::move(input);
    std::transform(output.begin(), output.end(), output.begin(), ::tolower);
    return output;
}