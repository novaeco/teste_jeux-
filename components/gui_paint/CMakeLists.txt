idf_component_register(SRCS "gui_bmp.c" "gui_paint.c" "ui_sprite.c"
                        INCLUDE_DIRS "."
                        REQUIRES fonts lovyangfx_port
                        )

set_source_files_properties(ui_sprite.c PROPERTIES LANGUAGE CXX)
