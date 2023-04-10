#include "Booking.h"
#include <sstream>

Booking::Booking(const string& roomCode, const string& userName, const string& userID, time_t startTime, time_t endTime, const string& reason)
    : roomCode(roomCode), userName(userName), userID(userID), startTime(startTime), endTime(endTime), reason(reason) {}
// Getters
string Booking::getUserName() const {
    return userName;
}

string Booking::getUserID() const {
    return userID;
}

time_t Booking::getStartTime() const {
    return startTime;
}

time_t Booking::getEndTime() const {
    return endTime;
}

string Booking::getReason() const {
    return reason;
}

// Setters
void Booking::setUserName(const string& userName) {
    this->userName = userName;
}

void Booking::setUserID(const string& userID) {
    this->userID = userID;
}

void Booking::setStartTime(time_t startTime) {
    this->startTime = startTime;
}

void Booking::setEndTime(time_t endTime) {
    this->endTime = endTime;
}

void Booking::setReason(const string& reason) {
    this->reason = reason;
}

string Booking::toCSV() const {
    stringstream ss;
    ss << roomCode << "," << userName << "," << userID << ","
        << startTime << "," << endTime << "," << reason;
    return ss.str();
}

