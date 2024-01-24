# GNU Make workspace makefile autogenerated by Premake

.NOTPARALLEL:

ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),debug)
  GLFW_config = debug
  Glad_config = debug
  ImGui_config = debug
  yaml_cpp_config = debug
  lua_config = debug
  Turtle_config = debug
  Tortuga_config = debug
endif
ifeq ($(config),release)
  GLFW_config = release
  Glad_config = release
  ImGui_config = release
  yaml_cpp_config = release
  lua_config = release
  Turtle_config = release
  Tortuga_config = release
endif
ifeq ($(config),dist)
  GLFW_config = dist
  Glad_config = dist
  ImGui_config = dist
  yaml_cpp_config = dist
  lua_config = dist
  Turtle_config = dist
  Tortuga_config = dist
endif

PROJECTS := GLFW Glad ImGui yaml-cpp lua Turtle Tortuga

.PHONY: all clean help $(PROJECTS) Dependencies

all: $(PROJECTS)

Dependencies: GLFW Glad ImGui lua yaml-cpp

GLFW:
ifneq (,$(GLFW_config))
	@echo "==== Building GLFW ($(GLFW_config)) ===="
	@${MAKE} --no-print-directory -C Turtle/vendor/GLFW -f Makefile config=$(GLFW_config)
endif

Glad:
ifneq (,$(Glad_config))
	@echo "==== Building Glad ($(Glad_config)) ===="
	@${MAKE} --no-print-directory -C Turtle/vendor/Glad -f Makefile config=$(Glad_config)
endif

ImGui:
ifneq (,$(ImGui_config))
	@echo "==== Building ImGui ($(ImGui_config)) ===="
	@${MAKE} --no-print-directory -C Turtle/vendor/imgui -f Makefile config=$(ImGui_config)
endif

yaml-cpp:
ifneq (,$(yaml_cpp_config))
	@echo "==== Building yaml-cpp ($(yaml_cpp_config)) ===="
	@${MAKE} --no-print-directory -C Turtle/vendor/yaml-cpp -f Makefile config=$(yaml_cpp_config)
endif

lua:
ifneq (,$(lua_config))
	@echo "==== Building lua ($(lua_config)) ===="
	@${MAKE} --no-print-directory -C Turtle/vendor/lua -f Makefile config=$(lua_config)
endif

Turtle: GLFW Glad ImGui yaml-cpp lua
ifneq (,$(Turtle_config))
	@echo "==== Building Turtle ($(Turtle_config)) ===="
	@${MAKE} --no-print-directory -C Turtle -f Makefile config=$(Turtle_config)
endif

Tortuga: Turtle GLFW Glad ImGui yaml-cpp lua
ifneq (,$(Tortuga_config))
	@echo "==== Building Tortuga ($(Tortuga_config)) ===="
	@${MAKE} --no-print-directory -C Tortuga -f Makefile config=$(Tortuga_config)
endif

clean:
	@${MAKE} --no-print-directory -C Turtle/vendor/GLFW -f Makefile clean
	@${MAKE} --no-print-directory -C Turtle/vendor/Glad -f Makefile clean
	@${MAKE} --no-print-directory -C Turtle/vendor/imgui -f Makefile clean
	@${MAKE} --no-print-directory -C Turtle/vendor/yaml-cpp -f Makefile clean
	@${MAKE} --no-print-directory -C Turtle/vendor/lua -f Makefile clean
	@${MAKE} --no-print-directory -C Turtle -f Makefile clean
	@${MAKE} --no-print-directory -C Tortuga -f Makefile clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  debug"
	@echo "  release"
	@echo "  dist"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   GLFW"
	@echo "   Glad"
	@echo "   ImGui"
	@echo "   yaml-cpp"
	@echo "   lua"
	@echo "   Turtle"
	@echo "   Tortuga"
	@echo ""
	@echo "For more information, see https://github.com/premake/premake-core/wiki"