set(bin_files ${bin_files}    ${CMAKE_CURRENT_SOURCE_DIR}/fix-paje-trace.sh
                              ${CMAKE_CURRENT_SOURCE_DIR}/generate-dwarf-functions
                              ${CMAKE_CURRENT_SOURCE_DIR}/normalize-pointers.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/sg_xml_unit_converter.py
                              ${CMAKE_CURRENT_SOURCE_DIR}/simgrid_update_xml.pl
                              ${CMAKE_CURRENT_SOURCE_DIR}/simgrid_convert_TI_traces.py            PARENT_SCOPE)

set(txt_files ${txt_files}    ${CMAKE_CURRENT_SOURCE_DIR}/pkg-config/simgrid.pc.in
                              ${CMAKE_CURRENT_SOURCE_DIR}/address_sanitizer.supp
                              ${CMAKE_CURRENT_SOURCE_DIR}/thread_sanitizer.supp
                              ${CMAKE_CURRENT_SOURCE_DIR}/simgrid.supp
                              ${CMAKE_CURRENT_SOURCE_DIR}/simgrid2vite.sed                        PARENT_SCOPE)
