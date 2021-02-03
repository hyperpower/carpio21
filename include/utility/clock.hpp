/* Copyright 2010 Jukka Jylanki

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License. */
#pragma once

/**
 *  @file  Clock.h
 *  @brief The Clock class. Supplies timing facilities.
 **/
#include "tinyformat.hpp"
#include <iostream>
#include <iterator>
#include <list>
#include "type_define.hpp"

#if defined(__unix__) || defined(__native_client__) || defined(EMSCRIPTEN) || defined(ANDROID) || defined(__APPLE__) || defined (__CYGWIN__)
#include <time.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#endif

#ifdef WIN32
#include <Windows.h>
#endif

#ifdef __APPLE__
#include <mach/mach_time.h>
#endif

#ifndef MARK_UNUSED
/// If a variable is labelled with this directive, the compiler should not emit a warning even if it is unused in the code.
#define MARK_UNUSED(x) ((void)x)
#endif

namespace carpio {

/// A tick is the basic unit of the high-resolution timer. If MATH_TICK_IS_FLOAT is defined,
/// then tick_t is a floating-point type. Otherwise a 64-bit unsigned integer is used instead.
#ifdef _TICK_IS_FLOAT
typedef double tick_t;
const tick_t TICK_INF = std::numeric_limits<double>::infinity();
#else
typedef unsigned long long tick_t;
const tick_t TICK_INF = 0xFFFFFFFFFFFFFFFFULL;
#endif

/**
 *  @brief High-resolution timing and system time.
 *
 *  Gives out timing information in various forms. Use this rather than
 *  any platform-dependent perf-counters or rdtsc or whatever.
 */
class Clock {
public:
	Clock() {
		this->start();
	}
	Clock(const std::string& name): Clock(){
		this->_name = name;
	}

//	~Clock() {}

/// Sleeps the current thread for the given amount of milliseconds.
	static void Sleep(int milliseconds) {
#ifdef WIN8RT
#pragma warning(Clock::Sleep has not been implemented!)
#elif defined(WIN32)
		::Sleep(milliseconds);
#elif !defined(__native_client__) && !defined(EMSCRIPTEN)
		// http://linux.die.net/man/2/nanosleep
		timespec ts;
		ts.tv_sec = milliseconds / 1000;
		ts.tv_nsec = (milliseconds - ts.tv_sec * 1000) * 1000 * 1000;
		int ret = nanosleep(&ts, nullptr);
		if (ret == -1)
			printf("nanosleep returned -1! Reason: %s(%d).", strerror(errno),
					(int) errno); //revised
#else
#warning Clock::Sleep has not been implemented!
#endif
	}

	/// @return The current year.
	static int Year() {
#ifdef WIN32
		SYSTEMTIME s;
		GetSystemTime(&s);
		return s.wYear;
#else
		time_t theTime = time(nullptr);
		struct tm *aTime = localtime(&theTime);
		return aTime->tm_year + 1900;
#endif
	}

	/// @return The current month, [1,12]
	static int Month() {
#ifdef WIN32
		SYSTEMTIME s;
		GetSystemTime(&s);
		return s.wMonth;
#else
		time_t theTime = time(nullptr);
		struct tm *aTime = localtime(&theTime);
		return aTime->tm_mon;
#endif
	}

	/// @return The current day of month, [1,31]
	static int Day() {
#ifdef WIN32
		SYSTEMTIME s;
		GetSystemTime(&s);
		return s.wDay;
#else
		time_t theTime = time(nullptr);
		struct tm *aTime = localtime(&theTime);
		return aTime->tm_mday;
#endif
	}

	/// @return The current hour of day, [0,23]
	static int Hour() {
#ifdef WIN32
		SYSTEMTIME s;
		GetSystemTime(&s);
		return s.wHour;
#else
		time_t theTime = time(nullptr);
		struct tm *aTime = localtime(&theTime);
		return aTime->tm_hour;
#endif
	}

