#include "Room.h"

Room::Room(string code, string name, string location, int capacity, int examCapacity,
    string specification, vector<string> equipment)
    : code(code), name(name), location(location), capacity(capacity), examCapacity(examCapacity),
    specification(specification), equipment(equipment) {}

// Getters
string Room::getCode() const {
    return code;
}

string Room::getName() const {
    return name;
}

string Room::getLocation() const {
    return location;
}

int Room::getCapacity() const {
    return capacity;
}

int Room::getExamCapacity() const {
    return examCapacity;
}

string Room::getSpecification() const {
    return specification;
}

vector<string> Room::getEquipment() const {
    return equipment;
}

// Setters
void Room::setCode(const string& code) {
    this->code = code;
}

void Room::setName(const string& name) {
    this->name = name;
}

void Room::setLocation(const string& location) {
    this->location = location;
}

void Room::setCapacity(int capacity) {
    this->capacity = capacity;
}

void Room::setExamCapacity(int examCapacity) {
    this->examCapacity = examCapacity;
}

void Room::setSpecification(const string& specification) {
    this->specification = specification;
}

void Room::setEquipment(const vector<string>& equipment) {
    this->equipment = equipment;
}
