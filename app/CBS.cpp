#include <iostream>
#include <iomanip>
#include <ctime>
#include <fstream>
#include <sstream>
#include "CBS.h"
#include <set>
#include <vector>
CBS::CBS() {
    loadRooms();
    loadBookings();
}

#include <algorithm>

int strToInt(const string& str) {
    if (str == "N/A") {
        return 0;
    }
    try {
        return stoi(str);
    }
    catch (const std::invalid_argument& e) {
        return 0;
    }
}

void CBS::loadRooms() {
    ifstream file("room_db.csv");

    if (!file.is_open()) {
        cerr << "Error opening room_db.csv" << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        vector<string> tokens;
        string token;

        bool inside_quotes = false;
        string quoted_token;
        while (getline(ss, token, ',')) {
            // Check if the token starts with a double quote
            if (!token.empty() && token.front() == '"' && !inside_quotes) {
                inside_quotes = true;
                quoted_token = token.substr(1);
            }
            else if (inside_quotes) {
                // If inside quotes, append the token to the quoted_token
                quoted_token += ',' + token;
                if (!token.empty() && token.back() == '"') {
                    // If the token ends with a double quote, we reached the end of the quoted_token
                    inside_quotes = false;
                    quoted_token.pop_back();
                    tokens.push_back(quoted_token);
                }
            }
            else {
                tokens.push_back(token);
            }
        }

        if (tokens.size() < 7) {
            cerr << "Invalid line format in room_db.csv" << endl;
            continue;
        }

        string code = tokens[0];
        string name = tokens[1];
        string location = tokens[2];
        int capacity = strToInt(tokens[3]);
        int examCapacity = strToInt(tokens[4]);
        string specification = tokens[5];

        vector<string> equipment;
        stringstream equipmentStream(tokens[6]);
        string equipmentItem;
        while (getline(equipmentStream, equipmentItem, ';')) {
            equipment.push_back(equipmentItem);
        }

        rooms.emplace_back(code, name, location, capacity, examCapacity, specification, equipment);
    }

    file.close();

    // Initialize roomCalendars vector with empty Calendar objects
    roomCalendars.resize(rooms.size());
}

void CBS::showBookingState(const string& roomCode, time_t startDate, time_t endDate) {
    int roomIndex = -1;

    // Find the index of the room with the given room code
    for (size_t i = 0; i < rooms.size(); ++i) {
        if (rooms[i].getCode() == roomCode) {
            roomIndex = i;
            break;
        }
    }

    if (roomIndex == -1) {
        cout << "Room not found." << endl;
        return;
    }

    // Check if roomIndex is a valid index for roomCalendars
    if (roomIndex >= roomCalendars.size()) {
        cout << "Error: room index out of range." << endl;
        return;
    }

    // Get the bookings for the specified date range
    vector<Booking> bookings = roomCalendars[roomIndex].getBookings(startDate, endDate);


    if (bookings.empty()) {
        cout << "No bookings found for the specified date range." << endl;
        return;
    }

    // Before the for loop in showBookingState()
    struct tm startTm, endTm;
    localtime_s(&startTm, &startDate);
    localtime_s(&endTm, &endDate);

    cout << "Booking state for room " << roomCode << " from " << std::put_time(&startTm, "%Y-%m-%d %H:%M:%S") << " to " << std::put_time(&endTm, "%Y-%m-%d %H:%M:%S") << endl;


    // Print booking details
    for (const Booking& booking : bookings) {
        cout << "Booking: " << endl;
        cout << "\tUser Name: " << booking.getUserName() << endl;
        cout << "\tUser ID: " << booking.getUserID() << endl;
        time_t startTime = booking.getStartTime();
        time_t endTime = booking.getEndTime();
        struct tm startTm, endTm;
        localtime_s(&startTm, &startTime);
        localtime_s(&endTm, &endTime);
        cout << "\tStart Time: " << std::put_time(&startTm, "%Y-%m-%d %H:%M:%S") << endl;
        cout << "\tEnd Time: " << std::put_time(&endTm, "%Y-%m-%d %H:%M:%S") << endl;
        cout << "\tReason: " << booking.getReason() << endl;
        cout << endl;
    }




}

void CBS::displayAllRooms() {
    cout << "All Rooms:" << endl;
    for (const Room& room : rooms) {
        cout << "Room Code: " << room.getCode() << ", Room Name: " << room.getName()
            << ", Location: " << room.getLocation() << ", Capacity: " << room.getCapacity()
            << ", Exam Capacity: " << room.getExamCapacity() << ", Specification: " << room.getSpecification()
            << ", Equipment: ";

        // Output the equipment list
        const vector<string>& equipment = room.getEquipment();
        for (size_t i = 0; i < equipment.size(); ++i) {
            cout << equipment[i];
            if (i < equipment.size() - 1) {
                cout << ", ";
            }
        }

        cout << endl;
    }
}