	/// @return The current clock minute, [0,59]
	static int Min() {
#ifdef WIN32
		SYSTEMTIME s;
		GetSystemTime(&s);
		return s.wMinute;
#else
		time_t theTime = time(nullptr);
		struct tm *aTime = localtime(&theTime);
		return aTime->tm_min;
		return 0;
#endif
	}

	/// @return The current clock second, [0,59]
	static int Sec() {
#ifdef WIN32
		SYSTEMTIME s;
		GetSystemTime(&s);
		return s.wSecond;
#else
		time_t theTime = time(nullptr);
		struct tm *aTime = localtime(&theTime);
		return aTime->tm_sec;
		return 0;
#endif
	}

	/// @return The current system time counter in milliseconds.
	static unsigned long SystemTime() {
#ifdef WIN32
#if WINVER >= 0x0600 /* Vista or newer */ && !defined(MATH_ENABLE_WINXP_SUPPORT)
		return (unsigned long)GetTickCount64();
#else
// We are explicitly building with XP support, so GetTickCount() instead of GetTickCount64 is desired.
#if _MSC_VER >= 1700 // VS2012
#pragma warning(push)
#pragma warning(disable:28159) // warning C28159: Consider using 'GetTickCount64' instead of 'GetTickCount'. Reason: GetTickCount overflows roughly every 49 days.  Code that does not take that into account can loop indefinitely.  GetTickCount64 operates on 64 bit values and does not have that problem
#endif
		return (unsigned long)GetTickCount();
#if _MSC_VER >= 1700 // VS2012
#pragma warning(pop)
#endif

#endif
#else
		return TickU32();
#endif
	}

	/// @return The low part of the current tick-time (using whatever high-resolution counter available)
	static unsigned long TickU32() {
#ifdef WIN32
		LARGE_INTEGER ddwTimer;
		BOOL success = QueryPerformanceCounter(&ddwTimer);
		__assume(success != 0);
		MARK_UNUSED(success);
		return ddwTimer.LowPart;
#else
		return (unsigned long) Tick();
#endif
	}

	/// @return The current tick-time (using whatever high-resolution counter available)
	static tick_t Tick() {
		//android
#if defined(ANDROID)
		struct timespec res;
		clock_gettime(CLOCK_REALTIME, &res);
		return 1000000000ULL*res.tv_sec + (tick_t)res.tv_nsec;
		//android
#elif defined(EMSCRIPTEN)
#ifdef _TICK_IS_FLOAT
		return (tick_t)emscripten_get_now();
#else
		// emscripten_get_now() returns a wallclock time as a float in milliseconds (1e-3).
		// scale it to microseconds (1e-6) and return as a tick.
		return (tick_t)(((double)emscripten_get_now()) * 1e3);
#endif

#elif defined(WIN32)
		LARGE_INTEGER ddwTimer;
		BOOL success = QueryPerformanceCounter(&ddwTimer);
		__assume(success != 0);
		MARK_UNUSED(success);
		return ddwTimer.QuadPart;
#elif defined(__APPLE__)
		return mach_absolute_time();
#elif defined(_POSIX_MONOTONIC_CLOCK)
		timespec t;
		clock_gettime(CLOCK_MONOTONIC, &t);
		return (tick_t)t.tv_sec * 1000 * 1000 * 1000 + (tick_t)t.tv_nsec;
#elif defined(_POSIX_C_SOURCE)
		timeval t;
		gettimeofday(&t, nullptr);
		return (tick_t) t.tv_sec * 1000 * 1000 + (tick_t) t.tv_usec;
#else
		return (tick_t)clock();
#endif
	}

