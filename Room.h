#ifndef ZOORK_ROOM_H
#define ZOORK_ROOM_H

#include "Character.h"
#include "RoomDefaultEnterCommand.h"
#include "Item.h"
#include "Location.h"
#include <map>
#include <vector>
#include <memory>

class Passage;

class Room : public Location {
public:
    Room(const std::string &, const std::string &);
    Room(const std::string &, const std::string &, std::shared_ptr<Command>);

    void addItem(std::shared_ptr<Item>);
    void removeItem(const std::string&);
    std::shared_ptr<Item> getItem(const std::string&);
    std::shared_ptr<Item> retrieveItem(const std::string&);
    std::vector<std::shared_ptr<Item>>& getItems();

    void addPassage(const std::string &, std::shared_ptr<Passage>);
    void removePassage(const std::string &);
    std::shared_ptr<Passage> getPassage(const std::string &);

protected:
    std::vector<std::shared_ptr<Item>> items;
    std::map<std::string, std::shared_ptr<Passage>> passageMap;
};

#endif //ZOORK_ROOM_H