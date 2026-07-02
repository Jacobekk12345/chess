#include "Utils.h"

std::string getCurrentDate() {
    auto now = std::chrono::system_clock::to_time_t(
        std::chrono::system_clock::now()
    );

    std::tm tm{};

#ifdef _WIN32
    localtime_s(&tm, &now);
#else
    localtime_r(&now, &tm);
#endif

    std::ostringstream ss;
    ss << std::put_time(&tm, "%Y.%m.%d");

    return ss.str();
}
