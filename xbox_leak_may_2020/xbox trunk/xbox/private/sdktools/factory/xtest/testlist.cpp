#include "stdafx.h"

#define MODULE_REGISTRAR(m, t) "CXModule" #m "Test" #t
#define LINKER_INCLUDE_MODULE(m, t) \
    "/include:?m_registrar@" MODULE_REGISTRAR(m, t) "@@2VCTestRegistrar@@A"

//
// Tell linker to include the follow test modules
//

#pragma comment(linker, LINKER_INCLUDE_MODULE(CPU, feature_check))
#pragma comment(linker, LINKER_INCLUDE_MODULE(CPU, fpu_stress))
#pragma comment(linker, LINKER_INCLUDE_MODULE(CPU, cpu_internal_speed))
#pragma comment(linker, LINKER_INCLUDE_MODULE(CPU, multimedia_extensions))
#pragma comment(linker, LINKER_INCLUDE_MODULE(CPU, simd_new_instructions))

#pragma comment(linker, LINKER_INCLUDE_MODULE(RTC, datetimedumputility))
#pragma comment(linker, LINKER_INCLUDE_MODULE(RTC, datetimeaccuracytest))
#pragma comment(linker, LINKER_INCLUDE_MODULE(RTC, ramtest))
#pragma comment(linker, LINKER_INCLUDE_MODULE(RTC, interrupttest))

#pragma comment(linker, LINKER_INCLUDE_MODULE(Tool, report_xmta_versions_and_checksums))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Tool, set_display_mode))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Tool, find_free_memory))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Tool, compare_dvd_firmware_revision))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Tool, mount_utility_drive))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Tool, unlock_xbox_hard_drive))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Tool, compare_scart_status_voltage))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Tool, check_for_uem_error))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Tool, compare_kernel_revision))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Tool, get_random_number))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Tool, set_uem_state))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Tool, check_chipset_revisions))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Tool, clear_uem_error_history))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Tool, mount_dashboard_partition))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Tool, image_hard_drive))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Tool, verify_per_box_information))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Tool, delete_file))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Tool, write_per_box_information))

#pragma comment(linker, LINKER_INCLUDE_MODULE(Audio, analog_silence))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Audio, digital_playback))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Audio, digital_accuracy))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Audio, analog_playback))

#pragma comment(linker, LINKER_INCLUDE_MODULE(DVD, file_system_checksum))
#pragma comment(linker, LINKER_INCLUDE_MODULE(DVD, file_sequential_read))
#pragma comment(linker, LINKER_INCLUDE_MODULE(DVD, dvd_random_read))
#pragma comment(linker, LINKER_INCLUDE_MODULE(DVD, physical_device_sequential_read))
#pragma comment(linker, LINKER_INCLUDE_MODULE(DVD, dvd_file_sequential_read))
#pragma comment(linker, LINKER_INCLUDE_MODULE(DVD, free_space_random_wrc))

#pragma comment(linker, LINKER_INCLUDE_MODULE(HardDisk, dvd_random_read))
#pragma comment(linker, LINKER_INCLUDE_MODULE(HardDisk, physical_device_sequential_read))
#pragma comment(linker, LINKER_INCLUDE_MODULE(HardDisk, free_space_random_wrc))
#pragma comment(linker, LINKER_INCLUDE_MODULE(HardDisk, file_sequential_read))
#pragma comment(linker, LINKER_INCLUDE_MODULE(HardDisk, file_system_checksum))
#pragma comment(linker, LINKER_INCLUDE_MODULE(HardDisk, dvd_file_sequential_read))

#pragma comment(linker, LINKER_INCLUDE_MODULE(Memory, MemorySizeCheck))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Memory, CacheRandomDwordOps))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Memory, WindowedCacheRandomDwordOps))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Memory, MemoryTransfer))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Memory, WindowedCacheMixedOps))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Memory, DisplayMemoryStatus))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Memory, CacheRandomData))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Memory, MemoryRandomData))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Memory, CacheMixedOps))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Memory, DDRBusNoise))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Memory, MemoryPerformance))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Memory, WindowedCacheUniqueness))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Memory, MemorySlidingOnesAndZeros))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Memory, DDRBusPowersof2))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Memory, DDRMemoryBanks))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Memory, DDR32KStride))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Memory, CacheSlidingOnesAndZeros))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Memory, WindowedCacheRandomData))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Memory, CacheAddressPatterns))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Memory, DDRDeviceStride))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Memory, MemoryAddressPatterns))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Memory, MemoryMixedOps))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Memory, CachePerformance))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Memory, QBusNoise))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Memory, MemoryRandomDwordOps))

#pragma comment(linker, LINKER_INCLUDE_MODULE(Grafx, 3DSurface))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Grafx, HDTV))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Grafx, Shapes))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Grafx, Vidmemory))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Grafx, Vidmemory_patterns))
#pragma comment(linker, LINKER_INCLUDE_MODULE(Grafx, Ani))

