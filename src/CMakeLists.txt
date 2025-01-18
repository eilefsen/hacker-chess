add_executable(${PROJECT_NAME}
		main.c validate.c move.c util.c
		pieces/pawn.c pieces/knight.c pieces/king.c pieces/rook.c pieces/bishop.c
)
set_target_properties(${PROJECT_NAME} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/
)
target_compile_options(
	${PROJECT_NAME}
	PRIVATE
	-O3 -Wall -Wextra -Wpedantic
	-Wshadow
)

