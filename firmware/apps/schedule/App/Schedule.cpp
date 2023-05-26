#include "Schedule.hpp"

#include <schedulec.h>
#include <stringdb.h>
#include <set>

static std::string get_talk_value(size_t talk_index, size_t value_id)
{
    char text[20];
    size_t recd = 0;
    recd = schedule_get_talk_value(talk_index, value_id, 0, text, sizeof(text)-1);
    text[recd] = '\0';
    if ( recd == 0 )
    {
        return "";
    }
    return std::string(text);
}

void Schedule::begin(BpodMenu &menu) {
    menu.set_title(STRING(STRING_SCHEDULE));

    std::set<std::string> days;

    for (size_t i = 0; i < schedule_get_talk_count(); i++)
    {
        std::string day = get_talk_value(i, SCHEDULE_DAY_VALUE_ID);
        if ( days.count(day) > 0 )
        {
            continue;
        }
        days.insert(day);
        menu.add(day, [day, this](){
            this->schedule_day_.set_day(day);
            App::manager_begin(this->schedule_day_);
        });
    }
}

void ScheduleDay::begin(BpodMenu &menu) {
    menu.set_title(this->day());

    for (size_t i = 0; i < schedule_get_talk_count(); i++)
    {
        std::string day = get_talk_value(i, SCHEDULE_DAY_VALUE_ID);
        if ( this->day() != day )
        {
            continue;
        }
        std::string time = get_talk_value(i, SCHEDULE_TIME_VALUE_ID);
        menu.add(time, [time, i, this](){
            std::string text;
            size_t text_size = schedule_get_talk_text_size(i);
            text.resize(text_size);
            size_t recd = schedule_get_talk_text(i, 0, (char*)text.c_str(), text.size());
            text.resize(recd);
            this->schedule_slot_.set_title(time);
            this->schedule_slot_.set_text(text);
            this->schedule_slot_.scroll_by_line();
            App::manager_begin(this->schedule_slot_);
        });
    }
}