void CBS::saveBookings() {
    ofstream file("bookings.csv");

    if (!file.is_open()) {
        cerr << "Error opening bookings.csv" << endl;
        return;
    }

    for (const Calendar& calendar : roomCalendars) {
        for (const Booking& booking : calendar.getBookings()) {
            file << booking.toCSV() << endl;
        }
    }


    file.close();
}

bool CBS::bookRoom(const string& userName, const string& userID, const string& roomCode, time_t startTime,
    time_t endTime, const string& reason) {
    int roomIndex = -1;

    // Find the index of the room with the given room code
    for (size_t i = 0; i < rooms.size(); ++i) {
        if (rooms[i].getCode() == roomCode) {
            roomIndex = i;
            break;
        }
    }

    if (roomIndex == -1) {
        cout << "Room not found." << endl;
        return false;
    }

    Calendar& calendar = roomCalendars[roomIndex];

    // Check if the time slot is available for booking
    if (calendar.isTimeSlotAvailable(startTime, endTime)) {
        // Create a new booking and add it to the calendar
        Booking booking(roomCode, userName, userID, startTime, endTime, reason);
        calendar.addBooking(booking);

        // Save bookings to file
        saveBookings();

        return true;
    }
    else {
        cout << "The room is not available during the requested time slot." << endl;
        return false;
    }
}


void CBS::loadBookings() {
    ifstream file("bookings.csv");

    if (!file.is_open()) {
        cerr << "Error opening bookings.csv" << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        vector<string> tokens;
        string token;

        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 6) {
            cerr << "Invalid line format in bookings.csv" << endl;
            continue;
        }

        string roomCode = tokens[0];
        string userName = tokens[1];
        string userID = tokens[2];
        time_t startTime = static_cast<time_t>(stoll(tokens[3]));
        time_t endTime = static_cast<time_t>(stoll(tokens[4]));
        string reason = tokens[5];

        int roomIndex = -1;
        for (size_t i = 0; i < rooms.size(); ++i) {
            if (rooms[i].getCode() == roomCode) {
                roomIndex = i;
                break;
            }
        }

        if (roomIndex == -1) {
            cerr << "Room not found in bookings.csv" << endl;
            continue;
        }

        Booking booking(roomCode, userName, userID, startTime, endTime, reason);
        roomCalendars[roomIndex].addBooking(booking);
    }

    file.close();
}


vector<string> splitEquipment(const string& equipmentStr) {
    vector<string> splitEquipment;
    istringstream ss(equipmentStr);
    string item;
    while (getline(ss, item, ',')) {
        splitEquipment.push_back(item);
    }
    return splitEquipment;
}

vector<Room> CBS::findFreeRoom(time_t startTime, time_t endTime, int desiredCapacity, const string& purpose, const vector<string>& requestedEquipment) {
    vector<Room> freeRooms;

    // Find the rooms that are not available during the requested time slot
    vector<string> bookedRoomCodes;
    for (size_t i = 0; i < rooms.size(); ++i) {
        Calendar& calendar = roomCalendars[i];
        if (!calendar.isTimeSlotAvailable(startTime, endTime)) {
            bookedRoomCodes.push_back(rooms[i].getCode());
        }
    }

    // Check the availability of each free room
    for (size_t i = 0; i < rooms.size(); ++i) {
        const Room& room = rooms[i];

        // Skip the room if it's already booked during the requested time slot
        if (find(bookedRoomCodes.begin(), bookedRoomCodes.end(), room.getCode()) != bookedRoomCodes.end()) {
            continue;
        }

        // Check if the room has the desired purpose
        if (!purpose.empty() && room.getSpecification().find(purpose) == std::string::npos) {
            continue;
        }

        // Check if the room has the desired capacity
        if (room.getCapacity() < desiredCapacity) {
            continue;
        }

        // Check if the room has the requested equipment
        if (!requestedEquipment.empty()) {
            bool equipmentMatch = true;
            const vector<string>& roomEquipment = room.getEquipment();

            // Check if each equipment in the requested list is present in the room equipment
            for (const string& reqEquip : requestedEquipment) {
                bool found = false;
                for (const string& roomEquip : roomEquipment) {
                    // Check if the room equipment contains the requested equipment
                    if (roomEquip.find(reqEquip) != std::string::npos) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    // If the requested equipment is not found, set equipmentMatch to false and break
                    equipmentMatch = false;
                    break;
                }
            }

            if (!equipmentMatch) {
                continue;
            }
        }

        // If the room passed all the checks, add it to the list of free rooms
        freeRooms.push_back(room);
    }

    return freeRooms;
}
