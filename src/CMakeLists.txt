add_executable(${PROJECT_NAME} main.c)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/)
target_compile_options(
	${PROJECT_NAME}
	PRIVATE
	-Og -Wall -Wextra -Wpedantic
	-Wshadow
)

