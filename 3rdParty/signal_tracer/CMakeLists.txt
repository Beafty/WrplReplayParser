# from test/signal_tracer.cpp
# and https://github.com/jeremy-rifkin/cpptrace/blob/91b6b78e408a8b1c0b7146c9034a03156c082da2/docs/signal-safe-tracing.md

add_executable(signal_tracer signal_tracer.cpp)
target_link_libraries(signal_tracer cpptrace::cpptrace)