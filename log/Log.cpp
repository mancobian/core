#include "Log.h"
#include <cstdarg>
#include <fstream>
#include <iostream>
#include <boost/assign.hpp>

using namespace RSSD;

///
/// @class Log
///

///
/// @note typical US display e.g. 06/15/2003 08:31:20 (24 hr time)
/// @deprecated const std::string Log::FORMAT_STRING = "%m-%d-%Y %I:%M:%S.%%06u";
///

const std::string Log::FORMAT_STRING = "%H:%M:%S.%%06u";
const char *Log::DEFAULT_GROUP = "";
Log::LevelDescription_m Log::LEVEL_DESCRIPTIONS = boost::assign::map_list_of
	(Log::Level::UNKNOWN, "?")
	(Log::Log::Level::ERROR, "p")
	(Log::Level::NORMAL, "-")
	(Log::Level::WARNING, "w")
	(Log::Level::DEBUG, "d")
	(Log::Level::INSANE, "!");

///
/// @reference http://stackoverflow.com/questions/1551597/using-strftime-in-c-how-can-i-format-time-exactly-like-a-unix-timestamp
///

std::string Log::getTimestamp()
{
	// Local vars
	const uint32_t SIZE = Log::FORMAT_STRING.size() * 2;
	char sec_buffer[SIZE],
		usec_buffer[SIZE];
	timeval current_timeval;
	tm *current_time = NULL;

	// Get the current time value
	gettimeofday(&current_timeval, NULL);
	current_time = localtime(&current_timeval.tv_sec);

	// Format the current time
	strftime(sec_buffer,
		SIZE,
		Log::FORMAT_STRING.c_str(),
		current_time);
	snprintf(usec_buffer,
		SIZE,
		sec_buffer,
		current_timeval.tv_usec);
	return usec_buffer;
}

std::string Log::getPrefix(const Log::Entry &entry)
{
	// Local vars
	std::stringstream prefix;

	// Append log level
	prefix << "(" << Log::LEVEL_DESCRIPTIONS[entry.Level] << ") ";

	// Append timestamp
	prefix << Log::getTimestamp();

	// Append and format log entry group
	if (!entry.Group.empty())
		prefix << " [" << entry.Group << "]";

	// Append file and line number
	boost::filesystem::path filepath = entry.File;
	prefix << " (" << filepath.leaf() << ", " << entry.Line << ")";

	// Append prefix delimiter
	prefix << ": ";

	return prefix.str();
}

Log::Log(const std::string &name) :
	_is_file(true),
	_level(Log::Level::NORMAL),
	_name(name),
	_stream(new std::ofstream(name.c_str()))
{
}

Log::Log(const std::string &name,
	std::streambuf *buffer) :
	_is_file(false),
	_level(Log::Level::NORMAL),
	_name(name),
	_stream(new std::ostream(buffer))
{
}

Log::~Log()
{
	if (this->_is_file && this->_stream)
	{
		static_cast<std::ofstream*>(this->_stream)->close();
		delete this->_stream;
	}
}

bool Log::operator ==(const std::string &name)
{
	if (name.empty())
		return false;
	return (this->_name.compare(name) == 0);
}

bool Log::operator ==(const std::streambuf *buffer)
{
	if (buffer && this->_stream
		&& (this->_stream->rdbuf() == buffer))
		return true;
	return false;
}

bool Log::operator ==(const Log &rhs)
{
	if (!this->_stream || !rhs._stream)
		return false;
	return (this->_stream->rdbuf() == rhs._stream->rdbuf());
}

void Log::log(
	const std::string &message,
	const std::string &file,
	const size_t line,
	const std::string &group,
	const Log::Level::Type level)
{
	Log::Entry entry = {file, line, level, group, message};
	this->log(entry);
}

void Log::log(
	const std::stringstream &message,
	const std::string &file,
	const size_t line,
	const std::string &group,
	const Log::Level::Type level)
{
	Log::Entry entry = {file, line, level, group, message.str()};
	this->log(entry);
}

