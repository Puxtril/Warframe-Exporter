#include "ExporterExceptions.h"

using namespace WarframeExporter;

not_imeplemented_error::not_imeplemented_error(std::string msg)
    : runtime_error(msg)
{}


std::ostringstream not_imeplemented_error::cnvt;
const char* not_imeplemented_error::what() const throw()
{
    return runtime_error::what();
}

unknown_format_error::unknown_format_error(std::string msg)
    : runtime_error(msg)
{}

std::ostringstream unknown_format_error::cnvt;
const char* unknown_format_error::what() const throw()
{
    return runtime_error::what();
}