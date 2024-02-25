#pragma once

#include <tclap/StdOutput.h>

class ExporterStdOutput : public TCLAP::StdOutput
{
    public:
		virtual void failure(TCLAP::CmdLineInterface& c, TCLAP::ArgException& e)
		{ 
			std::string progName = c.getProgramName();

            std::cerr << "PARSE ERROR: " << e.argId() << std::endl
                    << "             " << e.error() << std::endl
                    << std::endl;

            std::cerr << std::endl
                    << "For complete usage and help type: " << std::endl
                    << "   " << progName << " " << TCLAP::Arg::nameStartString()
                    << "help" << std::endl
                    << std::endl;

            throw TCLAP::ExitException(1);
		}

		virtual void usage(TCLAP::CmdLineInterface& c)
		{
            std::cout << std::endl;
            std::cout << "For an introduction into..." << std::endl;
            std::cout << " - How to use this tool" << std::endl;
            std::cout << " - Why it's necessary to use this instead of Ninjaripper" << std::endl;
            std::cout << " - How to handle the output of this program" << std::endl << std::endl;
            std::cout << "Please review this guide" << std::endl;
            std::cout << "https://docs.google.com/document/d/1ZzQ71NH2dVp7S-1fMUmVTezXFAKWgFRf0RodDqS6rUg" << std::endl << std::endl;

            std::cout << "------------------------------------------------------------------------------" << std::endl;

            std::cout << "All options:" << std::endl << std::endl;

			_longUsage(c, std::cout);
		}
};