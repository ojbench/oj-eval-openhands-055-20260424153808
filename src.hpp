

#include <iostream>
#include <algorithm>
#include "base.hpp"

using namespace std;

struct date
{
    int year, month, day;
    date() = default;
    date(int y, int m, int d) : year(y), month(m), day(d) {}
    
    // Stream input operator
    friend istream& operator>>(istream& is, date& d) {
        is >> d.year >> d.month >> d.day;
        return is;
    }
    
    // Less than operator for date comparison
    bool operator<(const date& other) const {
        if (year != other.year) return year < other.year;
        if (month != other.month) return month < other.month;
        return day < other.day;
    }
    
    // Greater than or equal operator for date comparison
    bool operator>=(const date& other) const {
        return !(*this < other);
    }
};

class mail : public object
{
protected:
    string postmark;
    date send_date;
    date arrive_date;

public:
    mail() = default;

    mail(string _contain_, string _postmark_, date send_d, date arrive_d) : object(_contain_)
    {
        postmark = _postmark_;
        send_date = send_d;
        arrive_date = arrive_d;
    }

    // Override virtual functions
    string send_status(int y, int m, int d) override
    {
        return "not send";
    }

    string type() override
    {
        return "no type";
    }

    void print() override
    {
        object::print();
        cout << "[mail] postmark: " << postmark << endl;
    }

    void copy(object *o) override
    {
        contain = reinterpret_cast<mail *>(o)->contain;
        postmark = reinterpret_cast<mail *>(o)->postmark;
        send_date = reinterpret_cast<mail *>(o)->send_date;
        arrive_date = reinterpret_cast<mail *>(o)->arrive_date;
    }

    virtual ~mail() {}
};

class air_mail : public mail
{
protected:
    string airlines;
    date take_off_date;
    date land_date;

public:
    air_mail() = default;

    air_mail(string _contain_, string _postmark_, date send_d, date arrive_d, date take_off, date land, string _airline) : mail(_contain_, _postmark_, send_d, arrive_d)
    {
        take_off_date = take_off;
        land_date = land;
        airlines = _airline;
    }

    string send_status(int y, int m, int d) override
    {
        date ask_date(y, m, d);
        if (ask_date < send_date)
            return "mail not send";
        else if (ask_date < take_off_date)
            return "wait in airport";
        else if (ask_date < land_date)
            return "in flight";
        else if (ask_date < arrive_date)
            return "already land";
        else
            return "already arrive";
    }

    string type() override
    {
        return "air";
    }

    void print() override
    {
        mail::print();
        cout << "[air] airlines: " << airlines << endl;
    }

    void copy(object *o) override
    {
        mail::copy(o);
        airlines = reinterpret_cast<air_mail *>(o)->airlines;
        take_off_date = reinterpret_cast<air_mail *>(o)->take_off_date;
        land_date = reinterpret_cast<air_mail *>(o)->land_date;
    }

    virtual ~air_mail() {}
};

class train_mail : public mail
{
protected:
    string *station_name;
    date *station_time;
    int len;

public:
    train_mail() = default;

    train_mail(string _contain_, string _postmark_, date send_d, date arrive_d, string *sname, date *stime, int station_num) : mail(_contain_, _postmark_, send_d, arrive_d)
    {
        len = station_num;
        station_name = new string[len];
        station_time = new date[len];
        for (int i = 0; i < len; i++) {
            station_name[i] = sname[i];
            station_time[i] = stime[i];
        }
    }

    string send_status(int y, int m, int d) override
    {
        date ask_date(y, m, d);
        if (ask_date < send_date)
            return "mail not send";
        else if (ask_date < station_time[0])
            return "wait for departure";
        else if (ask_date < station_time[len - 1]) {
            for (int i = 0; i < len - 1; i++) {
                if (ask_date >= station_time[i] && ask_date < station_time[i + 1]) {
                    return "pass " + station_name[i];
                }
            }
        }
        else if (ask_date < arrive_date)
            return "already arrive at destination";
        else
            return "already arrive";
    }

    string type() override
    {
        return "train";
    }

    void print() override
    {
        mail::print();
        cout << "[train] station_num: " << len << endl;
    }

    void copy(object *o) override
    {
        mail::copy(o);
        train_mail *temp = reinterpret_cast<train_mail *>(o);
        len = temp->len;
        
        // Clean up existing memory
        delete[] station_name;
        delete[] station_time;
        
        // Allocate new memory and copy
        station_name = new string[len];
        station_time = new date[len];
        for (int i = 0; i < len; i++) {
            station_name[i] = temp->station_name[i];
            station_time[i] = temp->station_time[i];
        }
    }

    virtual ~train_mail()
    {
        delete[] station_name;
        delete[] station_time;
    }
};

class car_mail : public mail
{
protected:
    int total_mile;
    string driver;

public:
    car_mail() = default;

    car_mail(string _contain_, string _postmark_, date send_d, date arrive_d, int mile, string _driver) : mail(_contain_, _postmark_, send_d, arrive_d)
    {
        total_mile = mile;
        driver = _driver;
    }

    string send_status(int y, int m, int d) override
    {
        date ask_date(y, m, d);
        if (ask_date < send_date)
            return "mail not send";
        else if (ask_date >= arrive_date)
            return "already arrive";
        else {
            // Calculate current_mile = (elapsed_time / total_time) * total_mile
            // Convert dates to total days since year 0
            int send_total = send_date.year * 360 + send_date.month * 30 + send_date.day;
            int arrive_total = arrive_date.year * 360 + arrive_date.month * 30 + arrive_date.day;
            int ask_total = ask_date.year * 360 + ask_date.month * 30 + ask_date.day;
            
            int total_time = arrive_total - send_total;
            int elapsed_time = ask_total - send_total;
            
            double current_mile = (double)elapsed_time / total_time * total_mile;
            return to_string(current_mile);
        }
    }

    string type() override
    {
        return "car";
    }

    void print() override
    {
        mail::print();
        cout << "[car] driver_name: " << driver << endl;
    }

    void copy(object *o) override
    {
        mail::copy(o);
        total_mile = reinterpret_cast<car_mail *>(o)->total_mile;
        driver = reinterpret_cast<car_mail *>(o)->driver;
    }

    virtual ~car_mail() {}
};

void obj_swap(object *&x, object *&y)
{
    object *temp = x;
    x = y;
    y = temp;
}

