#pragma once

#include <string>

#include <App/Menu.hpp>
#include <App/TextView.hpp>

class ScheduleDay : public Menu  {
    public:
        ScheduleDay() {};
        virtual ~ScheduleDay() {};
        virtual void begin(BpodMenu &menu);
        void set_day(const std::string& day) { day_ = day; };
        const std::string &day() const { return day_; }
    private:
        std::string day_;
        TextView schedule_slot_;
};

class Schedule : public Menu  {
    public:
        Schedule() {};
        virtual ~Schedule() {};
        virtual void begin(BpodMenu &menu);
    private:
        ScheduleDay schedule_day_;
};
