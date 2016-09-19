#pragma once

#include <ostream>
#include <vector>
#include <string>

void PrintFileHeader(std::ostream& os, const std::vector<std::string>& includes);

void PrintFileHeader(std::ostream& os);

void PrintFileFooter(std::ostream& os);

void PrintSectionHeader(std::ostream& os, const char* name);
