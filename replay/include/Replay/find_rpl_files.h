

#ifndef MYEXTENSION_FIND_RPL_FILES_H
#define MYEXTENSION_FIND_RPL_FILES_H

#include <iostream>
#include <filesystem>
#include <regex>
#include <vector>

namespace fs = std::filesystem;

struct FileWithTime {
  fs::path path;
  std::filesystem::file_time_type mod_time;
};

std::vector<fs::path> find_wt_replays(const fs::path& directory) {
  std::vector<FileWithTime> matches;
  std::regex pattern(R"(^wt_replay_.*)"); // Starts with wt_replay_a followed by digits

  fs::directory_options options = fs::directory_options::skip_permission_denied;

  try {
    auto iterator = fs::directory_iterator(directory, options);

    for (const auto& entry : iterator) {
      if (entry.is_regular_file()) {
        const std::string filename = entry.path().filename().string();
        if (std::regex_search(filename, pattern)) {
          auto mod_time = fs::last_write_time(entry);
          matches.push_back({entry.path(), mod_time});
        }
      }
    }
  } catch (const fs::filesystem_error& e) {
    std::cerr << "Filesystem error: " << e.what() << '\n';
  }

  // Sort by most recent modification time (descending)
  std::sort(matches.begin(), matches.end(), [](const FileWithTime& a, const FileWithTime& b) {
    return a.mod_time > b.mod_time;
  });

  // Return only the paths
  std::vector<fs::path> sorted_paths;
  for (const auto& f : matches) {
    sorted_paths.push_back(f.path);
  }

  return sorted_paths;
}

#endif //MYEXTENSION_FIND_RPL_FILES_H
