CXXFLAGS+=-std=c++17
CXXFLAGS+=-fsanitize=address
CXXFLAGS+=-O3
CXXFLAGS+=-Wall
CXXFLAGS+=-pedantic
CXXFLAGS+=-I/opt/local/include
#CXXFLAGS+=-g
## Silence linker warnings about boost having different visibility settings
CXXFLAGS+=-fvisibility=hidden

LDFLAGS+=-L/opt/local/lib
#LDFLAGS+=-lboost_filesystem-mt
LDFLAGS+=-ltbb

BINARIES=test

all: $(BINARIES)

clean:
	rm -f $(BINARIES)

#/opt/local/lib/libboost_atomic-mt.dylib
#/opt/local/lib/libboost_chrono-mt.dylib
#/opt/local/lib/libboost_container-mt.dylib
#/opt/local/lib/libboost_context-mt.dylib
#/opt/local/lib/libboost_contract-mt.dylib
#/opt/local/lib/libboost_coroutine-mt.dylib
#/opt/local/lib/libboost_date_time-mt.dylib
#/opt/local/lib/libboost_exception-mt.a
#/opt/local/lib/libboost_fiber-mt.dylib
#/opt/local/lib/libboost_filesystem-mt.dylib
#/opt/local/lib/libboost_graph-mt.dylib
#/opt/local/lib/libboost_iostreams-mt.dylib
#/opt/local/lib/libboost_locale-mt.dylib
#/opt/local/lib/libboost_log-mt.dylib
#/opt/local/lib/libboost_log_setup-mt.dylib
#/opt/local/lib/libboost_math_c99-mt.dylib
#/opt/local/lib/libboost_math_c99f-mt.dylib
#/opt/local/lib/libboost_math_c99l-mt.dylib
#/opt/local/lib/libboost_math_tr1-mt.dylib
#/opt/local/lib/libboost_math_tr1f-mt.dylib
#/opt/local/lib/libboost_math_tr1l-mt.dylib
#/opt/local/lib/libboost_prg_exec_monitor-mt.dylib
#/opt/local/lib/libboost_program_options-mt.dylib
#/opt/local/lib/libboost_python27-mt.dylib
#/opt/local/lib/libboost_random-mt.dylib
#/opt/local/lib/libboost_regex-mt.dylib
#/opt/local/lib/libboost_serialization-mt.dylib
#/opt/local/lib/libboost_stacktrace_addr2line-mt.dylib
#/opt/local/lib/libboost_stacktrace_basic-mt.dylib
#/opt/local/lib/libboost_stacktrace_noop-mt.dylib
#/opt/local/lib/libboost_system-mt.dylib
#/opt/local/lib/libboost_test_exec_monitor-mt.a
#/opt/local/lib/libboost_thread-mt.dylib
#/opt/local/lib/libboost_timer-mt.dylib
#/opt/local/lib/libboost_type_erasure-mt.dylib
#/opt/local/lib/libboost_unit_test_framework-mt.dylib
#/opt/local/lib/libboost_wave-mt.dylib
#/opt/local/lib/libboost_wserialization-mt.dylib
