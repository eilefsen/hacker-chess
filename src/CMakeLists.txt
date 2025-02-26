option(DEBUG_PRINT, "Activates print debugging" OFF)
option(DEBUG_PRINT_CHECK, "Activates print debugging for check and checkmate" OFF)
option(DEBUG_PRINT_VALIDATE, "Activates print debugging for move validation" OFF)

if(DEBUG_PRINT)
	add_compile_definitions(DEBUG_PRINT)
endif()
if(DEBUG_PRINT_CHECK)
	add_compile_definitions(DEBUG_PRINT_CHECK)
endif()
if(DEBUG_PRINT_VALIDATE)
	add_compile_definitions(DEBUG_PRINT_VALIDATE)
endif()

add_executable(${PROJECT_NAME}
		main.c validate.c move.c util.c check.c
		pieces/pawn.c pieces/knight.c pieces/king.c pieces/rook.c
		pieces/bishop.c pieces/queen.c pieces/shared.c
)
set_target_properties(${PROJECT_NAME} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/
)
target_compile_options(
	${PROJECT_NAME}
	PRIVATE
	-Wall -Wextra -Wpedantic
	-Wshadow
)

