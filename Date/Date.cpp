#include <bits/stdc++.h>
using namespace std;
using ll = long long;

/*=============================================================
  日期时间模板 DT

  应用场景:
  1. 两日期间隔天数/秒数（签到打卡、计费）
  2. 星期几判断（排班、周期问题）
  3. 日期遍历枚举（逐日统计）
  4. 时间排序+配对（上下班打卡求工时）
  5. 闰年/月天数判断

  输入格式: date="YYYY-MM-DD"  time="HH:MM:SS"
  time 可省略（默认 "00:00:00"）
=============================================================*/
struct DT {
    string date, time;
    DT(string d = "0001-01-01", string t = "00:00:00") : date(d), time(t) {}

    // --- 解析字段 ---
    int Y()  { return stoi(date.substr(0,4)); }
    int Mo() { return stoi(date.substr(5,2)); }
    int D()  { return stoi(date.substr(8,2)); }
    int h()  { return stoi(time.substr(0,2)); }
    int mi() { return stoi(time.substr(3,2)); }
    int s()  { return stoi(time.substr(6,2)); }

    // --- 基础工具 ---
    static bool leap(int y) { return (y%4==0 && y%100!=0) || y%400==0; }
    static int md(int y, int m) {
        int d[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
        return (m==2 && leap(y)) ? 29 : d[m];
    }

    // --- 绝对量（用于做差）---
    ll to_days() {
        int y=Y(), m=Mo(), d=D();
        ll r = (ll)(y-1)*365 + (y-1)/4 - (y-1)/100 + (y-1)/400;
        for (int i = 1; i < m; i++) r += md(y, i);
        return r + d;
    }
    ll to_sec() { return (to_days()-1)*86400LL + h()*3600 + mi()*60 + s(); }

    // --- 常用操作 ---
    static ll dsec(DT a, DT b) { return b.to_sec() - a.to_sec(); }
    static ll dday(DT a, DT b) { return b.to_days() - a.to_days(); }

    int wday() { int w = to_days() % 7; return w ? w : 7; } // 1=周一 7=周日

    void nxt() { // 推进到下一天
        int y=Y(), mo=Mo(), d=D();
        if (++d > md(y,mo)) { d=1; if (++mo>12) { mo=1; y++; } }
        char buf[11]; sprintf(buf, "%04d-%02d-%02d", y, mo, d);
        date = buf;
    }

    bool operator<(const DT& o) const {
        return date == o.date ? time < o.time : date < o.date;
    }

    // 快速构造（免手写字符串）
    static DT make(int y, int mo=1, int d=1, int h=0, int mi=0, int s=0) {
        char db[11], tb[9];
        sprintf(db, "%04d-%02d-%02d", y, mo, d);
        sprintf(tb, "%02d:%02d:%02d", h, mi, s);
        return {db, tb};
    }
};

int main() {
    // 1. 构造 & 解析
    DT a = DT::make(2022, 1, 1, 7, 58, 2);
    assert(a.date == "2022-01-01" && a.time == "07:58:02");
    assert(a.Y()==2022 && a.Mo()==1 && a.D()==1);

    // 2. 秒数差（同日不同时间）
    DT b = DT::make(2022, 1, 1, 12, 0, 5);
    assert(DT::dsec(a, b) == 14523);  // 约4小时

    // 3. 天数差（跨年）
    assert(DT::dday(DT::make(2024,12,31), DT::make(2025,1,1)) == 1);

    // 4. 跨年秒数差
    assert(DT::dsec(DT::make(2021,12,31,23,59,59), DT::make(2022,1,1,0,0,1)) == 2);

    // 5. 排序 + 配对求工时
    vector<DT> v = {
        {"2022-01-01","12:00:05"}, {"2022-01-02","00:20:05"},
        {"2022-01-01","07:58:02"}, {"2022-01-01","16:01:35"}
    };
    sort(v.begin(), v.end());
    ll total = 0;
    for (int i = 0; i+1 < (int)v.size(); i += 2)
        total += DT::dsec(v[i], v[i+1]);
    assert(total == 44433);

    // 6. 星期几  2024-01-01 = 周一
    assert(DT::make(2024,1,1).wday() == 1);

    // 7. 日期遍历（闰年2月）
    DT d = DT::make(2024,2,28);
    d.nxt(); assert(d.date == "2024-02-29");
    d.nxt(); assert(d.date == "2024-03-01");

    cout << "All Passed!\n";
}