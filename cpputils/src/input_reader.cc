#include "input_reader.hh"

#include <curl/curl.h>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <string>

#include "command_line_args.hh"
#include "config.hh"

InputReader::InputReader(int day, int year, bool useExample) {

  _inputFilePath =
      std::format("{}/year{}_d{}", INPUT_LOCAL_CACHE, year, day);
  if (useExample) {
    // use the example file
    _inputFilePath += ".example";
  }

  // download the input file if it is not present locally
  if (std::filesystem::exists(_inputFilePath)) {
    if (useExample) {
      std::cout << "Using example input file.." << std::endl;
    } else {
      std::cout << "Using input file from local cache.." << std::endl;
    }
  } else {
    if (useExample) {
      // Example file has to be manually copied
      throw std::invalid_argument("example file '" + _inputFilePath +
                                  "' not found");
    }
    std::string url = std::format("https://adventofcode.com/{}/day/{}/input",
                                  year, day);
    downloadInput(url);
  }
}

InputReader::InputReader(int day, int year, AocCmdLineArgs *args)
    : InputReader(day, year, args->useExampleInput()) {}

void InputReader::downloadInput(const std::string &url) {
  // input file doesn't exists yet - download it using curl
  CURL *curl = curl_easy_init();
  if (curl == nullptr) {
    throw std::runtime_error("failed to initialize libcurl");
  }

  // open the file to write the input
  std::ofstream inputFile(_inputFilePath);
  if (!inputFile.is_open()) {
    // file is not open
    throw std::runtime_error("failed to create a new local input file");
  }

  // Set the URL, response string
  std::string response;
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &inputFile);

  // Set the AOC token via header
  struct curl_slist *headers = NULL;
  std::string sessionToken =
      std::format("Cookie: session={}", AOC_SESSION_TOKEN);
  headers = curl_slist_append(headers, sessionToken.c_str());
  headers =
      curl_slist_append(headers, "User-Agent: github.com/lkshminarayanan");
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

  // Set the write callback function that writes into the file
  curl_easy_setopt(
      curl, CURLOPT_WRITEFUNCTION,
      // +[] returns a C style function pointer
      +[](void *contents, size_t size, size_t nmemb,
          std::ofstream *fs) -> size_t {
        size_t totalSize = size * nmemb;
        fs->write(static_cast<char *>(contents), totalSize);
        return totalSize;
      });

  // Perform the request
  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    throw std::runtime_error("curl_easy_perform() failed: " +
                             std::string(curl_easy_strerror(res)));
  }

  // Clean up
  inputFile.close();
  curl_easy_cleanup(curl);

  std::cout << "Downloaded input file from " << url << std::endl;
}

std::string InputReader::readAll() {
  constexpr auto readSize = std::size_t(4096);
  auto stream = std::ifstream(_inputFilePath.data());
  stream.exceptions(std::ios_base::badbit);

  if (not stream) {
    throw std::ios_base::failure("failed to open the input file at '" +
                                 _inputFilePath + "'");
  }

  auto out = std::string();
  auto buf = std::string(readSize, '\0');
  while (stream.read(&buf[0], readSize)) {
    out.append(buf, 0, stream.gcount());
  }
  out.append(buf, 0, stream.gcount());
  return out;
}

std::vector<std::string> InputReader::readAllAsVectorOfStrings() {
  std::vector<std::string> input;
  for (auto it = begin(); it != end(); ++it) {
    input.push_back(*it);
  }

  return std::move(input);
}
