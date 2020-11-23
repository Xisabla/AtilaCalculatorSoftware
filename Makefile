.DEFAULT_GOAL=build

#----------------------------------------------------------------------------------------#
#                                                                                        #
#                                        Options                                         #
#                                                                                        #
#----------------------------------------------------------------------------------------#

CLANG_FORMAT_OPTIONS=--verbose -i --style=file
CLANG_FORMAT_FILES=src/*.{cpp,h}
CMAKE_FLAGS=-Wno-dev

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

.PHONY: check-boost
check-boost: ## Check for boost include file (assuming that all boost include files might be present)
ifeq (,$(wildcard /usr/include/boost/version.hpp))
$(error "boost installation not found")
endif

.PHONY: check-gidpost
check-gidpost: /usr/local/include/gidpost.h ## Check for gidpost include file

.PHONY: check-hdf5
check-hdf5: /usr/include/hdf5.h ## Check for hdf5 include file

.PHONY: check-vtk
check-vtk: /usr/include/vtk/vtk_hdf5.h ## Check for vtk hdf5 include file (assuming that all vtk include files might be present)

.PHONY: check-zlib
check-zlib: /usr/include/zlib.h ## Check for zlib include file

.PHONY: check-dependencies ## Check for needed dependencies
check-dependencies: check-boost check-gidpost check-hdf5 check-vtk check-zlib

.PHONY: clean
clean: ## Clean build files
	rm -rf build
	rm -rf doc

.PHONY: doc
doc:  ## Generate documentation using Doxygen
	doxygen ./Doxyfile

.PHONY: format
format: ## Format sources using clang-format
	clang-format $(CLANG_FORMAT_OPTIONS) $(CLANG_FORMAT_FILES)

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