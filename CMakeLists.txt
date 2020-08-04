if (CONFIG_GEL_COMPONENTS)
idf_component_register(
    SRC_DIRS ${CONFIG_GEL_COMPONENTS}
    INCLUDE_DIRS "." "gel" ${CONFIG_GEL_CONF_PATH}
    )
else()
idf_component_register(INCLUDE_DIRS "." "gel" ${CONFIG_GEL_CONF_PATH})
endif()

