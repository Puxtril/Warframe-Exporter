#pragma once

#include <exception>
#include <stdexcept>
#include <sstream>
#include <string>

namespace WarframeExporter
{
    class not_imeplemented_error : public std::runtime_error {
    public:

        not_imeplemented_error(std::string msg);
        not_imeplemented_error(std::string msg, std::string debug);
        not_imeplemented_error(std::string msg, std::string debug1, std::string debug2);
        not_imeplemented_error(std::string msg, std::string debug1, std::string debug2, std::string debug3);
        not_imeplemented_error(std::string msg, std::string debug1, std::string debug2, std::string debug3, std::string debug4);
        not_imeplemented_error(std::string msg, std::string debug1, std::string debug2, std::string debug3, std::string debug4, std::string debug5);

        virtual const char* what() const throw();
        int getDebugCount();
        const std::string* getDebugs();

    private:
        static std::ostringstream cnvt;
        int debugCount;
        std::string debugs[5];
    };

    class unknown_format_error : public std::runtime_error {
    public:

        unknown_format_error(std::string msg);
        unknown_format_error(std::string msg, std::string debug);
        unknown_format_error(std::string msg, std::string debug1, std::string debug2);
        unknown_format_error(std::string msg, std::string debug1, std::string debug2, std::string debug3);
        unknown_format_error(std::string msg, std::string debug1, std::string debug2, std::string debug3, std::string debug4);
        unknown_format_error(std::string msg, std::string debug1, std::string debug2, std::string debug3, std::string debug4, std::string debug5);

        virtual const char* what() const throw();
        int getDebugCount();
        const std::string* getDebugs();

    private:
        static std::ostringstream cnvt;
        int debugCount;
        std::string debugs[5];
    };
}