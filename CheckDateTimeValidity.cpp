#define DATE_CHECKER_MACROS(object_name, field_name, left_side, right_side) \
    do {    \
        if (object_name.field_name < left_side) \
            throw std::domain_error(#field_name" is too small"); \
        if (object_name.field_name > right_side) \
            throw std::domain_error(#field_name" is too big"); \
    } while (0);


void CheckDateTimeValidity(const DateTime& dt) {  
    DATE_CHECKER_MACROS(dt, year, 1, 9999);
    DATE_CHECKER_MACROS(dt, month, 1, 12);

    const bool is_leap_year = (dt.year % 4 == 0) && !(dt.year % 100 == 0 && dt.year % 400 != 0);
    const array month_lengths = {31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    DATE_CHECKER_MACROS(dt, day, 1, month_lengths[dt.month - 1]);
    DATE_CHECKER_MACROS(dt, hour, 0, 23);
    DATE_CHECKER_MACROS(dt, minute, 0, 59);
    DATE_CHECKER_MACROS(dt, second, 0, 59);
}