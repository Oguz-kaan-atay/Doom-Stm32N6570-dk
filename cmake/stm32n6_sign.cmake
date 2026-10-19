# STM32N6 imaj imzalama: ELF -> BIN -> Boot ROM başlıklı "-trusted.bin"


find_program(STM32_SIGNING_TOOL STM32_SigningTool_CLI
    HINTS "C:/Program Files/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin"
    REQUIRED
)

function(stm32n6_sign target)
    set(bin    ${target}.bin)
    set(signed ${target}-trusted.bin)

    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE_NAME:${target}> ${bin}
        COMMAND ${STM32_SIGNING_TOOL} -s -bin ${bin} -nk -of 0x80000000 -t fsbl -hv 2.3 -o ${signed}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/${bin} ${CMAKE_CURRENT_BINARY_DIR}/${signed}
        COMMENT "Imzalaniyor: ${signed}"
        VERBATIM
    )
endfunction()
