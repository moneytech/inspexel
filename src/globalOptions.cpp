#include "globalOptions.h"

#include <string>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

auto listDeviceFiles(std::vector<std::string> const& _str) -> std::pair<bool, std::set<std::string>> {
	std::set<std::string> res;
	std::vector<fs::path> passedPaths;
	for (auto const& s : _str) {
		try {
			passedPaths.emplace_back(fs::canonical(fs::path(s)));
		} catch (...) {} // ignore invalid paths
	}
	if (fs::is_directory("/dev/serial/by-id/")) {
		for (auto p : fs::directory_iterator("/dev/serial/by-id/")) {
			// if p resolves to anything specifies by _str remove it
			auto canonical = fs::canonical(p);
			if (std::find(passedPaths.begin(), passedPaths.end(), p) == passedPaths.end() and
				std::find(passedPaths.begin(), passedPaths.end(), canonical) == passedPaths.end()) {
				res.insert(p.path().string());
				res.insert(canonical.string());
			}
		}
	}

	return {not _str.empty(), res};
}
auto listTypicalBaudrates(std::vector<std::string> const& _str) -> std::pair<bool, std::set<std::string>> {
	std::set<std::string> res;
	res.emplace("9600");
	res.emplace("11520");
	res.emplace("57142");
	res.emplace("1m");
	res.emplace("2m");
	res.emplace("3m");
	for (auto const& s : _str) {
		if (res.count(s) > 0) {
			res.erase(s);
		}
	}

	return {true, res};
}


auto getDefaultSerialPort() -> std::string {
	auto devices = listDeviceFiles({}).second;
	if (devices.empty()) {
		return "";
	}
	return *devices.begin();
}

