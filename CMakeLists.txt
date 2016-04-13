cmake_minimum_required (VERSION 2.8.11)

project (utf8rewind C)

add_library (utf8rewind
  source/internal/casemapping.c
  source/internal/codepoint.c
  source/internal/composition.c
  source/internal/database.c
  source/internal/decomposition.c
  source/internal/seeking.c
  source/internal/streaming.c
  source/unicodedatabase.c
  source/utf8rewind.c
)

target_include_directories (utf8rewind PUBLIC
  include/utf8rewind
)

set_target_properties (utf8rewind PROPERTIES
  POSITION_INDEPENDENT_CODE ON
)
