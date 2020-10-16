.DEFAULT_GOAL=run
.PHONY=all

# ************************************************************************************** #

# DISCLAIMER:
#	This makefile purpose is mainly to test VTK versions or as developing stage use,
#   make sure to understand that it will mainly use CMake as build tool
#
#	If you simply want to build and run the program, it is recommended to rather use
#	CMake directly.
#
#	However, if you truly understand how this Makefile works, feel free to use it
#
# 	And just to tell you, "run" recipe will build everytime and does not look for change

# ************************************************************************************** #

#----------------------------------------------------------------------------------------#
#                                                                                        #
#                                      VTK Options                                       #
#                                                                                        #
#----------------------------------------------------------------------------------------#

VTK_PATH=/home/gautier/Downloads/vtk
VTK_VERSION=9.0.1

#----------------------------------------------------------------------------------------#
#                                                                                        #
#                                  Formatting Options                                    #
#                                                                                        #
#----------------------------------------------------------------------------------------#

CLANG_FORMAT_OPTIONS=--verbose -i --style=file
CLANG_FORMAT_FILES=src/*.{cpp,h}

#----------------------------------------------------------------------------------------#
#                                                                                        #
#                                        Recipes                                         #
#                                                                                        #
#----------------------------------------------------------------------------------------#

help: ## Show this help.
	@printf "\033[32m%-30s     \033[32m %s\n" "VARIABLE NAME" "DEFAULT_VALUE"
	@grep -E '^[a-zA-Z_-]+(\?=|=).*$$' $(MAKEFILE_LIST) |sort | awk 'BEGIN {FS = "(?=|=)"}; {printf "\033[36m%-30s     \033[0m %s\n", $$1, $$2}'
	@printf "\n\033[32m%-30s     \033[32m %s\033[0m\n" "RECIPE" "DESCRIPTION"
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "make \033[36m%-30s\033[0m %s\n", $$1, $$2}'

checkout: ## Checkout VTK branch
	@cd $(VTK_PATH); \
		git checkout tags/v$(VTK_VERSION) | sed -e 's/^/[vtk:checkout] /'

$(VTK_PATH)/build: checkout $(VTK_PATH) ## VTK Build
	@mkdir -p $(VTK_PATH)/build
	@cd $(VTK_PATH)/build; \
		git describe --tags | sed -e 's/^/[vtk:version] Now on: /'; \
		cmake ../ -Wno-dev | sed -e 's/^/[vtk:cmake] /'

deep-clean: clean ## Clean build files and VKT build
	rm -rf $(VTK_PATH)/build

clean: ## Clean build files
	rm -rf build

format: ## Format sources using clang-format configuration
	clang-format $(CLANG_FORMAT_OPTIONS) $(CLANG_FORMAT_FILES)

build: format $(VTK_PATH)/build ## Build the program
	@mkdir -p build
	@cd build; \
		cmake -DVTK_DIR:PATH=$(VTK_PATH) ../ -Wno-dev | sed -e 's/^/[build:cmake] /'; \
		make | sed -e 's/^/[build:make] /'

run: build ## Run the program
	./build/AtilaCalculatorSoftware | sed -e 's/^/[run] /'

test-run: clean build ## Try to build and run then kill the program (also print VTK version)
	@echo Starting...
	@timeout --preserve-status 2s ./build/AtilaCalculatorSoftware ; echo Done.
	@echo VTK_VERSION=$(VTK_VERSION)