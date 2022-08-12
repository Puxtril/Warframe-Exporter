#include "ExporterExceptions.h"

using namespace WarframeExporter;

not_imeplemented_error::not_imeplemented_error(std::string msg)
    : runtime_error(msg), debugCount(0)
{}

not_imeplemented_error::not_imeplemented_error(std::string msg, std::string debug)
    : runtime_error(msg), debugCount(1)
{
    debugs[0] = debug;
}

not_imeplemented_error::not_imeplemented_error(std::string msg, std::string debug1, std::string debug2)
    : runtime_error(msg), debugCount(2)
{
    debugs[0] = debug1;
    debugs[1] = debug2;
}

not_imeplemented_error::not_imeplemented_error(std::string msg, std::string debug1, std::string debug2, std::string debug3)
    : runtime_error(msg), debugCount(3)
{
    debugs[0] = debug1;
    debugs[1] = debug2;
    debugs[2] = debug3;
}

not_imeplemented_error::not_imeplemented_error(std::string msg, std::string debug1, std::string debug2, std::string debug3, std::string debug4)
    : runtime_error(msg), debugCount(4)
{
    debugs[0] = debug1;
    debugs[1] = debug2;
    debugs[2] = debug3;
    debugs[3] = debug4;
}

not_imeplemented_error::not_imeplemented_error(std::string msg, std::string debug1, std::string debug2, std::string debug3, std::string debug4, std::string debug5)
    : runtime_error(msg), debugCount(5)
{
    debugs[0] = debug1;
    debugs[1] = debug2;
    debugs[2] = debug3;
    debugs[3] = debug4;
    debugs[4] = debug5;
}

std::ostringstream not_imeplemented_error::cnvt;
const char* not_imeplemented_error::what() const throw()
{
    cnvt.str("");
    cnvt << runtime_error::what();
    return cnvt.str().c_str();
}

int
not_imeplemented_error::getDebugCount()
{
    return debugCount;
}

const std::string*
not_imeplemented_error::getDebugs()
{
    return debugs;
}



unknown_format_error::unknown_format_error(std::string msg)
    : runtime_error(msg)
{}

unknown_format_error::unknown_format_error(std::string msg, std::string debug)
    : runtime_error(msg), debugCount(1)
{
    debugs[0] = debug;
}

unknown_format_error::unknown_format_error(std::string msg, std::string debug1, std::string debug2)
    : runtime_error(msg), debugCount(2)
{
    debugs[0] = debug1;
    debugs[1] = debug2;
}

unknown_format_error::unknown_format_error(std::string msg, std::string debug1, std::string debug2, std::string debug3)
    : runtime_error(msg), debugCount(3)
{
    debugs[0] = debug1;
    debugs[1] = debug2;
    debugs[2] = debug3;
}

unknown_format_error::unknown_format_error(std::string msg, std::string debug1, std::string debug2, std::string debug3, std::string debug4)
    : runtime_error(msg), debugCount(4)
{
    debugs[0] = debug1;
    debugs[1] = debug2;
    debugs[2] = debug3;
    debugs[3] = debug4;
}

unknown_format_error::unknown_format_error(std::string msg, std::string debug1, std::string debug2, std::string debug3, std::string debug4, std::string debug5)
    : runtime_error(msg), debugCount(5)
{
    debugs[0] = debug1;
    debugs[1] = debug2;
    debugs[2] = debug3;
    debugs[3] = debug4;
    debugs[4] = debug5;
}

std::ostringstream unknown_format_error::cnvt;
const char* unknown_format_error::what() const throw()
{
    cnvt.str("");
    cnvt << runtime_error::what();
    return cnvt.str().c_str();
}

int
unknown_format_error::getDebugCount()
{
    return debugCount;
}
 
const std::string*
unknown_format_error::getDebugs()
{
    return debugs;
}