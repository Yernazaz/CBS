#include <iostream>
#include <iomanip>
#include <ctime>
#include "CBS.h"
#include <sstream>
#include <windows.h>

using namespace std;

time_t stringToDate(const string& dateStr) {
    istringstream iss(dateStr);
    char delimiter;
    int day, month, year;
    iss >> day >> delimiter >> month >> delimiter >> year;

    struct tm timeinfo = { 0 };
    timeinfo.tm_mday = day;
    timeinfo.tm_mon = month - 1; // months are zero-based
    timeinfo.tm_year = year - 1900; // years since 1900

    return mktime(&timeinfo);
}

string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == string::npos) {
        return "";
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

vector<string> splitAndTrim(const string& input, char delimiter) {
    vector<string> result;
    istringstream ss(input);
    string token;

    while (getline(ss, token, delimiter)) {
        result.push_back(trim(token));
    }

    return result;
}

time_t stringToTime(const string& dateStr, const string& hourStr) {
    istringstream iss(dateStr);
    char delimiter;
    int day, month, year;
    iss >> day >> delimiter >> month >> delimiter >> year;

    int hour;
    istringstream(hourStr) >> hour;

    struct tm timeinfo = { 0 };
    timeinfo.tm_mday = day;
    timeinfo.tm_mon = month - 1; // months are zero-based
    timeinfo.tm_year = year - 1900; // years since 1900
    timeinfo.tm_hour = hour;

    return mktime(&timeinfo);
}

void setColor(int color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void setFont(int size)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX font;
    font.cbSize = sizeof(font);
    GetCurrentConsoleFontEx(hConsole, FALSE, &font);
    font.dwFontSize.Y = size;
    SetCurrentConsoleFontEx(hConsole, FALSE, &font);
}

int main() {
    CBS cbs;
    cbs.loadRooms();
    string startDateStr, startHourStr, endHourStr;
    time_t startTime, endTime;
    int capacity;
    string specification;
    vector<string> equipmentList;

    int choice;
    do {
        cout << "\nChoose an option:\n";
        cout << "1. Show booking state\n";
        cout << "2. Find free rooms\n";
        cout << "3. Book a room\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            string roomCode;
            cout << "Enter room code: ";
            cin >> roomCode;

            string startDateStr, endDateStr;
            cout << "Enter the start date (dd/mm/yyyy): ";
            cin >> startDateStr;
            cout << "Enter the end date (dd/mm/yyyy): ";
            cin >> endDateStr;

            time_t startDate = stringToDate(startDateStr);
            time_t endDate = stringToDate(endDateStr);

            // Check if the year is 2023 and if the end day is after the start day
            struct tm startTm, endTm;
            localtime_s(&startTm, &startDate);
            localtime_s(&endTm, &endDate);

            if (startTm.tm_year + 1900 != 2023 || endTm.tm_year + 1900 != 2023) {
                cout << "Incorrect date. The year must be 2023." << endl;
            }
            else if (difftime(endDate, startDate) < 0) {
                cout << "Incorrect date. The end date must be after the start date." << endl;
            }
            else {
                startTm.tm_hour = 8; // Set start hour to 8 AM
                endTm.tm_hour = 22; // Set end hour to 10 PM
                startDate = mktime(&startTm);
                endDate = mktime(&endTm);

                cbs.showBookingState(roomCode, startDate, endDate);
            }

            break;
        }
        case 2: {
            string startDateStr, startHourStr, endHourStr, specification;
            int capacity;
            vector<string> equipmentList;
            time_t startTime, endTime;

            cout << "Enter the start date (dd/mm/yyyy): ";
            cin >> startDateStr;

            // Check if year is 2023
            string yearStr = startDateStr.substr(6, 4);
            int year = stoi(yearStr);
            if (year != 2023) {
                cout << "Invalid year. Year must be 2023." << endl;
                break;
            }

            // Check if day is valid
            string dayStr = startDateStr.substr(0, 2);
            int day = stoi(dayStr);
            if (day < 1 || day > 30) {
                cout << "Invalid day. Day must be between 1 and 30." << endl;
                break;
            }

            // Check if month is valid
            string monthStr = startDateStr.substr(3, 2);
            int month = stoi(monthStr);
            if (month < 1 || month > 12) {
                cout << "Invalid month. Month must be between 1 and 12." << endl;
                break;
            }

            cout << "Enter the start hour (8-22): ";
            cin >> startHourStr;
            cout << "Enter the end hour (8-22): ";
            cin >> endHourStr;

            // Check if end hour is after start hour
            int startHour = stoi(startHourStr);
            int endHour = stoi(endHourStr);
            if (endHour <= startHour) {
                cout << "Invalid end hour. End hour must be after start hour." << endl;
                break;
            }

            cout << "Enter the desired capacity: ";
            if (!(cin >> capacity)) {
                cout << "Invalid capacity. Capacity must be an integer." << endl;
                break;
            }
            cin.ignore(); // consume the newline character

            cout << "Enter the purpose (optional): ";
            getline(cin, specification);

            cout << "Enter the necessary equipment (optional, comma separated): ";
            string equipmentStr;
            getline(cin, equipmentStr);
            equipmentList = splitAndTrim(equipmentStr, ',');

            startTime = stringToTime(startDateStr, startHourStr);
            endTime = stringToTime(startDateStr, endHourStr);

            vector<Room> freeRooms = cbs.findFreeRoom(startTime, endTime, capacity, specification, equipmentList);

            if (freeRooms.size() == 0) {
                cout << "No free rooms found." << endl;
            }
            else {
                cout << "Free rooms matching the criteria:" << endl;
                for (const auto& room : freeRooms) {
                    cout << "Room code: " << room.getCode() << ", capacity: " << room.getCapacity();
                    if (!room.getSpecification().empty()) {
                        cout << ", specification: " << room.getSpecification();
                    }
                    if (!room.getEquipment().empty()) {
                        cout << ", equipment: ";
                        for (const auto& e : room.getEquipment()) {
                            cout << e << " ";
                        }
                    }
                    cout << endl;
                }
            }
            break;
        }
        case 3: {
            string userName, userID, roomCode, startDateStr, startHourStr, endHourStr, reason;

            cout << "Enter user name: ";
            cin.ignore();
            getline(cin, userName);
            cout << "Enter user ID: ";
            cin >> userID;
            cout << "Enter room code: ";
            cin >> roomCode;
            cout << "Enter the start date (dd/mm/yyyy): ";
            cin >> startDateStr;
            cout << "Enter the start hour (8-22): ";
            cin >> startHourStr;
            cout << "Enter the end hour (8-22): ";
            cin >> endHourStr;

            time_t startDate = stringToTime(startDateStr, startHourStr);
            time_t endDate = stringToTime(startDateStr, endHourStr);

            cout << "Enter reason for booking: ";
            cin.ignore();
            getline(cin, reason);

            bool bookingSuccessful = cbs.bookRoom(userName, userID, roomCode, startDate, endDate, reason);

            if (bookingSuccessful) {
                cout << "Booking successful!" << endl;
            }
            else {
                cout << "Booking failed." << endl;
            }

            break;
        }
        case 0:
            cout << "Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice. Try again." << endl;
            break;
        }
    } while (choice != 0);

    return 0;
}
