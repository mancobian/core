///
/// @file Log.h
/// @author Mancobian Poemandres
/// @license BSD License
///
/// Copyright (c) MMX by The Secret Design Collective
/// All rights reserved
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
///
///    * Redistributions of source code must retain the above copyright notice,
/// 		this list of conditions and the following disclaimer.
///    * Redistributions in binary form must reproduce the above copyright notice,
/// 		this list of conditions and the following disclaimer in the documentation
/// 		and/or other materials provided with the distribution.
///    * Neither the name of The Secret Design Collective nor the names of its
/// 		contributors may be used to endorse or promote products derived from
/// 		this software without specific prior written permission.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
/// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
/// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
/// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
/// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
/// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
/// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
/// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
/// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
/// USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///

#ifndef RSSD_LOG_H
#define RSSD_LOG_H

#include <cstdarg>
#include <ostream>
#include <sstream>
#include <string>
#include <sys/time.h>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include "Pattern"

namespace RSSD {

class Loggable
{
public:
	Loggable(const std::string &log_group = "") : _log_group(log_group) {}
	virtual ~Loggable() {}

public:
	virtual inline const std::string& getLogGroup() const { return this->_log_group; }
	virtual inline void setLogGroup(const std::string &value) { this->_log_group = value; }

protected:
	std::string _log_group;
}; // class Loggable

class Log
{
public:
	struct Level
	{
	public:
		enum Type
		{
			UNKNOWN = 0,
			ERROR,
			NORMAL,
			WARNING,
			DEBUG,
			INSANE,
			COUNT
		}; // enum Type
	}; // struct Level

public:
	struct Entry
	{
		std::string File;
		size_t Line;
		Log::Level::Type Level;
		std::string Group;
		std::string Message;
	}; // struct Entry
	TYPEDEF_CONTAINERS(Entry)

public:
	typedef std::map<Log::Level::Type, const char*> LevelDescription_m;

public:
	static std::string getPrefix(const Log::Entry &entry);
	static std::string getTimestamp();

public:
	static const char *DEFAULT_GROUP;
	static const std::string FORMAT_STRING;
	static LevelDescription_m LEVEL_DESCRIPTIONS;

public:
	Log(const std::string &name);
	Log(const std::string &name, std::streambuf *buffer);
	virtual ~Log();

public:
	inline const std::string& getName() const { return this->_name; }
	inline std::ostream* getStream() { return this->_stream; }
	inline const Log::Level::Type getLevel() const { return this->_level; }
	inline void setLevel(const Log::Level::Type value) { this->_level = value; }

public:
	bool operator ==(const std::string &name);
	bool operator ==(const std::streambuf *buffer);
	bool operator ==(const Log &log);

public:
	void log(const Entry &entry);
	void log(const std::string &message,
		const std::string &file = "",
		const size_t line = 0,
		const std::string &group = Log::DEFAULT_GROUP,
		const Level::Type level = Log::Level::NORMAL);
	void log(const std::stringstream &message,
		const std::string &file = "",
		const size_t line = 0,
		const std::string &group = Log::DEFAULT_GROUP,
		const Level::Type level = Log::Level::NORMAL);

protected:
	bool _is_file;
	Log::Level::Type _level;
	std::string _name;
	std::ostream *_stream;
	boost::mutex _mutex;
}; // class Log

class LogManager :
	public Pattern::Singleton<LogManager>,
	public Pattern::Manager<Log*>
{
public:
	typedef Pattern::Manager<Log*> BaseManager;
	typedef BaseManager::Item_l Log_l;

public:
	static const char DEFAULT_NLOG_NAME[];

public:
	LogManager();
	LogManager(const std::string &name);
	~LogManager();

public:
	inline const Log::Level::Type getLogLevel() const { return this->_level; }
	inline Log* getDefaultLog() { return this->_default_log; }
	bool setDefaultLog(const Log *log);
	void setLogLevel(const Log::Level::Type value);
	bool hasLog(const std::streambuf *buffer);
	bool hasLog(const std::string &name);
	bool hasLog(const Log *log);
	Log* getLog(const std::streambuf *buffer);
	Log* getLog(const std::string &name);

public:
	Log* createLog(const std::string &name,
		bool is_default = false);
	Log* createLog(const std::string &name,
		std::streambuf *buffer,
		bool is_default = false);
	bool destroyLog(const std::string &name);
	bool destroyLog(Log *log);
	void destroyAllLogs();

public:
	void log(const std::string &message,
		const std::string &file,
		const size_t line,
		...);
	void log(const std::stringstream &message,
		const std::string &file,
		const size_t line,
		...);

protected:
	void parseVarArgs(Log::Entry &entry, va_list &args);

protected:
	Log::Level::Type _level;
	Log *_default_log;
}; // class LogManager

///
// Macros
///

#ifdef NLOG
#undef NLOG
#endif

#define NLOG(__MESSAGE, ...) \
{ \
	std::stringstream ss; \
	ss << __MESSAGE; \
	RSSD::LogManager::getPointer()->log(ss, __FILE__, __LINE__, ##__VA_ARGS__, (void*)NULL); \
}

} // namespace RSSD

#endif // RSSD_LOG_H