#pragma comment(linker, LINKER_INCLUDE_MODULE(TV, ntscmsvcb))
#pragma comment(linker, LINKER_INCLUDE_MODULE(TV, ntscjcompcb))
#pragma comment(linker, LINKER_INCLUDE_MODULE(TV, ntscmcompcb))
#pragma comment(linker, LINKER_INCLUDE_MODULE(TV, ntscjsvcb))
#pragma comment(linker, LINKER_INCLUDE_MODULE(TV, ntscjcompmb))
#pragma comment(linker, LINKER_INCLUDE_MODULE(TV, ntscmcompmb))
#pragma comment(linker, LINKER_INCLUDE_MODULE(TV, ntscjsvmb))
#pragma comment(linker, LINKER_INCLUDE_MODULE(TV, ntscmsvmb))
#pragma comment(linker, LINKER_INCLUDE_MODULE(TV, secamcompcb))
#pragma comment(linker, LINKER_INCLUDE_MODULE(TV, palnsvpcb))
#pragma comment(linker, LINKER_INCLUDE_MODULE(TV, secamsvcb))
#pragma comment(linker, LINKER_INCLUDE_MODULE(TV, palbsvcb))
#pragma comment(linker, LINKER_INCLUDE_MODULE(TV, palmcompcb))
#pragma comment(linker, LINKER_INCLUDE_MODULE(TV, palbcompcb))
#pragma comment(linker, LINKER_INCLUDE_MODULE(TV, palmsvcb))
#pragma comment(linker, LINKER_INCLUDE_MODULE(TV, palncompcb))
#pragma comment(linker, LINKER_INCLUDE_MODULE(TV, palnsvpmb))
#pragma comment(linker, LINKER_INCLUDE_MODULE(TV, palbcompmb))
#pragma comment(linker, LINKER_INCLUDE_MODULE(TV, secamsvmb))
#pragma comment(linker, LINKER_INCLUDE_MODULE(TV, palmcompmb))
#pragma comment(linker, LINKER_INCLUDE_MODULE(TV, secamcompmb))
#pragma comment(linker, LINKER_INCLUDE_MODULE(TV, palmsvmb))
#pragma comment(linker, LINKER_INCLUDE_MODULE(TV, palbsvmb))
#pragma comment(linker, LINKER_INCLUDE_MODULE(TV, palncompmb))
#pragma comment(linker, LINKER_INCLUDE_MODULE(TV, tv_linearity))

#pragma comment(linker, LINKER_INCLUDE_MODULE(USB, enumerationtest))
#pragma comment(linker, LINKER_INCLUDE_MODULE(USB, memoryunitstresstest))

#pragma comment(linker, LINKER_INCLUDE_MODULE(SMC, AudioClampUtility))
#pragma comment(linker, LINKER_INCLUDE_MODULE(SMC, DVDTrayStatesandEmptyTest))
#pragma comment(linker, LINKER_INCLUDE_MODULE(SMC, LEDSystemTest))
#pragma comment(linker, LINKER_INCLUDE_MODULE(SMC, AbortRegulateTemperatureUtility))
#pragma comment(linker, LINKER_INCLUDE_MODULE(SMC, RevisionCheckTest))
#pragma comment(linker, LINKER_INCLUDE_MODULE(SMC, SystemPowerCycleUtility))
#pragma comment(linker, LINKER_INCLUDE_MODULE(SMC, LEDBoardTest))
#pragma comment(linker, LINKER_INCLUDE_MODULE(SMC, ReadTempUtility))
#pragma comment(linker, LINKER_INCLUDE_MODULE(SMC, SystemResetUtility))
#pragma comment(linker, LINKER_INCLUDE_MODULE(SMC, DVDSpinDownUtility))
#pragma comment(linker, LINKER_INCLUDE_MODULE(SMC, RegulateTemperatureUtility))
#pragma comment(linker, LINKER_INCLUDE_MODULE(SMC, VModeTest))
#pragma comment(linker, LINKER_INCLUDE_MODULE(SMC, DVDOpenCloseMediaDetectTest))
#pragma comment(linker, LINKER_INCLUDE_MODULE(SMC, AirSensorCalibrationUtility))
#pragma comment(linker, LINKER_INCLUDE_MODULE(SMC, FanSpeedBoardTest))
#pragma comment(linker, LINKER_INCLUDE_MODULE(SMC, SystemShutdownUtility))
#pragma comment(linker, LINKER_INCLUDE_MODULE(SMC, BootCountGateUtility))
#pragma comment(linker, LINKER_INCLUDE_MODULE(SMC, ThermalOverloadUtility))
#pragma comment(linker, LINKER_INCLUDE_MODULE(SMC, ThermalTest))
#pragma comment(linker, LINKER_INCLUDE_MODULE(SMC, FanSpeedSystemTest))

#pragma comment(linker, LINKER_INCLUDE_MODULE(LAN, UDPResponder))
