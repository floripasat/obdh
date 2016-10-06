################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"/home/elder/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.6.0.STS/bin/cl430" -vmspx --data_model=large --use_hw_mpy=F5 --include_path="/home/elder/ti/ccsv6/ccs_base/msp430/include" --include_path="/home/elder/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.6.0.STS/include" --include_path="/code/floripasat/obdh/obdh_v2/FreeRTOS" --include_path="/code/floripasat/obdh/obdh_v2/FreeRTOS/include" --include_path="/code/floripasat/obdh/obdh_v2/FreeRTOS/portable/MemMang" --include_path="/code/floripasat/obdh/obdh_v2/FreeRTOS/portable/CCS/MSP430X/" --include_path="/code/floripasat/obdh/obdh_v2/Fsat/" --include_path="/code/floripasat/obdh/obdh_v2/hal/" --include_path="/code/floripasat/obdh/obdh_v2/util/" --include_path="/code/floripasat/obdh/obdh_v2/" --advice:power="all" --define=__MSP430F6659__ -g --c99 --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="main.d" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '


