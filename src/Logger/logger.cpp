#include "fmt/base.h"
#include "logger.h"
log_handler g_log_handler;

file_sink::~file_sink() {
  out.close();
  g_log_handler.remove_file_sink(this->file_sink_path);
}

uint64_t get_current_time_ms() {
  auto now = std::chrono::system_clock::now();
  auto duration = now.time_since_epoch();
  return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

void *thread_start(void *) {
  g_log_handler.consumer_loop();
  return nullptr;
}

void start_log_consumer_thread() {
  auto did_fail = pthread_create(&g_log_handler.handle, NULL, thread_start, nullptr);
}
void log_ext(const std::string &func, int line, sink_handle_t sink, LOGLEVEL level, std::string &&message) {
  g_log_handler.add_logmessage(std::move(fmt::format("{}({}): {}", func, line, message)), level, sink);
}