void Log::log(const Log::Entry &entry)
{
	boost::mutex::scoped_lock lock(this->_mutex);

	if (entry.Level > this->_level)
		return;

	*this->_stream << Log::getPrefix(entry);
	*this->_stream << entry.Message << std::endl;

	// std::cout << Log::getPrefix(entry) << entry.Message << std::endl;
}

///
/// @class LogManager
///

template<> LogManager* Pattern::Singleton<LogManager>::_instance = NULL;
const char LogManager::DEFAULT_NLOG_NAME[] = "nous.default.log";

LogManager::LogManager() :
	Pattern::Manager<Log*>(),
	_default_log(NULL)
{
	this->_default_log = this->createLog(
		LogManager::DEFAULT_NLOG_NAME,
		true);
}

LogManager::LogManager(const std::string &name) :
	Pattern::Manager<Log*>(),
	_default_log(NULL)
{
	this->_default_log = this->createLog(
		name,
		true);
}

LogManager::~LogManager()
{
	this->destroyAllLogs();
}

void LogManager::setLogLevel(const Log::Level::Type value)
{
	this->_level = value;
	Log_l::iterator
		iter = this->_items.begin(),
		end = this->_items.end();
	for (; iter != end; ++iter)
	{
		Log *log = *iter;
		log->setLevel(value);
	}
}

bool LogManager::hasLog(const std::streambuf *buffer)
{
	return (this->getLog(buffer) != NULL);
}

bool LogManager::hasLog(const std::string &name)
{
	return (this->getLog(name) != NULL);
}

bool LogManager::hasLog(const Log *log)
{
	if (!log)
		return false;
	return (this->getLog(log->getName()) != NULL);
}

Log* LogManager::getLog(const std::streambuf *buffer)
{
	Log_l::iterator
		iter = this->_items.begin(),
		end = this->_items.end();
	for (; iter != end; ++iter)
	{
		Log *log = *iter;
		if (*log == buffer)
			return log;
	}
	return NULL;
}

Log* LogManager::getLog(const std::string &name)
{
	Log_l::iterator
		iter = this->_items.begin(),
		end = this->_items.end();
	for (; iter != end; ++iter)
	{
		Log *log = *iter;
		if (*log == name)
			return log;
	}
	return NULL;
}

Log* LogManager::createLog(
	const std::string &name,
	bool is_default)
{
	Log *log = new Log(name);
	this->add(log);
	if (is_default)
		this->_default_log = log;
	return log;
}

Log* LogManager::createLog(
	const std::string &name,
	std::streambuf *buffer,
	bool is_default)
{
	Log *log = new Log(name, buffer);
	this->add(log);
	if (is_default)
		this->_default_log = log;
	return log;
}

bool LogManager::destroyLog(const std::string &name)
{
	Log *log = this->getLog(name);
	return this->destroyLog(log);
}

bool LogManager::destroyLog(Log *log)
{
	if (this->remove(log))
		delete log;
	return true;
}

void LogManager::destroyAllLogs()
{
	this->clear();
}

void LogManager::log(
	const std::string &message,
	const std::string &file,
	const size_t line,
	...)
{
	Log::Entry entry;
	entry.Message = message;
	entry.File = file;
	entry.Line = line;

	va_list args;
	va_start(args, line);
	this->parseVarArgs(entry, args);
	va_end(args);

	this->_default_log->log(entry);
}

void LogManager::log(
	const std::stringstream &message,
	const std::string &file,
	const size_t line,
	...)
{
	Log::Entry entry;
	entry.Message = message.str();
	entry.File = file;
	entry.Line = line;

	va_list args;
	va_start(args, line);
	this->parseVarArgs(entry, args);
	va_end(args);

	this->_default_log->log(entry);
}

void LogManager::parseVarArgs(Log::Entry &entry, va_list &args)
{
	char *group_cstr = NULL;
	group_cstr = va_arg(args, char*);
	if (group_cstr)
	{
		entry.Group = group_cstr;
		entry.Level = static_cast<Log::Level::Type>(va_arg(args, uint32_t));
	}

	if ((entry.Level <= Log::Level::UNKNOWN)
		|| (entry.Level >= Log::Level::COUNT))
		entry.Level = Log::Level::NORMAL;
}
