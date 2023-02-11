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

        virtual const char* what() const throw();

    private:
        static std::ostringstream cnvt;
    };

    class unknown_format_error : public std::runtime_error {
    public:

        unknown_format_error(std::string msg);

        virtual const char* what() const throw();

    private:
        static std::ostringstream cnvt;
    };
}