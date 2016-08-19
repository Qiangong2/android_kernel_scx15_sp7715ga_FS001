obj-y += irq.o arch_init.o arch_lock.o \
         reserve.o efuse.o common.o glb.o

obj-$(CONFIG_SC_INTERNAL_ADI) += adi.o adie_irq.o
obj-$(CONFIG_SC_INTERNAL_ADC) += adc.o
obj-$(CONFIG_SC_INTERNAL_WATCHDOG) += watchdog.o

obj-$(CONFIG_ARCH_SCX35) += devices-sc8830.o io-sc8830.o  clock-sc8830.o arch_misc.o pin_switch.o syssleep.o
obj-$(CONFIG_ARCH_SCX35) += timer_sc8830.o
obj-$(CONFIG_PM_DEVFREQ) += dmc_freq.o

obj-$(CONFIG_PM) += pm.o
obj-$(CONFIG_FIX_V7TAGRAM_BUG) += fix_v7_tag_ram_bug.o
obj-$(CONFIG_CPU_FREQ) += cpufreq-scx35.o
ifeq ($(CONFIG_BUS_MONITOR),y)
obj-$(CONFIG_ARCH_SCX35) += bm_sc8830.o
endif

ifeq ($(CONFIG_CPU_IDLE),y)
	obj-$(CONFIG_ARCH_SCX35) += cpuidle-scx35.o
endif

ifeq ($(CONFIG_PM),y)
	obj-$(CONFIG_ARCH_SCX35) += pm-scx35.o standby-scx35.o pm_debug_scx35.o
endif
obj-$(CONFIG_SCX35_DMC_FREQ_AP) += dmc_freq_ap.o
obj-$(CONFIG_SPRD_DCDC_DEBUG) += dcdc_debug.o
obj-$(CONFIG_SMP) += platsmp.o headsmp.o
obj-$(CONFIG_HOTPLUG_CPU) += hotplug.o
obj-$(CONFIG_SC_VIBRATOR) += vibrator.o
obj-$(CONFIG_SIPC) += sipc.o
ifeq ($(CONFIG_BCMDHD),m)
obj-y += dhd_adapter.o
endif
obj-$(CONFIG_MACH_SPX35EB) += board-sp8830eb.o dma_r4p0.o
obj-$(CONFIG_MACH_SP8835EB) += board-sp8835eb.o dma_r4p0.o
obj-$(CONFIG_MACH_SPX15FPGA) += board-sp8830fpga.o pinmap-sp8830fpga.o dma_r4p0.o
obj-$(CONFIG_MACH_SPX35EC) += board-sp8830ec.o dma_r4p0.o
obj-$(CONFIG_MACH_SC9620OPENPHONE) += board-sc9620openphone.o dma_r4p0.o
obj-$(CONFIG_MACH_SC9620FPGA) += board-sc9620fpga.o dma_r4p0.o
obj-$(CONFIG_MACH_KANAS_W) += board-kanas_w.o dma_r4p0.o
obj-$(CONFIG_MACH_KANAS_TD) += board-kanas_td.o dma_r4p0.o
obj-$(CONFIG_MACH_SP7715EATRISIM) += board-sp7715eatrisim.o dma_r4p0.o
ifeq ($(CONFIG_ZYT_PRJ),7671)
	obj-$(CONFIG_MACH_SP7715EA_7671) += board-sp7715ea_7671.o dma_r4p0.o
else
	ifeq ($(CONFIG_ZYT_PRJ),7001)
		obj-$(CONFIG_MACH_SP7715EA_FS001) += board-sp7715ea_FS001.o dma_r4p0.o
	else
	    ifeq ($(CONFIG_ZYT_PRJ),7675)
		    obj-$(CONFIG_MACH_SP7715EA_7675) += board-sp7715ea_7675.o dma_r4p0.o
		    obj-$(CONFIG_MACH_SP7715EA_7675_HVGA) += board-sp7715ea_7675_hvga.o dma_r4p0.o
	    else
	    	ifeq ($(CONFIG_ZYT_PRJ),7676)
	    		obj-$(CONFIG_MACH_SP7715EA_7676) += board-sp7715ea_7676.o dma_r4p0.o
	    	else
	    		ifeq ($(CONFIG_ZYT_PRJ),7003)
	    			obj-$(CONFIG_MACH_SP7715EA_FS003) += board-sp7715ea_FS003.o dma_r4p0.o
	    			obj-$(CONFIG_MACH_SP7715EA_FS003_HVGA) += board-sp7715ea_FS003.o dma_r4p0.o
	    		else
		              ifeq ($(CONFIG_ZYT_PRJ),7006)
	    	 	          obj-$(CONFIG_MACH_SP7715EA_FS006) += board-sp7715ea_FS006.o dma_r4p0.o
                              else
                                     ifeq ($(CONFIG_ZYT_PRJ),7007)
                                     	obj-$(CONFIG_MACH_SP7715EA_FS007_HVGA) += board-sp7715ea_FS007_hvga.o dma_r4p0.o
	            	             else
	            	             	obj-$(CONFIG_MACH_SP7715EA) += board-sp7715ea.o dma_r4p0.o
	            	             endif
			      endif
	            endif
	        endif
	    endif
    endif