	/// @return How many ticks make up a second.
	static tick_t TicksPerSec() {
#if defined(ANDROID)
		return 1000000000ULL; // 1e9 == nanoseconds.
#elif defined(EMSCRIPTEN)

#ifdef _TICK_IS_FLOAT
		return (tick_t)1000.0;
#else
		return 1000000ULL; // 1e6 == microseconds.
#endif

#elif defined(WIN32)
		LARGE_INTEGER ddwTimerFrequency;
		if (!QueryPerformanceFrequency(&ddwTimerFrequency)){
			ddwTimerFrequency.HighPart = (unsigned long)-1;
			ddwTimerFrequency.LowPart = (unsigned long)-1;
		}

		return ddwTimerFrequency.QuadPart;
#elif defined(__APPLE__)
		return ticksPerSecond;
#elif defined(_POSIX_MONOTONIC_CLOCK)
		return 1000 * 1000 * 1000;
#elif defined(_POSIX_C_SOURCE) || defined(__APPLE__)
		return 1000 * 1000;
#else
		return CLOCKS_PER_SEC;
#endif
	}

	static inline tick_t TicksPerMillisecond() {
		return TicksPerSec() / 1000;
	}

	/// @returns the number of ticks occurring between the two wallclock times.
	static inline tick_t TicksInBetween(tick_t newTick, tick_t oldTick) {
		return (tick_t) (newTick - oldTick);
	}

	/// @returns true if newTick represents a later wallclock time than oldTick.
	static inline bool IsNewer(tick_t newTick, tick_t oldTick) {
#ifdef _TICK_IS_FLOAT
		return newTick >= oldTick;
#else
		return TicksInBetween(newTick, oldTick) < ((tick_t) (-1) >> 1);
#endif
	}

	static inline float MillisecondsSinceF(tick_t oldTick) {
		return TimespanToMillisecondsF(oldTick, Tick());
	}
	static inline double MillisecondsSinceD(tick_t oldTick) {
		return TimespanToMillisecondsD(oldTick, Tick());
	}

	static inline float SecondsSinceF(tick_t oldTick) {
		return TimespanToSecondsF(oldTick, Tick());
	}
	static inline double SecondsSinceD(tick_t oldTick) {
		return TimespanToSecondsD(oldTick, Tick());
	}

	static inline float TicksToMillisecondsF(tick_t ticks) {
		return float(ticks * 1000.0 / (float) TicksPerSec());
	}
	static inline double TicksToMillisecondsD(tick_t ticks) {
		return ticks * 1000.0 / (double) TicksPerSec();
	}

	static inline float TicksToSecondsF(tick_t ticks) {
		return ticks / (float) TicksPerSec();
	}
	static inline double TicksToSecondsD(tick_t ticks) {
		return ticks / (double) TicksPerSec();
	}

	static inline float TimespanToMillisecondsF(tick_t oldTick,
			tick_t newTick) {
		return TicksToMillisecondsF(TicksInBetween(newTick, oldTick));
	}
	static inline double TimespanToMillisecondsD(tick_t oldTick,
			tick_t newTick) {
		return TicksToMillisecondsD(TicksInBetween(newTick, oldTick));
	}

	static inline float TimespanToSecondsF(tick_t oldTick, tick_t newTick) {
		return TicksToSecondsF(TicksInBetween(newTick, oldTick));
	}
	static inline double TimespanToSecondsD(tick_t oldTick, tick_t newTick) {
		return TicksToSecondsD(TicksInBetween(newTick, oldTick));
	}

	/*
	 * start a clock
	 */
	void start() {
		this->_start_time_cpu = Clock::SystemTime();
		this->_start_time_wt = Clock::Tick();
	}

	void break_point(const std::string& name = "", const Float& num = 1) {
		_tp_name.push_back(name);
		_tp_cpu.push_back(Clock::SystemTime());
		_tp_wt.push_back(Clock::Tick());
		_tp_num.push_back(num);
	}

	void clear_records() {
		_tp_cpu.clear();
		_tp_wt.clear();
		_tp_name.clear();
		_tp_num.clear();
	}

	typename std::list<std::string>::size_type
	size() const {
		return _tp_name.size();
	}

protected:
	typedef std::list<tick_t>::iterator iterator_t;
	typedef std::list<tick_t>::const_iterator const_iterator_t;

	typedef std::list<Float>::iterator iterator_n;
	typedef std::list<Float>::const_iterator const_iterator_n;

public:
	tick_t start_time_cpu() const {
		return this->_start_time_cpu;
	}

	tick_t start_time_wall() const {
		return this->_start_time_wt;
	}

