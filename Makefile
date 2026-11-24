SHELL := cmd.exe

CXX := g++
BASE_CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -Iinclude
DEBUG_FLAGS := -DLOCAL_DEBUG
CXXFLAGS ?= $(BASE_CXXFLAGS) $(DEBUG_FLAGS)

# Automatically picks the first .cpp file in the root directory (falls back to any .cpp)
SRC_FILES := $(wildcard *.cpp)
SRC := $(if $(SRC_FILES),$(firstword $(SRC_FILES)),$(firstword $(wildcard **/*.cpp)))

TARGET := bin\main.exe
HEADERS := $(wildcard include/*.h) $(wildcard include/*.hpp) $(wildcard include/bits/*.h)

# Looks for any .in file in files/ first, then in the current directory
IN_CANDIDATES := $(wildcard files/*.in) $(wildcard *.in)
IN_FILE := $(firstword $(IN_CANDIDATES))

.PHONY: default run debug release clean

default: $(TARGET)

$(TARGET): $(SRC) $(HEADERS) | bin files
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

bin:
	@if not exist bin mkdir bin

files:
	@if not exist files mkdir files

run: default
	@echo.
ifneq ($(IN_FILE),)
	@$(TARGET)
	@echo ---- INPUT ($(subst /,\,$(IN_FILE))) ----
	@type $(subst /,\,$(IN_FILE))
	@echo.
	@echo ---- OUTPUT ----
	@if not exist files\*.out if not exist *.out echo -NO .OUT FILE GENERATED-
	@for %%f in (files\*.out *.out) do @if exist "%%f" (@echo [%%f] && @type "%%f" && @echo.)
else
	@echo ---- CONSOLE MODE ----
	@$(TARGET)
endif

debug:
	@$(MAKE) CXXFLAGS="$(BASE_CXXFLAGS) $(DEBUG_FLAGS)" run

release:
	@$(MAKE) CXXFLAGS="$(BASE_CXXFLAGS)" run

clean:
	@if exist bin rmdir /s /q bin