endif
obj-$(CONFIG_MACH_SP7715EGOOD) += board-sp7715egood.o dma_r4p0.o
obj-$(CONFIG_MACH_SP7715EAOPENPHONE) += board-sp7715eaopenphone.o dma_r4p0.o
obj-$(CONFIG_MACH_SP7715GATRISIM) += board-sp7715gatrisim.o dma_r4p0.o
ifeq ($(CONFIG_ZYT_PRJ),7672)
	obj-$(CONFIG_MACH_SP7715GA_7672) += board-sp7715ga_7672.o dma_r4p0.o
else
	ifeq ($(CONFIG_ZYT_PRJ),7673)
		obj-$(CONFIG_MACH_SP7715GA_7673) += board-sp7715ga_7673.o dma_r4p0.o
	else
		ifeq ($(CONFIG_ZYT_PRJ),5701)
			obj-$(CONFIG_MACH_SP7715GA_5701) += board-sp7715ga_5701.o dma_r4p0.o
		else
			ifeq ($(CONFIG_ZYT_PRJ),7676)
				obj-$(CONFIG_MACH_SP7715GA_7676) += board-sp7715ga_7676.o dma_r4p0.o
			else
				ifeq ($(CONFIG_ZYT_PRJ),7003)
					obj-$(CONFIG_MACH_SP7715GA_FS003) += board-sp7715ga_FS003.o dma_r4p0.o
				else
					ifeq ($(CONFIG_ZYT_PRJ),7001)
						obj-$(CONFIG_MACH_SP7715GA_FS001) += board-sp7715ga_FS001.o dma_r4p0.o
					else
						ifeq ($(CONFIG_ZYT_PRJ),7675)
							obj-$(CONFIG_MACH_SP7715GA_7675_HVGA) += board-sp7715ga_7675_hvga.o dma_r4p0.o
							obj-$(CONFIG_MACH_SP7715GA_7675) += board-sp7715ga_7675.o dma_r4p0.o
                                                else
                                                	  ifeq ($(CONFIG_ZYT_PRJ),7007)
                                                	  	   obj-$(CONFIG_MACH_SP7715GA_FS007_HVGA) += board-sp7715ga_FS007_hvga.o dma_r4p0.o
                                                          else
						                  obj-$(CONFIG_MACH_SP7715GA) += board-sp7715ga.o dma_r4p0.o
						         endif
						endif
					endif
				endif
			endif
		endif
	endif
endif

ifeq ($(CONFIG_ZYT_PRJ),7271)
obj-$(CONFIG_MACH_SP6815GA_7271) += board-sp6815ga_7271.o dma_r4p0.o
obj-$(CONFIG_MACH_SP6815GA_7271_WVGA) += board-sp6815ga_7271_wvga.o dma_r4p0.o
obj-$(CONFIG_MACH_SP6815EA_7271) += board-sp6815ea_7271.o dma_r4p0.o
else
    ifeq ($(CONFIG_ZYT_PRJ),7010)
    obj-$(CONFIG_MACH_SP6815GA_FS010) += board-sp6815ga_FS010.o dma_r4p0.o
    else
    	ifeq ($(CONFIG_ZYT_PRJ),7011)
    	obj-$(CONFIG_MACH_SP6815GA_FS011_WVGA) += board-sp6815ga_FS011_wvga.o dma_r4p0.o
    	obj-$(CONFIG_MACH_SP6815EA_FS011_WVGA) += board-sp6815ea_FS011_wvga.o dma_r4p0.o
    	else    	
	obj-$(CONFIG_MACH_SP6815GA) += board-sp6815ga.o dma_r4p0.o
    	obj-$(CONFIG_MACH_SP6815EA) += board-sp6815ea.o dma_r4p0.o
    	endif
    endif
endif

obj-$(CONFIG_MACH_SP8815GA) += board-sp8815ga.o dma_r4p0.o
obj-$(CONFIG_MACH_SP8815GAOPENPHONE) += board-sp8815gaopenphone.o dma_r4p0.o
obj-$(CONFIG_MACH_POCKET2) += board-pocket2.o dma_r4p0.o
obj-$(CONFIG_MACH_CORSICA_VE) += board-corsica_ve.o dma_r4p0.o
obj-$(CONFIG_MACH_YOUNG2) += board-young2.o dma_r4p0.o
obj-$(CONFIG_MACH_HIGGS) += board-higgs.o dma_r4p0.o
obj-$(CONFIG_MACH_VIVALTO) += board-vivalto.o dma_r4p0.o
obj-$(CONFIG_MACH_SP5735EA) += board-sp5735ea.o dma_r4p0.o
obj-$(CONFIG_MACH_SP5735C1EA) += board-sp5735c1ea.o dma_r4p0.o
obj-$(CONFIG_MACH_SP8830GA) += board-sp8830ga.o dma_r4p0.o
obj-$(CONFIG_MACH_SP7730GA) += board-sp7730ga.o dma_r4p0.o
obj-$(CONFIG_MACH_SP7730EC) += board-sp7730ec.o dma_r4p0.o
obj-$(CONFIG_MACH_SP7730ECTRISIM) += board-sp7730ectrisim.o dma_r4p0.o
obj-$(CONFIG_SPRD_DEBUG) += sprd_debug.o
obj-$(CONFIG_SPRD_MEM_POOL) += sprd_mem_pool.o
obj-$(CONFIG_SPRD_AVS_DEBUG) += avs_debug.o
obj-$(CONFIG_SPRD_AUX_DEBUG) += aux_debug.o
