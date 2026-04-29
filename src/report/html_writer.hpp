#pragma once

#include <string>
#include <vector>
#include "core/result.hpp"

namespace sps::report {

class HtmlWriter {
public:
    static std::string write(const std::vector<core::ScanResult>& results);
};

} // namespace sps::report
