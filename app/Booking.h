#include <string>
#include <ctime>

using namespace std;

class Booking {
public:
    Booking(const string& roomCode, const string& userName, const string& userID, time_t startTime, time_t endTime, const string& reason);

    // Getters
    string getUserName() const;
    string getUserID() const;
    time_t getStartTime() const;
    time_t getEndTime() const;
    string getReason() const;

    // Setters
    void setUserName(const string& userName);
    void setUserID(const string& userID);
    void setStartTime(time_t startTime);
    void setEndTime(time_t endTime);
    void setReason(const string& reason);

    string toCSV() const;

private:
    string roomCode;
    string userName;
    string userID;
    time_t startTime;
    time_t endTime;
    string reason;
};
