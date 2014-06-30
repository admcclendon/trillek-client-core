#ifndef LOGGING_HPP_INCLUDED
#define LOGGING_HPP_INCLUDED

#include "trillek.hpp"
#include "type-id.hpp"
#include <memory>

#define LOGMSG(ll) trillek::Logging::Log<trillek::L_##ll, trillek::Logging>()
#define LOGMSGFOR(ll,x) trillek::Logging::Log<trillek::L_##ll, x>()
#define LOGMSGC(ll) trillek::Logging::Log<trillek::L_##ll>(*this)

namespace trillek {
enum LogLevel : int {
    L_FATAL,
    L_ERROR,
    L_WARNING,
    L_NOTICE,
    L_INFO,
    L_DEBUG,
    L_DEBUG_FINE
};
template<LogLevel T> std::string LogLevelString() { return "LOG"; }
template<> inline std::string LogLevelString<L_FATAL>() { return "FATAL"; }
template<> inline std::string LogLevelString<L_ERROR>() { return "ERROR"; }
template<> inline std::string LogLevelString<L_WARNING>() { return "WARNING"; }
template<> inline std::string LogLevelString<L_NOTICE>() { return "NOTICE"; }
template<> inline std::string LogLevelString<L_INFO>() { return "INFO"; }
template<> inline std::string LogLevelString<L_DEBUG>() { return "DEBUG"; }
template<> inline std::string LogLevelString<L_DEBUG_FINE>() { return "FINE"; }

class Logging {
private:
    Logging();
    static std::unique_ptr<Logging> instance;
    static Logging& GetInstance();
public:

    void WriteLine(std::string l, std::string message);
    void WriteLine(std::string l, std::string section, std::string message);

    template<LogLevel T>
    class LogLine {
    public:
        LogLine() : show(true), glob(true) { }
        LogLine(std::string system) : show(true), glob(false), systemname(system) { }
        LogLine(const LogLine&) = delete;
        LogLine& operator=(const LogLine&) = delete;
        LogLine& operator=(LogLine&&) = delete;
        LogLine(LogLine&& that) : show(true) {
            that.show = false;
            glob = that.glob;
            systemname = std::move(that.systemname);
            message = std::move(that.message);
        }
        ~LogLine() {
            if(show) {
                if(glob) {
                    GetInstance().WriteLine(LogLevelString<T>(), message);
                }
                else {
                    GetInstance().WriteLine(LogLevelString<T>(), systemname, message);
                }
            }
        }
        LogLine& operator<<(std::string m) {
            message.append(m);
            return *this;
        }
        LogLine& operator<<(long m) {
            message.append(std::to_string(m));
            return *this;
        }
    private:
        bool show, glob;
        std::string systemname;
        std::string message;
    };

    template<LogLevel L, typename SYSTEM>
    static LogLine<L> Log() {
        if(reflection::GetTypeID<SYSTEM>() == ~0) {
            return LogLine<L>();
        }
        else {
            return LogLine<L>(reflection::GetTypeName<SYSTEM>());
        }
    }
    template<LogLevel L, typename SYSTEM>
    static LogLine<L> Log(const SYSTEM&) {
        if(reflection::GetTypeID<SYSTEM>() == ~0) {
            return LogLine<L>();
        }
        else {
            return LogLine<L>(reflection::GetTypeName<SYSTEM>());
        }
    }
};

} // namespace trillek

#endif