	iterator_t begin_cpu() {
		return this->_tp_cpu.begin();
	}

	const_iterator_t begin_cpu() const {
		return this->_tp_cpu.begin();
	}

	iterator_t end_cpu() {
		return this->_tp_cpu.end();
	}

	const_iterator_t end_cpu() const {
		return this->_tp_cpu.end();
	}

	iterator_t begin_wall() {
		return this->_tp_wt.begin();
	}

	const_iterator_t begin_wall() const {
		return this->_tp_wt.begin();
	}

	iterator_t end_wall() {
		return this->_tp_wt.end();
	}

	const_iterator_t end_wall() const {
		return this->_tp_wt.end();
	}

	iterator_n begin_num() {
		return this->_tp_num.begin();
	}

	const_iterator_n begin_num() const {
		return this->_tp_num.begin();
	}

	iterator_n end_num() {
		return this->_tp_num.end();
	}

	const_iterator_n end_num() const {
		return this->_tp_num.end();
	}

	std::string name() const{
		return this->_name;
	}

	void show() {
		// get leatest time
		tick_t d_end_cpu, d_end_wt;
		tfm::format(std::cout, "%7s %10s %10s %5s %5s %7s %s\n",
				" Index", "Cpu", "Wall", "Cpu%", "Wall%", "Num", "Name");
		tfm::format(std::cout, "%60f\n", " ");
		if (_tp_cpu.empty()) {
			std::cout << " Empty\n";
			return;
		} else {
			d_end_cpu = Clock::TicksInBetween(*(_tp_cpu.rbegin()),
					_start_time_cpu);
			d_end_wt = Clock::TicksInBetween(*(_tp_wt.rbegin()),
					_start_time_wt);
		}
		std::list<tick_t>::iterator iter_c  = _tp_cpu.begin(),
				                    iter_wt =_tp_wt.begin();
		std::list<std::string>::iterator iter_name = _tp_name.begin();
		iterator_n iter_num = _tp_num.begin();

		// loop --------------------
		int i = 0;
		while (iter_c != _tp_cpu.end()) {
			tick_t dt_cpu, dt_wt;
			if (iter_c == _tp_cpu.begin()) {
				dt_cpu = Clock::TicksInBetween(*iter_c, _start_time_cpu);
				dt_wt = Clock::TicksInBetween(*iter_wt, _start_time_wt);
			} else {
				// c++11
				auto prev_c = std::prev(iter_c, 1);
				auto prev_wt = std::prev(iter_wt, 1);
				dt_cpu = Clock::TicksInBetween(*iter_c, *prev_c);
				dt_wt = Clock::TicksInBetween(*iter_c, *prev_wt);
			}
			// output
			tfm::format(
					"%7d %10.5f %10.5f %5.2f %5.2f %4.1e %s\n",
					i,  // Index
					Clock::TicksToSecondsD(dt_cpu), // d Cpu time
					Clock::TicksToSecondsD(dt_wt),  // d Wall time
					Clock::TicksToSecondsD(dt_cpu)
							/ Clock::TicksToSecondsD(d_end_cpu) * 100,
					Clock::TicksToSecondsD(dt_wt)
							/ Clock::TicksToSecondsD(d_end_wt) * 100,
					(*iter_num), (*iter_name));


			++iter_c;
			++iter_wt;
			++iter_name;
			++iter_num;
			++i;
		}
//		tfmt::format("{:-<60}<\n", " ");
//		tfmt::format("{: ^7} {: >10.5f} {: >10.5f}\n", "sum",
//				Clock::TicksToSecondsD(d_end_cpu),
//				Clock::TicksToSecondsD(d_end_wt));

	}

protected:

	tick_t _start_time_cpu;
	tick_t _start_time_wt;

	std::string       _name;          // clock name
	std::list<tick_t> _tp_cpu;        // time points of cpu time
	std::list<tick_t> _tp_wt;         // time points of wall time
	std::list<std::string> _tp_name;
	std::list<Float> _tp_num;        // store
};

}
