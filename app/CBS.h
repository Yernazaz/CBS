#include <vector>
#include "Room.h"
#include "Calendar.h"

using namespace std;

class CBS {
public:
    CBS();

    // Load rooms from room_db.csv
    void loadRooms();

    vector<Room> findFreeRoom(time_t startTime, time_t endTime, int desiredCapacity, const string& purpose, const vector<string>& requestedEquipment);

    void displayAllRooms();

    void saveBookings();

    // Show the booking state of a room
    void showBookingState(const string& roomCode, time_t startDate, time_t endDate);

 
    // Book a room
    bool bookRoom(const string& userName, const string& userID, const string& roomCode, time_t startTime,
        time_t endTime, const string& reason);

    void loadBookings();

    void findFreeRooms(time_t startTime, time_t endTime, int capacity, const string& purpose, const vector<string>& necessaryEquipment) const;

private:
    vector<Room> rooms;
    vector<Calendar> roomCalendars;
};