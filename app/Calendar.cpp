#include "Calendar.h"

Calendar::Calendar() {}

void Calendar::addBooking(const Booking& booking) {
    bookings.push_back(booking);
}

bool Calendar::isTimeSlotAvailable(time_t startTime, time_t endTime) const {
    for (const Booking& booking : bookings) {
        if ((booking.getStartTime() < endTime) && (booking.getEndTime() > startTime)) {
            return false;
        }
    }
    return true;
}

vector<Booking> Calendar::getBookings(time_t startTime, time_t endTime) const {
    vector<Booking> overlappingBookings;

    for (const Booking& booking : bookings) {
        // Check if booking overlaps with given time range
        if (booking.getEndTime() > startTime && booking.getStartTime() < endTime) {
            overlappingBookings.push_back(booking);
        }
    }

    return overlappingBookings;
}

