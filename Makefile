.DEFAULT_GOAL=build

#----------------------------------------------------------------------------------------#
#                                                                                        #
#                                        Options                                         #
#                                                                                        #
#----------------------------------------------------------------------------------------#

CLANG_FORMAT_OPTIONS=--verbose -i --style=file
CLANG_FORMAT_FILES=$(wildcard ./include/**/**.h ./include/**.h ./src/**/**.cpp ./src/**.cpp)
CMAKE_FLAGS=-Wno-dev
CPP_CHECK_FLAGS=--enable=all --quiet --suppress=unusedFunction --suppress=copyCtorPointerCopying

#----------------------------------------------------------------------------------------#
#                                                                                        #
#                                        Recipes                                         #
#                                                                                        #
#----------------------------------------------------------------------------------------#

.PHONY: help
help: ## Show this help.
	@printf "\033[32m%-30s     \033[32m %s\n" "VARIABLE NAME" "DEFAULT_VALUE"
	@grep -E '^[a-zA-Z_-]+(\?=|=).*$$' $(MAKEFILE_LIST) |sort | awk 'BEGIN {FS = "(?=|=)"}; {printf "\033[36m%-30s     \033[0m %s\n", $$1, $$2}'
	@printf "\n\033[32m%-30s     \033[32m %s\033[0m\n" "RECIPE" "DESCRIPTION"
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "make \033[36m%-30s\033[0m %s\n", $$1, $$2}'

.PHONY: check-dependencies ## Check for needed dependencies
check-dependencies:
	@./scripts/check-includable.sh boost/version.hpp gidpost.h hdf5.h vtk_hdf5.h zlib.h && \
		echo "Dependencies check ok"

.PHONY: clean
clean: ## Clean build files
	rm -rf build
	rm -rf doc

.PHONY: doc
doc: ## Generate documentation using Doxygen
	doxygen ./Doxyfile

.PHONY: format
format: ## Format sources using clang-format
	@./scripts/fix-new-line.sh $(CLANG_FORMAT_FILES)
	@clang-format $(CLANG_FORMAT_OPTIONS) $(CLANG_FORMAT_FILES)

.PHONY: check
check: ## Run ccp-check code analysis
	@mkdir -p build
	@cd build; \
		cmake $(CMAKE_FLAGS) -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ../ | tail -n +90; \
		cppcheck --project=compile_commands.json $(CPP_CHECK_FLAGS)

./build/AtilaCalculatorSoftware: check-dependencies
	@mkdir -p build
	@cd build; \
		cmake $(CMAKE_FLAGS) ../; \
		make

.PHONY: build
build: ./build/AtilaCalculatorSoftware ## Build the program

.PHONY: run
run: ./build/AtilaCalculatorSoftware ## Run the program
	./build/AtilaCalculatorSoftware
