IF(NOT DEFINED FPU)  
    SET(FPU "-mfloat-abi=hard -mfpu=fpv5-sp-d16")  
ENDIF()  

IF(NOT DEFINED SPECS)  
    SET(SPECS "--specs=nano.specs --specs=nosys.specs")  
ENDIF()  

IF(NOT DEFINED DEBUG_CONSOLE_CONFIG)  
    SET(DEBUG_CONSOLE_CONFIG "-DSDK_DEBUGCONSOLE=1")  
ENDIF()  

SET(CMAKE_ASM_FLAGS_DEBUG " \
    ${CMAKE_ASM_FLAGS_DEBUG} \
    -DDEBUG \
    -D__STARTUP_CLEAR_BSS \
    -mcpu=cortex-m33 \
    -mthumb \
    ${FPU} \
")
SET(CMAKE_ASM_FLAGS_RELEASE " \
    ${CMAKE_ASM_FLAGS_RELEASE} \
    -DNDEBUG \
    -D__STARTUP_CLEAR_BSS \
    -mcpu=cortex-m33 \
    -mthumb \
    ${FPU} \
")
SET(CMAKE_C_FLAGS_DEBUG " \
    ${CMAKE_C_FLAGS_DEBUG} \
    -DDEBUG \
    -DCPU_MIMXRT595SFFOC_cm33 \
    -DBOOT_HEADER_ENABLE=1 \
    -DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1 \
    -DTFM_LVL=2 \
    -DTFM_SPM_LOG_LEVEL=TFM_SPM_LOG_LEVEL_INFO \
    -DTFM_SP_LOG_RAW_ENABLED \
    -DTFM_PARTITION_LOG_LEVEL=TFM_PARTITION_LOG_LEVEL_INFO \
    -DTFM_CRYPTO_IOVEC_BUFFER_SIZE=5120 \
    -DTFM_CRYPTO_ENGINE_BUF_SIZE=9600 \
    -DITS_MAX_ASSET_SIZE=512 \
    -DITS_NUM_ASSETS=5 \
    -DPS_MAX_ASSET_SIZE=512 \
    -DPS_NUM_ASSETS=5 \
    -DPS_RAM_FS \
    -DPS_CREATE_FLASH_LAYOUT \
    -DPS_WIPE_ALL \
    -DITS_RAM_FS \
    -DITS_CREATE_FLASH_LAYOUT \
    -DITS_WIPE_ALL \
    -DOTP_NV_COUNTERS_RAM_EMULATION \
    -DCONFIG_TFM_CONN_HANDLE_MAX_NUM=8 \
    -DDAUTH_CHIP_DEFAULT \
    -D__SEMIHOST_HARDFAULT_DISABLE \
    -DCRYPTO_HW_ACCELERATOR \
    -DTFM_PSA_API=1 \
    -DCONFIG_TFM_ENABLE_MEMORY_PROTECT \
    -DTFM_PARTITION_NS_AGENT_TZ \
    -DTFM_PARTITION_CRYPTO \
    -DPLATFORM_DEFAULT_CRYPTO_KEYS \
    -DMBEDTLS_PSA_CRYPTO_DRIVERS \
    -DMBEDTLS_PSA_CRYPTO_BUILTIN_KEYS \
    -DPSA_CRYPTO_DRIVER_TFM_BUILTIN_KEY \
    -DPSA_CRYPTO_DRIVER_TFM_BUILTIN_KEY_LOADER \
    -DTFM_PARTITION_INITIAL_ATTESTATION \
    -DT_COSE_USE_PSA_CRYPTO \
    -DTFM_PARTITION_INTERNAL_TRUSTED_STORAGE \
    -DTFM_PARTITION_PROTECTED_STORAGE \
    -DPS_ENCRYPTION \
    -DPS_ROLLBACK_PROTECTION \
    -DTFM_PARTITION_PLATFORM \
    -DTFM_CRYPTO_ASYM_ENCRYPT_MODULE_DISABLED \
    -DPS_CRYPTO_AEAD_ALG=PSA_ALG_CCM \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DCONFIG_TFM_BUILDING_SPE=1 \
    -DPLATFORM_DEFAULT_NV_COUNTERS \
    -DPLATFORM_DEFAULT_OTP \
    -DPLATFORM_DEFAULT_PROVISIONING \
    -DOTP_WRITEABLE \
    -DTFM_DUMMY_PROVISIONING \
    -DCONFIG_TFM_PSA_API_SUPERVISOR_CALL \
    -DCONFIG_TFM_PARTITION_META \
    -DCONFIG_TFM_FLOAT_ABI=2 \
    -DCONFIG_TFM_ENABLE_CP10CP11 \
    -DCONFIG_TFM_LAZY_STACKING \
    -DCONFIG_TFM_HALT_ON_CORE_PANIC \
    -DCONFIG_TFM_USE_TRUSTZONE \
    -DATTEST_TOKEN_PROFILE_PSA_IOT_1 \
    -DATTEST_KEY_BITS=256 \
    -DTFM_PARTITION_TEST_SECURE_SERVICES \
    -DMCUXPRESSO_SDK \
    -O1 \
    -g \
    -O0 \
    -mcmse \
    -Wno-unused-variable \
    -Wno-unused-value \
    -Wno-unused-function \
    -Wno-unused-but-set-variable \
    -Wno-return-type \
    -mcpu=cortex-m33 \
    -Wall \
    -mthumb \
    -MMD \
    -MP \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
    -fno-builtin \
    -mapcs \
    -std=gnu99 \
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_C_FLAGS_RELEASE " \
    ${CMAKE_C_FLAGS_RELEASE} \
    -DNDEBUG \
    -DCPU_MIMXRT595SFFOC_cm33 \
    -DBOOT_HEADER_ENABLE=1 \
    -DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1 \
    -DTFM_LVL=2 \
    -DTFM_SPM_LOG_LEVEL=TFM_SPM_LOG_LEVEL_INFO \
    -DTFM_SP_LOG_RAW_ENABLED \
    -DTFM_PARTITION_LOG_LEVEL=TFM_PARTITION_LOG_LEVEL_INFO \
    -DTFM_CRYPTO_IOVEC_BUFFER_SIZE=5120 \
    -DTFM_CRYPTO_ENGINE_BUF_SIZE=9600 \
    -DITS_MAX_ASSET_SIZE=512 \
    -DITS_NUM_ASSETS=5 \
    -DPS_MAX_ASSET_SIZE=512 \
    -DPS_NUM_ASSETS=5 \
    -DPS_RAM_FS \
    -DPS_CREATE_FLASH_LAYOUT \
    -DPS_WIPE_ALL \
    -DITS_RAM_FS \
    -DITS_CREATE_FLASH_LAYOUT \
    -DITS_WIPE_ALL \
    -DOTP_NV_COUNTERS_RAM_EMULATION \
    -DCONFIG_TFM_CONN_HANDLE_MAX_NUM=8 \
    -DDAUTH_CHIP_DEFAULT \
    -D__SEMIHOST_HARDFAULT_DISABLE \
    -DCRYPTO_HW_ACCELERATOR \
    -DTFM_PSA_API=1 \
    -DCONFIG_TFM_ENABLE_MEMORY_PROTECT \
    -DTFM_PARTITION_NS_AGENT_TZ \
    -DTFM_PARTITION_CRYPTO \
    -DPLATFORM_DEFAULT_CRYPTO_KEYS \
    -DMBEDTLS_PSA_CRYPTO_DRIVERS \
    -DMBEDTLS_PSA_CRYPTO_BUILTIN_KEYS \
    -DPSA_CRYPTO_DRIVER_TFM_BUILTIN_KEY \
    -DPSA_CRYPTO_DRIVER_TFM_BUILTIN_KEY_LOADER \
    -DTFM_PARTITION_INITIAL_ATTESTATION \
    -DT_COSE_USE_PSA_CRYPTO \
    -DTFM_PARTITION_INTERNAL_TRUSTED_STORAGE \
    -DTFM_PARTITION_PROTECTED_STORAGE \
    -DPS_ENCRYPTION \
    -DPS_ROLLBACK_PROTECTION \
    -DTFM_PARTITION_PLATFORM \
    -DTFM_CRYPTO_ASYM_ENCRYPT_MODULE_DISABLED \
    -DPS_CRYPTO_AEAD_ALG=PSA_ALG_CCM \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DCONFIG_TFM_BUILDING_SPE=1 \
    -DPLATFORM_DEFAULT_NV_COUNTERS \
    -DPLATFORM_DEFAULT_OTP \
    -DPLATFORM_DEFAULT_PROVISIONING \
    -DOTP_WRITEABLE \
    -DTFM_DUMMY_PROVISIONING \
    -DCONFIG_TFM_PSA_API_SUPERVISOR_CALL \
    -DCONFIG_TFM_PARTITION_META \
    -DCONFIG_TFM_FLOAT_ABI=2 \
    -DCONFIG_TFM_ENABLE_CP10CP11 \
    -DCONFIG_TFM_LAZY_STACKING \
    -DCONFIG_TFM_HALT_ON_CORE_PANIC \
    -DCONFIG_TFM_USE_TRUSTZONE \
    -DATTEST_TOKEN_PROFILE_PSA_IOT_1 \
    -DATTEST_KEY_BITS=256 \
    -DTFM_PARTITION_TEST_SECURE_SERVICES \
    -DMCUXPRESSO_SDK \
    -Os \
    -mcmse \
    -Wno-unused-variable \
    -Wno-unused-value \
    -Wno-unused-function \
    -Wno-unused-but-set-variable \
    -Wno-return-type \
    -mcpu=cortex-m33 \
    -Wall \
    -mthumb \
    -MMD \
    -MP \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
    -fno-builtin \
    -mapcs \
    -std=gnu99 \
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_CXX_FLAGS_DEBUG " \
    ${CMAKE_CXX_FLAGS_DEBUG} \
    -DDEBUG \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DMCUXPRESSO_SDK \
    -g \
    -O0 \
    -mcmse \
    -mcpu=cortex-m33 \
    -Wall \
    -mthumb \
    -MMD \
    -MP \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
    -fno-builtin \
    -mapcs \
    -fno-rtti \
    -fno-exceptions \
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_CXX_FLAGS_RELEASE " \
    ${CMAKE_CXX_FLAGS_RELEASE} \
    -DNDEBUG \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DMCUXPRESSO_SDK \
    -Os \
    -mcmse \
    -mcpu=cortex-m33 \
    -Wall \
    -mthumb \
    -MMD \
    -MP \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
    -fno-builtin \
    -mapcs \
    -fno-rtti \
    -fno-exceptions \
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_EXE_LINKER_FLAGS_DEBUG " \
    ${CMAKE_EXE_LINKER_FLAGS_DEBUG} \
    -Wl,--out-implib=./debug/tfm_psatest_s_CMSE_lib.o \
    -g \
    -Wl,--cmse-implib \
    -mcpu=cortex-m33 \
    -Wall \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
    -fno-builtin \
    -mthumb \
    -mapcs \
    -Xlinker \
    --gc-sections \
    -Xlinker \
    -static \
    -Xlinker \
    -z \
    -Xlinker \
    muldefs \
    -Xlinker \
    -Map=output.map \
    -Wl,--print-memory-usage \
    ${FPU} \
    ${SPECS} \
    -T${ProjDirPath}/../../../../../../middleware/tfm/tf-m/platform/ext/target/nxp/common/armgcc/tfm_common_s_pre.ld -static \
")
SET(CMAKE_EXE_LINKER_FLAGS_RELEASE " \
    ${CMAKE_EXE_LINKER_FLAGS_RELEASE} \
    -Wl,--out-implib=./release/tfm_psatest_s_CMSE_lib.o \
    -Wl,--cmse-implib \
    -mcpu=cortex-m33 \
    -Wall \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
    -fno-builtin \
    -mthumb \
    -mapcs \
    -Xlinker \
    --gc-sections \
    -Xlinker \
    -static \
    -Xlinker \
    -z \
    -Xlinker \
    muldefs \
    -Xlinker \
    -Map=output.map \
    -Wl,--print-memory-usage \
    ${FPU} \
    ${SPECS} \
    -T${ProjDirPath}/../../../../../../middleware/tfm/tf-m/platform/ext/target/nxp/common/armgcc/tfm_common_s_pre.ld -static \
")
