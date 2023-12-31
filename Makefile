#******************************************************************************
#
#   MAKEFILE template
#   Summer 2023
#
#   Copyright (c) 2023-2030 Caiuby Freitas
#
#   This software is provided "as-is", without any express or implied warranty.
#   In no event will the author be held liable for any damages arising from the 
#   use of this software.
#
#   Permission is granted to anyone to use this software for any purpose, 
#   including commercial applications, and to alter it and redistribute it 
#   freely, subject to the following restrictions:
#
#    1. The origin of this software must not be misrepresented; you must not 
#       claim that you wrote the original software. If you use this software
#       in a product, an acknowledgment in the product documentation would 
#       be appreciated but is not required.
#
#    2. Altered source versions must be plainly marked as such, and must not 
#       be misrepresented as being the original software.
#
#    3. This notice may not be removed or altered from any source distribution.
#
#   The application folder structure must be setup as indicated below.
# 
#   ROOT
#    +---- include	; app include files folder
#    +---- src		; app source code folder
#    +---- external	; root folder for other libraries linked to the app
#    +---- bin		; app executable target folder
#
#******************************************************************************

.PHONY: all clean $(APPNAME) release


# Application executable file name.
APPNAME := test.prg


# Base folder structure
DIR_BASE		:= ./
DIR_SOURCE		:= $(addprefix $(DIR_BASE),src)
DIR_INCLUDE		:= $(addprefix $(DIR_BASE),include)
DIR_BINARY		:= $(addprefix $(DIR_BASE),bin)
DIR_EXTERNAL		:= $(addprefix $(DIR_BASE),external)
REQUIRED_FOLDERS	:= src include bin external


# Define default C compiler, archiver to pack library and compiler flags
CC		 	:= g++
AR 			:= ar
CFLAGS 			:= -std=c++11 -Wall -Werror -Wpedantic -lGL -lm -lpthread -ldl -lrt -lX11


#*****************************************************************************
#   DEPENDENCIES
#   Static libraries in the format ‘-lname’. Make will search for LIBPATTERNS (‘lib%.so lib%.a’)


REQUIRED_LIBRARIES 	:= 	mylib \
				raylib 
INCLUDE_PATH		:=	$(DIR_EXTERNAL)/mylib \
				$(DIR_EXTERNAL)/raylib \
				/usr/local/include					
INCLUDE_PATH		:=	$(addprefix -I, $(INCLUDE_PATH))
LIBRARY_PATH		:= 	$(DIR_EXTERNAL)/mylib \
				$(DIR_EXTERNAL)/raylib 
LIBRARY_PATH		:= 	$(addprefix -L, $(LIBRARY_PATH)) \
				-L/usr \
				-L/usr/local/lib


#*****************************************************************************
#   RECIPES

all: $(APPNAME)

# Set main target and standard folder structure as order-only prerequisites
$(APPNAME): | clean $(REQUIRED_FOLDERS) main.o


# @ at the beginning will not print the command being executed
clean:
	$(info Cleaning up)
	@rm -f ./$(DIR_BASE)/*.o  
	@rm -f ./$(DIR_BASE)/*.a
	@rm -f ./$(DIR_BINARY)/*.*  


# Ensure stardard folders are in place as pre-requisite to build
$(REQUIRED_FOLDERS): 
	$(info Checking folder structure)
	@mkdir -p $(DIR_SOURCE)
	@mkdir -p $(DIR_INCLUDE)
	@mkdir -p $(DIR_BINARY)
	@mkdir -p $(DIR_EXTERNAL)


# Main application
main.o: | $(REQUIRED_LIBRARIES) 
	$(info Compiling the application)
	$(CC) -g \
		$(DIR_SOURCE)/main.cpp \
		$(INCLUDE_PATH) \
		-o $(DIR_BINARY)/$(APPNAME) \
		$(LIBRARY_PATH) $(addprefix -l, $(REQUIRED_LIBRARIES)) \
		$(CFLAGS)
	@echo

# Compiled on the fly
mylib:
	$(info Compiling dependencies)
	$(CC) -c \
		$(DIR_EXTERNAL)/mylib/*.cpp \
		$(CFLAGS)
	$(AR) rcs \
		$(DIR_EXTERNAL)/mylib/libmylib.a \
		*.o
	@rm $(DIR_BASE)/*.o 

raylib: ;

showchanges:
	@find ./src -mtime -1
	@find ./include -mtime -1
