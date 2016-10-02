#include "Timestamp.h"


bool Timestamp::operator == (const Timestamp& other) const {
    return (this->sec == other.sec) && (this->usec == other.sec);
}

bool Timestamp::operator > (const Timestamp& other) const {
    
    if (this->sec == other.sec) {
        return (this->usec > other.usec);
    } else {
        return (this->sec > other.sec);
    }
}

bool Timestamp::operator < (const Timestamp& other) const {
    
    if (this->sec == other.sec) {
        return (this->usec < other.usec);
    } else {
        return (this->sec < other.sec);
    }
}

bool Timestamp::is_empty() const {
    return (*this == Timestamp());
}

std::string Timestamp::to_str() const {
    return std::to_string(this->sec) + "." + std::to_string(this->usec);
}

Timestamp Timestamp::now() {

    struct timeval time;
    gettimeofday(&time, NULL);

    return Timestamp(time);
}

double Timestamp::get_elapsed_sec(const Timestamp& bgn, const Timestamp& end) {

    double sec = (end.sec - bgn.sec);
    double usec = (end.usec - bgn.usec);

    return sec + (usec / 1000.0 / 1000.0);
}
