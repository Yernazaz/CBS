#include <vector>
#include "Booking.h"

using namespace std;

class Calendar {
public:
    Calendar();

    // Add a booking
    void addBooking(const Booking& booking);

    // Check if the time slot is available for booking
    bool isTimeSlotAvailable(time_t startTime, time_t endTime) const;

    // Get bookings for a specific date range
    vector<Booking> getBookings(time_t startDate, time_t endDate) const;

    vector<Booking> getBookings() const {
        return bookings;
    }

private:
    vector<Booking> bookings;

};
