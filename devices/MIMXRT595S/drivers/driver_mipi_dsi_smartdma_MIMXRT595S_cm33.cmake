include_guard()
message("driver_mipi_dsi_smartdma component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/fsl_mipi_dsi_smartdma.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.
)


include(driver_lpc_smartdma_MIMXRT595S_cm33)

include(driver_mipi_dsi_MIMXRT595S_cm33)

